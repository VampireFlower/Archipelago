import mgtt
import json
import shutil
import struct
import subprocess

from pathlib import Path

game_dir = Path(r"G:\mgtt\discs\compressed\EN".strip('"'))


class DOL:

    text_offsets = 0x00
    text_load_to = 0x48
    text_sizes   = 0x90

    def __init__(self, path_to_dol: Path):

        self.data = bytearray(path_to_dol.stat().st_size)

        self.file = open(path_to_dol, 'rb+')

        self.file.readinto(self.data)

    def iter_text(self):

        for i in range(7):
            i *= 4
            yield (self.read_int(self.text_offsets + i),
                   self.read_int(self.text_load_to + i),
                   self.read_int(self.text_sizes   + i))

    def read_int(self, offset):
        return struct.unpack_from(">I", self.data, offset)[0]

    def write_int(self, x, offset):
        x &= 0xFFFFFFFF
        struct.pack_into(">I", self.data, offset, x)

    def write_bytes(self, x, offset):
        self.data[offset:offset+len(x)] = x

    def pad(self, n):
        while len(self.data) % n:
            self.data.append(0)

    # Obtain the location in the dol that backs a specified memory address
    def address_to_offset(self, address):
        for file_offset, load_to, size in self.iter_text():
            if load_to <= address < load_to + size:
                return file_offset + address - load_to

    def add_text(self, blob, load_address):

        # if self.address_to_offset(load_address) \
        # or self.address_to_offset(load_address+len(blob)):
        #     raise Exception(f"Cannot load blob to {load_address} because it is already taken!")

        self.pad(32)

        for i, (offset, _, _) in enumerate(self.iter_text()):
            i *= 4
            if offset == 0:

                self.write_int(len(self.data), self.text_offsets+i)
                self.write_int(load_address, self.text_load_to+i)
                self.write_int(len(blob), self.text_sizes+i)

                self.data.extend(blob)

                break
        else:
            raise Exception("Maximum number of sections already in dol!")

        self.pad(32)

    def save(self):
        self.file.seek(0)
        self.file.write(self.data)
        self.file.close()



dol = DOL(game_dir/'sys'/'main.dol')


patch = Path(__file__).parent

build = patch/'build'
src   = patch/'src'

if build.exists(): shutil.rmtree(build)

build.mkdir()




source_files = [file for file in src.rglob('*.c')]

args = ['powerpc-eabi-gcc',
        '-Os',        # optimize for size
        '-c',         # output object files
        '-fno-pic',   # generate code that expects to load at a fixed address
        '-mno-sdata', # SDA is already taken
        '-fno-asynchronous-unwind-tables' # omit section .eh_frame
        ] + source_files

# -nostdlib controls how linker is driven, so we don't need it here

#args.append("-ffreestanding")

gcc = subprocess.run(args, cwd=build, capture_output=True, text=True)

if gcc.returncode != 0:
    print(gcc.stderr)
    raise Exception("Compilation failed!")




object_files = [file for file in build.glob("*.o")]

args = ['powerpc-eabi-ld',
        '-T', patch/'link.ld',
        '-o', 'blob.elf'
        ] + object_files

ld = subprocess.run(args, cwd=build, capture_output=True, text=True)

if ld.returncode != 0:
    print(ld.stderr)
    raise Exception("Linking failed!")




args = ['powerpc-eabi-objcopy',
        '-O', 'binary',
        'blob.elf',
        'dump.bin'
        ]

dump = subprocess.run(args, cwd=build, capture_output=True, text=True)

if dump.returncode != 0:
    print(dump.stderr)
    raise Exception("Extracting code failed!")




args = ['powerpc-eabi-nm',
        '-n',
        'blob.elf'
        ]

nm = subprocess.run(args, cwd=build, capture_output=True, text=True)

if nm.returncode != 0:
    print(nm.stderr)
    raise Exception("Getting symbol information failed!")

# nm.sdout:                symbols:
# 8001a670 T MyFunction -> {'MyFunction': 2147591792}
symbols = {}
for line in nm.stdout.splitlines():
    address, type, symbol = line.split(' ')
    
    if type == 'A': continue # not novel information
    
    symbols[symbol] = int(address, 16)

json.dump(symbols, open(build/"symbols.txt", "w"), indent = 4)

hooks = json.load(open(patch/'hooks.json'))

for hook in hooks:

    hook["target"] = symbols[hook["target"]]

    # Write hooks to absolute memory addresses.
    match hook["file"]:
        
        case 'main.dol':
            trampoline = dol.address_to_offset(hook["origin"])
            branch = mgtt.gecko.make_bl(hook["origin"], hook["target"])
            dol.write_bytes(branch, trampoline)
        
        case 'overlay_golf':
            pass

        case 'overlay_menu':
            pass

    # TODO: write hooks to relative memory addresses (0x00001400 instead of 0x80001400)

dol.add_text((build/'dump.bin').read_bytes(), 0x80127f60)
dol.save()

print("\nBuild complete. Wahoo!")
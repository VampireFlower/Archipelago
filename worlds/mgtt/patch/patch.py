import mgtt
import json
import shutil
import struct
import subprocess
import sys

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
        if to_add := -len(self.data) % n:
            self.data.extend(bytes(to_add))

    # Obtain the location in the dol that backs a specified memory address
    def address_to_offset(self, address):
        for file_offset, load_to, size in self.iter_text():
            if load_to <= address < load_to + size:
                return file_offset + address - load_to

    def add_text(self, blob: bytes, load_address):

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
clv = (game_dir/'files'/'C'/'L'/'V')
buc = (game_dir/'files'/'B'/'U'/'C')
clv_bytes = bytearray(mgtt.decompress(clv.read_bytes())[0])
buc_bytes = bytearray(mgtt.decompress(buc.read_bytes())[0])


patch = Path(__file__).parent

build = patch/'build'
src   = patch/'src'

if build.exists(): shutil.rmtree(build)

build.mkdir()


hooks = json.load(open(patch/'hooks.json'))

# add symbol names for hook locations
with open(build/"auto_symbols.ld", "w") as f:
    for hook in hooks:
        f.write(f"{hook['name']} = {hook['origin']};\n")


source_files = [file for file in src.rglob('*.[sc]')]

args = [patch/sys.platform/'gcc',
        '-Os',        # optimize for size
        '-c',         # output object files
        '-fno-pic',   # generate code that expects to load at a fixed address
        '-mno-sdata', # SDA is already taken
        '-fno-asynchronous-unwind-tables', # omit section .eh_frame
        '-B', patch/sys.platform, # tell gcc where to find executables it depends on
        '-mregnames'  # allow usage of register names in assembly source 
        ] + source_files

#args.append("-ffreestanding")

gcc = subprocess.run(args, cwd=build, capture_output=True, text=True)

if gcc.returncode != 0:
    print(gcc.stderr)
    raise Exception("Compilation failed!")




object_files = [file for file in build.glob("*.o")]

args = [patch/sys.platform/'ld',
        '-T', patch/'link.ld',
        '-o', 'blob.elf'
        ] + object_files

ld = subprocess.run(args, cwd=build, capture_output=True, text=True)

if ld.returncode != 0:
    print(ld.stderr)
    raise Exception("Linking failed!")




args = [patch/sys.platform/'objcopy',
        '-O', 'binary', # output type
        'blob.elf',     # input
        'dump.bin'      # output
        ]

dump = subprocess.run(args, cwd=build, capture_output=True, text=True)

if dump.returncode != 0:
    print(dump.stderr)
    raise Exception("Extracting code failed!")




args = [patch/sys.platform/'nm',
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
    address, kind, symbol = line.split(' ')
    
    if kind == 'A': continue # not novel information
    
    symbols[symbol] = int(address, 16)

# Dump symbols for debugging purposes
pad = max(len(symbol) + 1 for symbol in symbols)
with open(build/"symbols.txt", "w") as f:
    for symbol, address in symbols.items():
        symbol = (symbol+":").ljust(pad)
        f.write(f"{symbol} {address:08x}\n")




for hook in hooks:

    # if the target is a symbol name, use the address of that symbol name
    if type(hook["target"]) == str:
        hook["target"] = symbols[hook["target"]]

    match hook["file"]:
        
        case 'dol':

            trampoline = dol.address_to_offset(hook["origin"])

            if hook['type'] == 'bl':
                branch = mgtt.gecko.make_bl(hook["origin"], hook["target"])
            elif hook['type'] == 'b':
                branch = mgtt.gecko.make_b(hook["origin"], hook["target"])

            dol.write_bytes(branch, trampoline)
        
        case 'golf':

            if hook['type'] == 'bl':
                branch = mgtt.gecko.make_bl(hook["origin"], hook["target"])
            elif hook['type'] == 'b':
                branch = mgtt.gecko.make_b(hook["origin"], hook["target"])

            trampoline = hook['origin'] - 0x80400000
            clv_bytes[trampoline:trampoline+4] = branch


        case 'menu':

            if hook['type'] == 'bl':
                branch = mgtt.gecko.make_bl(hook["origin"], hook["target"])
            elif hook['type'] == 'b':
                branch = mgtt.gecko.make_b(hook["origin"], hook["target"])

            trampoline = hook['origin'] - 0x80400000
            buc_bytes[trampoline:trampoline+4] = branch




dol.add_text((build/'dump.bin').read_bytes(), 0x80127f60)
dol.save()
clv.write_bytes(mgtt.compress(clv_bytes))
buc.write_bytes(mgtt.compress(buc_bytes))


print("\nBuild complete. Wahoo!")
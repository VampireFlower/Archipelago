import mgtt
import shutil
import subprocess
import sys

from pathlib import Path


from hooks import hooks





patch = Path(__file__).parent

build = patch/'build'
src   = patch/'src'


if build.exists():
    shutil.rmtree(build)
    subprocess.run(['git', '-C', mgtt.REPO_ROOT, 'restore', 'discs'])

build.mkdir()


game_dir = mgtt.get_disc("compressed")


dol = mgtt.DOL(game_dir/'sys'/'main.dol')
clv = (game_dir/'files'/'C'/'L'/'V')
buc = (game_dir/'files'/'B'/'U'/'C')
clv_bytes = mgtt.decompress(clv.read_bytes())[0]
buc_bytes = mgtt.decompress(buc.read_bytes())[0]





source_files = [file for file in src.rglob('*.[sc]')]


args = [patch/sys.platform/'gcc',
        '-Os',        # optimize for size
        '-c',         # output object files
        '-fno-pic',   # generate code that expects to load at a fixed address
        '-mregnames', # allow usage of register names in assembly source
        '-mno-sdata', # SDA is already taken
        '-B', patch/sys.platform, # tell gcc where to find executables it depends on
        '-I', patch/'include',
        '-fno-asynchronous-unwind-tables' # omit section .eh_frame
        ] + source_files

args.append("-fno-use-linker-plugin")
#args.append("-ffreestanding")


gcc = subprocess.run(args, cwd=build, text=True)

if gcc.returncode != 0:
    raise Exception("Compilation failed!")



# add symbol names for hook locations
with open(build/"auto_symbols.ld", "w") as f:
    for hook in hooks:
        f.write(f"{hook['name']} = {hook['origin']:#x};\n")




object_files = [file for file in build.glob("*.o")]

args = [patch/sys.platform/'ld',
        '-T', patch/'link.ld',
        '-o', 'blob.elf'
        ] + object_files

ld = subprocess.run(args, cwd=build, text=True)

if ld.returncode != 0:
    raise Exception("Linking failed!")




args = [patch/sys.platform/'objcopy',
        '-O', 'binary', # output type
        'blob.elf',     # input
        'dump.bin'      # output
        ]

dump = subprocess.run(args, cwd=build, text=True)

if dump.returncode != 0:
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
    address, kind, symbol = line.split()
    
    if kind == 'A': continue # not novel information
    
    symbols[symbol] = int(address, 16)

# Dump symbols for debugging purposes
pad = max(len(symbol) + 1 for symbol in symbols)
with open(build/"symbols.txt", "w") as f:
    for symbol, address in symbols.items():
        symbol = (symbol+":").ljust(pad)
        f.write(f"{symbol} {address:08x}\n")




funcs = {
    "b":  mgtt.ppc.make_b,
    "bl": mgtt.ppc.make_bl,
}

for hook in hooks:

    # if the target is a symbol name, use the address of that symbol name
    if type(hook["target"]) == str:
        hook["target"] = symbols[hook["target"]]

    branch = funcs[hook['type']](hook["origin"], hook["target"])

    match hook["file"]:
        
        case 'dol':

            trampoline = dol.address_to_offset(hook["origin"])        

            dol.write_bytes(branch, trampoline)
        
        case 'golf':

            trampoline = hook['origin'] - 0x80400000
            clv_bytes[trampoline:trampoline+4] = branch


        case 'menu':

            trampoline = hook['origin'] - 0x80400000
            buc_bytes[trampoline:trampoline+4] = branch




dol.add_text((build/'dump.bin').read_bytes(), 0x805247b4)
dol.save()
clv.write_bytes(mgtt.compress(clv_bytes))
buc.write_bytes(mgtt.compress(buc_bytes))


print("\nBuild complete. Wahoo!")
import mgtt
import shutil
import struct
import subprocess
import sys
from pathlib import Path


DEBUG_ASM = False
GECKO     = False


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


from hooks import hooks
# add symbol names for hook locations
with open(build/"auto_symbols.ld", "w") as f:
    for hook in hooks:
        f.write(f"{hook['name']} = {hook['origin']:#x};\n")


source_files = [file for file in src.rglob('*.[Ssc]')]


args = [patch/sys.platform/'gcc',
        '-Os',        # optimize for size
        '-fno-pic',   # generate code that expects to load at a fixed address
        '-nostdlib',  # do not pull in the C runtime
        '-mregnames', # allow usage of register names in assembly source
        '-mno-sdata', # SDA is already taken
        '-T', patch/'link.ld',
        '-I', patch/'include',
        '-B', patch/sys.platform, # tell gcc where to find executables it depends on
        '-fno-asynchronous-unwind-tables', # omit section .eh_frame
        '-S' if DEBUG_ASM else '-oblob.elf'
        ] + source_files

if subprocess.run(args, cwd=build, text=True).returncode != 0:
    raise Exception("Compilation failed!")

if DEBUG_ASM:
    print("Success!\nCompiled:", [file.stem for file in source_files if file.suffix == '.c'])
    exit()




args = [patch/sys.platform/'objcopy',
        '-O', 'binary', # output type
        'blob.elf',     # input
        'dump.bin'      # output
        ]

if subprocess.run(args, cwd=build, text=True).returncode != 0:
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
    if kind == 't': continue # ignore local labels
    
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




def make_hook_branch(hook) -> bytes:
    match hook['type']:
        case 'b':
            return mgtt.ppc.make_b(hook["origin"], hook["target"])
        case 'bl':
            return mgtt.ppc.make_bl(hook["origin"], hook["target"])




if GECKO == False:

    for hook in hooks:

        branch = make_hook_branch(hook)
        
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

else:

    gecko = ""

    # add hooks to gecko code
    for hook in hooks:

        branch = make_hook_branch(hook)

        gecko += '04' + f'{hook["origin"]:08x}'[2:] + f' {branch.hex()}\n'


    source= f'''
    lis r3,   0x8052
    ori r3,r3,0x47B4 # freespace

    bl foo
    foo:
    mflr r4
    addi r4,r4,32
    li r5,{(build/'dump.bin').stat().st_size}

    lis r0,   0x8007
    ori r0,r0,0x27f8 # memcpy
    mtlr r0

    blrl
    b ret

    # payload
    .incbin "dump.bin"

    ret:
    '''

    args = [patch/sys.platform/'as', '-mregnames', '-o', 'gecko_bootstrap.elf']

    if subprocess.run(args, cwd=build, input=source, text=True).returncode != 0:
        raise Exception("Assembling Gecko code failed!")

    args = [patch/sys.platform/'objcopy',
        '-O', 'binary',
        'gecko_bootstrap.elf',
        'gecko_bootstrap.bin'
        ]

    if subprocess.run(args, cwd=build, text=True).returncode != 0:
        raise Exception("Extracting gecko code failed!")

    bin = bytearray((build/'gecko_bootstrap.bin').read_bytes())

    # code must end with 0x00000000
    if len(bin) % 8 == 0:
        bin.extend(b'\x06\x00\x00\x00\x00\x00\x00\x00')
    else:
        bin.extend(b'\x00\x00\x00\x00')
    
    bootstrap = "\n".join(
        f"{word1:08x} {word2:08x}"
        for word1, word2 in struct.iter_unpack(">II", bin)
    )
    gecko += ("C2018428 " + f"{bootstrap.count('\n')+1:08x}\n"
             + bootstrap + '\n')

    (build/'gecko.txt').write_text(gecko)



print("\nBuild complete. Wahoo!")
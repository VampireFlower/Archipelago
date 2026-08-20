import mgtt
import shutil
import struct
import subprocess
import sys
from pathlib import Path
from hooks import hooks

GECKO = False
DEBUG_ASM = False
OPTIMIZE = True

patch = Path(__file__).parent

build = patch/'build'
src   = patch/'src'


if build.exists():
    shutil.rmtree(build)
    if not DEBUG_ASM and not GECKO:
        subprocess.run(['git', '-C', mgtt.REPO_ROOT, 'restore', 'discs'])

build.mkdir()


game_dir = mgtt.get_disc("compressed")
dol = mgtt.DOL(game_dir/'sys'/'main.dol')
clv = (game_dir/'files'/'C'/'L'/'V')
buc = (game_dir/'files'/'B'/'U'/'C')
clv_bytes = mgtt.decompress(clv.read_bytes())[0]
buc_bytes = mgtt.decompress(buc.read_bytes())[0]


source_files = [file for file in src.rglob("*") if file.suffix in (".S", ".s", ".c", ".cpp")]

cpp = any(".cpp" in file.name for file in source_files)

if cpp:
    hooks.append({
        'name': 'cpp_dynamic_initializers',
        'file': 'dol',
      'origin': 0x8000326c,
      'target': '__init_cpp',
        'type': "b"
    })

# add symbol names for hook locations
with open(build/"hook_symbols.ld", "w") as f:
    for hook in hooks:
        f.write(f"{hook['name']} = {hook['origin']:#x};\n")

args = [patch/sys.platform/'gcc',
        '-O1',        # basic optimization
        '-Wall',      # enable all warnings
        '-Wa,-W',     # make assembler shut up
        '-fno-pic',   # generate code that expects to load at a fixed address
        '-nostdlib',  # do not pull in the C runtime
        '-mregnames', # allow usage of register names in assembly source
        '-mno-sdata', # SDA is already taken
        '-I', patch/'include',
        '-T', patch/'link.ld',
        '-T', patch/'intrinsics.ld',
        '-B', patch/sys.platform, # tell gcc where to find executables it depends on
        '-fno-asynchronous-unwind-tables', # omit section .eh_frame
        '-S' if DEBUG_ASM else '-oblob.elf'
        ] + source_files

if OPTIMIZE and not DEBUG_ASM:
    args[args.index('-O1')] = '-Oz'
    args.extend([
        '-flto', # link time optimization
        "-ffunction-sections",
        "-fdata-sections",
        "-Wl,--gc-sections", # remove unreachable sections after lto pass
    ])

# tell the linker our program's entrypoints
for hook in hooks:
    if type(hook['target']) is str:
        args.append(f"-Wl,--undefined={hook['target']}")

if cpp:
    args.extend([file for file in (patch/'cpp').rglob("*")])
    args.extend(("-fno-rtti", "-fno-exceptions", '-Wno-complain-wrong-lang', '-Wl,--no-demangle'))

if subprocess.run(args, cwd=build).returncode != 0:
    raise Exception("Compilation failed!")

if DEBUG_ASM:
    print("Success!\nCompiled:", [file.name for file in source_files if file.suffix in ('.c', '.cpp')])
    exit()




args = [patch/sys.platform/'objcopy',
        '-O', 'binary', # output type
        'blob.elf',     # input
        'blob.bin'      # output
        ]

if subprocess.run(args, cwd=build).returncode != 0:
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
# 8001a670 T MyFunction -> {'MyFunction': 2147591792, 'T'}

symbols = {}
for line in nm.stdout.splitlines():
    address, kind, symbol = line.split()

    address = int(address, 16)

    # ignore game symbols
    if address < 0x805247c0:
        continue

    symbols[symbol] = {
        'address': address,
        'kind': kind,
    }


# Dump symbols for debugging purposes
kind_names = {
    'T': 'TEXT',
    'D': 'DATA',
    'B': 'BSS',
    'R': 'RODATA',
}

pad = max(len(symbol) + 1 for symbol in symbols)

with open(build/"symbols.txt", "w") as f:
    for kind in ('T', 'D', 'B', 'R'):
        entries = [
            (symbol, info)
            for symbol, info in symbols.items()
            if info['kind'].upper() == kind
        ]

        if not entries:
            continue

        f.write(f"\n[{kind_names[kind]}]\n")

        for symbol, info in entries:
            demangled = (
                ' ' + subprocess.run(
                    [patch/sys.platform/'c++filt', symbol],
                    capture_output=True,
                    text=True,
                ).stdout.rstrip()
                if cpp and symbol.startswith("_Z")
                else ""
            )

            name = (symbol + ":").ljust(pad)
            f.write(f"{name} {info['address']:08x}{demangled}\n")


# Hooks still want just the address.
for hook in hooks:
    if type(hook["target"]) is str:
        hook["target"] = symbols[hook["target"]]['address']




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


    dol.add_text((build/'blob.bin').read_bytes(), 0x805247c0)
    dol.save()
    clv.write_bytes(mgtt.compress(clv_bytes))
    buc.write_bytes(mgtt.compress(buc_bytes))

else:

    hooks04 = ''.join('04' + f'{hook["origin"]:08x}'[2:] + f' {make_hook_branch(hook).hex()}\n' for hook in hooks)


    source= f'''
    lis r3,   0x8052
    ori r3,r3,0x47c0 # freespace

    bl foo
    foo:
    mflr r4
    addi r4,r4,32
    li r5,{(build/'blob.bin').size}

    lis r0,   0x8007
    ori r0,r0,0x27f8 # memcpy
    mtlr r0

    blrl
    b ret

    .incbin "blob.bin"

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

    if subprocess.run(args, cwd=build).returncode != 0:
        raise Exception("Extracting gecko code failed!")

    bin = bytearray((build/'gecko_bootstrap.bin').size)
    open(build/'gecko_bootstrap.bin', 'rb').readinto(bin)

    # code must end with 0x00000000 and be a multiple of 8 bytes
    bin.extend(b'\x60\x00\x00\x00\x00\x00\x00\x00') if len(bin) % 8 == 0 else bin.extend(b'\x00\x00\x00\x00')

    payload = "\n".join(
        f"{word1:08x} {word2:08x}"
        for word1, word2 in struct.iter_unpack(">II", bin)
    )

    gecko = f"C2003268 {len(bin)//8:08x}\n" + payload

    (build/'gecko.txt').write_text(hooks04+gecko)



print("\nBuild complete. Wahoo!")
size=(build/'blob.bin').size
print(size, "bytes")
print(f"{size/0x2B840*100:.2f}% of the budget used")
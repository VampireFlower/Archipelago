import mgtt
import json
import shutil
import subprocess

from pathlib import Path

#game = Path(input("Path to game: ").trim('"'))

patch = Path(__file__).parent

build = patch/'build'
src   = patch/'src'

if build.exists(): shutil.rmtree(build)

build.mkdir()

source_files = [str(file) for file in (patch/'src').rglob('*.c')]

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


object_files = [str(file) for file in build.glob("*.o")]

args = ['powerpc-eabi-ld',
        "-L", str(patch), # search path
        '-T', 'link.ld',  # linker script
        "-o", "blob.elf"
        ] + object_files

ld = subprocess.run(args, cwd=build, capture_output=True, text=True)

if ld.returncode != 0:
    print(ld.stderr)
    raise Exception("Linking failed!")


args = ['powerpc-eabi-nm',
        "-n",
        "blob.elf"
        ]

nm = subprocess.run(args, cwd=build, capture_output=True, text=True)

if nm.returncode != 0:
    print(nm.stderr)
    raise Exception("Getting symbol information failed!")

print(nm.stdout)
print(nm.stdout.__class__)




def address_to_offset(address):
    pass
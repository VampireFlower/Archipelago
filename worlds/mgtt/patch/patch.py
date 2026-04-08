import mgtt

import subprocess

from pathlib import Path

dol = Path(input("Path to main.dol: ").trim('"')).read_bytes()

DOL_SECTION_OFFSETS = [
    0x100,
    0,
    0,
    0,
    0,
    0,
    0,
    0x15DF40,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
]

DOL_SECTION_ADDRESS = [
    0x80003100,
    0,
    0,
    0,
    0,
    0,
    0,
    0x802D7480,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
]

DOL_SECTION_SIZES = [
    0x15DE40,
    0,
    0,
    0,
    0,
    0,
    0,
    0x880,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
]
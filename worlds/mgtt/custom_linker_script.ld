SECTIONS
{
  . = 0x80550000;

  .text : {
    KEEP(*(.init))
    *(.text)
    *(.text.*)
    *(.rodata)
    *(.rodata.*)
  }

  .data : {
    *(.data)
    *(.data.*)
  }

  .bss (NOLOAD) : {
    *(.bss)
    *(.bss.*)
    *(COMMON)
  }

  _end = .;
}
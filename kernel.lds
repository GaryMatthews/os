OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS {
  . = 0x100000;
  .text : {
    PROVIDE(code = .);
    KEEP(*(.multiboot))
    *(.text* .gnu.linkonce.t.*)
  }
  .rodata : {
    PROVIDE(rodata = .);
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }
  .data : {
    PROVIDE(data = .);
    *(.data)
  }
  .bss : {
    PROVIDE(bss = .);
    *(.bss)
  }
  PROVIDE(end = .);
  /DISCARD/ : {
    *(.eh_frame .rel.eh_frame .note.gnu.build-id .note.GNU-stack)
  }
}

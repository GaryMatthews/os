OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS {
  . = 1M;

  /* Multiboot header plus code. */
  .text : ALIGN(4K) {
    PROVIDE(code = .);
    KEEP(*(.multiboot))
    *(.text* .gnu.linkonce.t.*)
  }

  /* Read-only data. */
  .rodata : ALIGN(4K) {
    PROVIDE(rodata = .);
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }

  /* Read-write data (initialized). */
  .data : ALIGN(4K) {
    PROVIDE(data = .);
    *(.data)
  }

  /* Read-write data (uninitialized) and stack. */
  .bss : ALIGN(4K) {
    PROVIDE(bss = .);
    *(COMMON)
    *(.bss)
  }

  PROVIDE(end = .);

  /DISCARD/ : {
    *(.eh_frame .rel.eh_frame .note.gnu.build-id .note.GNU-stack)
  }
}

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

phys = 0x00100000;

SECTIONS {
  /* Multiboot header plus code. */
  .text phys : AT(phys) ALIGN(4K) {
    PROVIDE(code = .);
    KEEP(*(.multiboot))
    *(.text* .stub .gnu.linkonce.t.*)
  }

  /* Read-only data. */
  .rodata : AT(phys + (rodata - code)) ALIGN(4K) {
    PROVIDE(rodata = .);
    *(.rodata* .gnu.linkonce.r.*)
  }

  /* Read-write data (initialized). */
  .data : AT(phys + (data - code)) ALIGN(4K) {
    PROVIDE(data = .);
    *(.data)
  }

  /* Read-write data (uninitialized) and stack. */
  .bss : AT(phys + (bss - code)) ALIGN(4K) {
    PROVIDE(bss = .);
    *(COMMON)
    *(.bss)
  }

  PROVIDE(end = .);

  /DISCARD/ : {
    *(.comment .eh_frame .rel.eh_frame .note.gnu.build-id .note.GNU-stack)
  }
}

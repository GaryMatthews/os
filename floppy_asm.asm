extern floppy_irq_done

global floppy_int
floppy_int:
    pushad
    inc byte [floppy_irq_done]
    mov al, 0x20
    out 0x20, al
    popad
    iretd


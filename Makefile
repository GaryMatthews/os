TARGET ?= i386

CC=gcc
LD=ld
AS=gcc

SRCS = $(wildcard *.[cS])
OBJS = $(addsuffix .o,$(basename $(SRCS)))
KERNEL = kernel.elf

ASFLAGS += -m32 -I.

CFLAGS += -O3
CFLAGS += -m32 -std=gnu17 -pipe -Wall -Wextra -Wunused -fno-stack-protector
CFLAGS += -fno-omit-frame-pointer -ffreestanding -fno-builtin -nostdlib
CFLAGS += -I. -Iinclude
CFLAGS += -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
CFLAGS += -Wno-type-limits
CFLAGS += -Wno-int-conversion
#CFLAGS += -nostdinc

LDFLAGS += -melf_i386 -T kernel.lds -Map kernel.map

QEMU ?= qemu-system-$(TARGET)
QEMUFLAGS += -vga std -m 256M -no-reboot
QEMUFLAGS += -device isa-debug-exit,iobase=0xf4,iosize=0x04
QEMUFLAGS += -enable-kvm
QEMUFLAGS += -soundhw pcspk -soundhw sb16
#QEMUFLAGS += -d in_asm

all: qemu-kernel

iso: $(KERNEL)
	@mkdir -p iso/boot/grub
	@cp $(KERNEL) iso/boot/$(KERNEL)
	@cp grub.cfg iso/boot/grub/grub.cfg
	@grub-mkrescue -o os.iso iso 1>&2 2>/dev/null

qemu-kernel: $(KERNEL)
	$(QEMU) -kernel $(KERNEL) $(QEMUFLAGS) -display none -serial 'mon:stdio'

qemu-iso: iso
	$(QEMU) -cdrom os.iso $(QEMUFLAGS) -boot d -serial 'mon:stdio'

qemu-nox: iso
	$(QEMU) -cdrom os.iso $(QEMUFLAGS) -boot d -display none -serial 'mon:stdio'

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(AS) $(ASFLAGS) -c -o $@ $^

kernel.lst: $(KERNEL)
	objdump -D $(KERNEL) > kernel.lst

clean:
	@rm -rf $(KERNEL) kernel.lst kernel.map $(OBJS) *~ os.iso iso

.PHONY: all iso qemu-kernel qemu-iso qemu-nox clean

TARGET ?= i386

CC=i686-elf-gcc
LD=i686-elf-ld
OBJCOPY=i686-elf-objcopy

SRCS = $(wildcard *.[cS] *.asm) $(wildcard lib/string/*.c)
OBJS = $(addsuffix .o,$(basename $(SRCS))) font.o
KERNEL = kernel.elf

ASFLAGS += -m32 -I.

CFLAGS += -O2 -g -DDEBUG
CFLAGS += -Wall -Wextra -Wunused #-pedantic
CFLAGS += -m32 -std=gnu11 -pipe -fno-stack-protector
CFLAGS += -finline-functions -Wno-missing-field-initializers
CFLAGS += -fno-omit-frame-pointer -ffreestanding -fno-builtin -nostdlib
CFLAGS += -I. -Iinclude
CFLAGS += -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
CFLAGS += -Wno-type-limits -Wno-array-bounds
CFLAGS += -Wno-int-conversion -Wno-sign-compare -Wno-maybe-uninitialized
#CFLAGS += -nostdinc

LDFLAGS += -melf_i386 -T kernel.lds -Map kernel.map

QEMU ?= qemu-system-$(TARGET)
QEMUFLAGS += -vga std -m 512M -no-reboot
QEMUFLAGS += -device isa-debug-exit,iobase=0xf4,iosize=0x04
QEMUFLAGS += -enable-kvm
QEMUFLAGS += -soundhw pcspk -soundhw sb16
QEMUFLAGS += -d in_asm,cpu,guest_errors,exec
QEMUFLAGS += -rtc base=localtime,clock=vm

all: qemu-iso

iso: $(KERNEL)
	@mkdir -p iso/boot/grub
	@cp $(KERNEL) iso/boot/$(KERNEL)
	@cp grub.cfg iso/boot/grub/grub.cfg
	@grub-mkrescue -o os.iso iso 1>&2 2>/dev/null

qemu-kernel: $(KERNEL)
	$(QEMU) -kernel $(KERNEL) $(QEMUFLAGS) -display none -serial 'mon:stdio'

qemu-iso: iso
	$(QEMU) -cdrom os.iso $(QEMUFLAGS) -boot d,menu=off -serial 'mon:stdio'

qemu-nox: iso
	$(QEMU) -cdrom os.iso $(QEMUFLAGS) -boot d,menu=off -display none -serial 'mon:stdio'

$(KERNEL): $(OBJS)
	@$(LD) $(LDFLAGS) -o $@ $^
	@grub-file --is-x86-multiboot $(KERNEL)
	@grub-file --is-x86-multiboot2 $(KERNEL)

.c.o:
	@$(CC) -MD $(CFLAGS) -o $@ -c $<

.S.o:
	@$(CC) -MD $(ASFLAGS) -o $@ -c $<

%.o: %.asm
	@nasm -f elf -o $@ $^

font.o:
	@$(OBJCOPY) -O elf32-i386 -B i386 -I binary unifont.sfn font.o

kernel.lst: $(KERNEL)
	objdump -D $(KERNEL) > kernel.lst

clean:
	@rm -rf $(KERNEL) kernel.lst kernel.map $(OBJS) *.d lib/string/*.d *~ os.iso iso

.PHONY: all iso qemu-kernel qemu-iso qemu-nox clean

-include $(OBJS:.o=.d)

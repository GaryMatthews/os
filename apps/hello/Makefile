CC=gcc
LD=ld

LDFLAGS += -melf_i386 -T hello.lds -Map hello.map

all:
	@$(CC) -Wall -g -gstabs -Wextra -fno-builtin -nodefaultlibs -nostartfiles -nostdlib -m32 -I ../../include/lib -c main.c
	@$(LD) $(LDFLAGS) -o hello main.o ../../lib/stdio.o ../../lib/string.o ../../lib/system_calls.o ../../lib/stdlib.o

clean:
	@rm -f hello hello.map *.o

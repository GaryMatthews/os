LDFLAGS += -melf_i386 -T example.lds -Map example.map

all:
	@$(CC) -Wall -g -gstabs -Wextra -fno-builtin -nodefaultlibs -nostartfiles -nostdlib -m32 -I ../../include/lib -c main.c
	@$(LD) $(LDFLAGS) -o example main.o ../../lib/stdio.o ../../lib/string.o ../../lib/system_calls.o ../../lib/stdlib.o

clean:
	@rm -f example example.map *.o

CFLAGS += -m32
CFLAGS += -c -I.. -I../include -ffreestanding -fno-builtin -nostdlib
CFLAGS += -Wno-pointer-to-int-cast

all:
	@$(CC) $(CFLAGS) unistd.c
	@$(CC) $(CFLAGS) string.c
	@$(CC) $(CFLAGS) stdio.c
	@$(CC) $(CFLAGS) stdlib.c
	@$(CC) $(CFLAGS) system_calls.c

clean:
	@rm -f *.o

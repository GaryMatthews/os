#include <multiboot.h>
#include <stddef.h>

/** Extract command name from the multiboot module command line.
 *
 * @param buf      Destination buffer (will be always NULL-terminated).
 * @param size     Size of destination buffer (in bytes).
 * @param cmd_line Input string (the command line).
 *
 */
void multiboot_extract_command(char *buf, size_t size, const char *cmd_line) {
    (void)buf;
    (void)size;
    (void)cmd_line;
}

/** Extract arguments from the multiboot module command line.
 *
 * @param buf      Destination buffer (will be always NULL-terminated).
 * @param size     Size of destination buffer (in bytes).
 * @param cmd_line Input string (the command line).
 *
 */
void multiboot_extract_argument(char *buf, size_t size, const char *cmd_line) {
    (void)buf;
    (void)size;
    (void)cmd_line;
}

static void multiboot_modules(uint32_t count, multiboot_module_t *mods) {
    (void)count;
    (void)mods;
}

static void multiboot_memmap(uint32_t length, multiboot_memmap_t *memmap)
{
	uint32_t pos = 0;

	while ((pos < length) && (e820counter < MEMMAP_E820_MAX_RECORDS)) {
		e820table[e820counter] = memmap->mm_info;

		/* Compute address of next structure. */
		uint32_t size = sizeof(memmap->size) + memmap->size;
		memmap = (multiboot_memmap_t *) ((uintptr_t) memmap + size);
		pos += size;

		e820counter++;
	}
}

/** Parse multiboot information structure.
 *
 * If @a signature does not contain a valid multiboot signature,
 * assumes no multiboot information is available.
 *
 * @param signature Should contain the multiboot signature.
 * @param info      Multiboot information structure.
 *
 */
void multiboot_info_parse(const multiboot_info_t *info) {
	/* Copy command line. */
	/*if ((info->flags & MULTIBOOT_INFO_FLAGS_CMDLINE) != 0)
      multiboot_cmdline((char *) MULTIBOOT_PTR(info->cmd_line));*/

	/* Copy modules information. */
	if ((info->flags & MULTIBOOT_INFO_FLAGS_MODS) != 0)
		multiboot_modules(info->mods_count,
		    (multiboot_module_t *) MULTIBOOT_PTR(info->mods_addr));

	/* Copy memory map. */
	if ((info->flags & MULTIBOOT_INFO_FLAGS_MMAP) != 0)
		multiboot_memmap(info->mmap_length,
		    (multiboot_memmap_t *) MULTIBOOT_PTR(info->mmap_addr));
}

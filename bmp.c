#include <bmp.h>
#include <vfs.h>
#include <kheap.h>
#include <printf.h>
#include <lib/string.h>

bmp_image_t* bmp_image_from_file(char* filename) {
    char buff[512];
    struct bmp_image hdr;
    bmp_image_t* bmp;

    file* fd = vfs_file_open(filename,"r");

    vfs_file_read(fd, buff);

    memcpy(&hdr, buff, sizeof(struct bmp_image));

    bmp = (bmp_image_t*)kmalloc(hdr.total_size);

    if( bmp == NULL ) {
      printf("bmp_image_from_file(): out of memory\n");
      return NULL;
    }
    vfs_file_close(fd);
    
    fd = vfs_file_open(filename,"r");
    //XXX
    int j = 0;
    while(fd->eof != 1) {
        char buf[512];
        vfs_file_read(fd, buf);
        memcpy((uint8_t*)((uint32_t)bmp + j), buf, 512);
        j += 512;
    }

    bmp->total_size = hdr.total_size;
    bmp->data = (uint8_t*)((uint32_t)bmp + bmp->offset);
    bmp->width = hdr.width;
    bmp->height = hdr.height;
    bmp->bpp = hdr.bpp;
    bmp->offset = hdr.offset;

    //printf("%s: %dkB %dx%dpx (%dbits), data @ +%d\n", filename, hdr.total_size,
    //       hdr.width, hdr.height, hdr.bpp, hdr.offset);

    return bmp;
}

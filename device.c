#include <device.h>
#include <lib/string.h>
#include <vfs.h>
#include <printf.h>

static device_t *devices[8];

void device_register(device_t *dev) {
    printf("device_register()\n");
    if(dev->id < 8) {
        devices[dev->id] = dev;
        vfs_mount(dev->mount);
    }
    printf("device_register() ok\n");
}

device_t *get_dev_by_name(char *name) {
    if(name[0] == '/')
        name++;
    for(int i = 0; i < 8; i++) {
        if(strncmp(devices[i]->mount, name, 3) == 0)
            return devices[i];
    }
    return NULL;
}

device_t *get_dev_by_id(int id) {
    for(int i = 0; i < 8; i++) {
        if(devices[i]->id == id)
            return devices[i];
    }
    return NULL;
}

int get_dev_id_by_name(char *name) {
    if(name[0] == '/')
        name++;
    for(int i = 0; i < 8; i++) {
        if(strncmp(devices[i]->mount, name, 3) == 0)
            return devices[i]->id;
    }
    return -1;
}


#pragma once

#include <vfs.h>
#include <fat_mount.h>

typedef struct device {
    int id;
    int type; // 0 floppy, 1 ata hdd
    char mount[4];
    char* (*read) (int lba);
    int (*write) (int lba);
    filesystem fs;
    fat_mount_info_t minfo;
} device_t;

void device_register(device_t *dev);
device_t *get_dev_by_name(char *name);
device_t *get_dev_by_id(int id);
int get_dev_id_by_name(char *name);

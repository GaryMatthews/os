#pragma once

#include <types.h>

typedef struct chardev_struct {
    int (*read)(struct chardev_struct *dev, char *buf, size_t nbyte);
    int (*write)(struct chardev_struct *dev, const char *buf, size_t nbyte);
    void *data;
} chardev_t;

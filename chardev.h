#pragma once

#include <stdio.h>

typedef struct chardev_struct {
    ssize_t (*read)(struct chardev_struct *dev, char *buf, size_t nbyte);
    ssize_t (*write)(struct chardev_struct *dev, const char *buf, size_t nbyte);
    void *data;
} chardev_t;

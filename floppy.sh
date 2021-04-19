#!/usr/bin/env bash

LO=`losetup -f`

MNT=/mnt/floppy
IMG=floppy.img

qemu-img create $IMG 1.44M
mkfs.fat $IMG

sudo mkdir -p $MNT
sudo losetup $LO $IMG
sudo mount $LO $MNT -t msdos -o "fat=12"
sudo rm -f $MNT/hello
sudo cp apps/hello/hello $MNT/hello
sudo umount $MNT
sudo losetup -d $LO
sudo rm -rf $MNT

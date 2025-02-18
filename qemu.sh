#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) \
    -cdrom myos.iso\
    --no-reboot\
    -D debug.log\
    -d int\
    -soundhw pcspk\
    -drive file=image.img,format=raw,media=disk\
    -boot menu=on

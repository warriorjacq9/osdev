# Prerequisites:

 - i686 GCC Cross-compiler (The source code is available at https://ftp.gnu.org/gnu/gcc/gcc-14.1.0/)
 - i686 Binutils (Source code at https://ftp.gnu.org/gnu/binutils/) (the latest version is at the bottom of the list)
 - Make

    Available as `make` on most package managers. Used for building the project
 - xorriso

    Available as `xorriso` on most package managers.
    Used for burning to the USB drive
 - Grub 2

    Install `grub2-common` with your package manager.
    Used for creating the boot image
 - Qemu-i386 (optional, for testing)

    Available as `qemu-system-x86` on most package managers.

# Building
To build the cross-comipling tools, follow this tutorial
for the i686-elf target:
https://wiki.osdev.org/GCC_Cross-Compiler

(Note: This takes a while! **Do this first** 
and be prepared to wait).

In the project root folder, run `make build`.

To test the operating system, run `make test`.

### Note:
The operating system requires a hard disk image formatted as FAT16 for testing. You can make one with:

```bash
dd if=/dev/zero of=image.img bs=1M count=32
mkfs.fat -F16 image.img
```

If you plan to run it on your computer, make sure you have a hard drive that doesn't have anything on it and is formatted as FAT16.

# Installation
This project currently runs off of a USB stick. To
 burn to the stick, first run `lsblk` to find your
 stick's device name (mine was `/dev/sdb`). Then,
 run `dd if=myos.iso of=/dev/sdb`. Substitute your
 device name for `/dev/sdb`. Then, eject and remove
 your USB drive, plug it into a computer, and press
 your computer's boot menu button (refer to
 [this table](https://www.disk-image.com/faq-bootmenu.htm)
 to find it) and select "Removable drive" or "USB".
 Enjoy the operating system!
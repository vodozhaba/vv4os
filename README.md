# VV4OS

VV4OS is an open-source non-commercial operating system. The target is an OS with the full abstraction of the internals of the computer, such as files and programs, from the user. The OS has been inspired by macOS.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

* [CMake]
* [GCC Cross-Compiler]
* [GNU GRUB]
* [GNU fdisk]
* [QEMU] or [Bochs]

[CMake]: https://cmake.org/
[GCC Cross-Compiler]: http://wiki.osdev.org/GCC_Cross_Compiler
[GNU GRUB]: http://www.gnu.org/software/grub/
[GNU fdisk]: https://www.gnu.org/software/fdisk/
[QEMU]: https://www.qemu.org/
[Bochs]: http://bochs.sourceforge.net/

### Installing

Create an empty disk image:

```
$ dd if=/dev/zero of=disk.img bs=1048576 count=<size in MiB>
```

Partition it:

```
$ fdisk disk.img

    Welcome to fdisk (util-linux 2.27.1).
    Changes will remain in memory only, until you decide to write them.
    Be careful before using the write command.

    Device does not contain a recognized partition table.
    Created a new DOS disklabel with disk identifier 0xf7e660f9.

    Command (m for help): o
    Created a new DOS disklabel with disk identifier 0x3105fb6d.

    Command (m for help): n
    Partition type
    p   primary (0 primary, 0 extended, 4 free)
    e   extended (container for logical partitions)
    Select (default p): <Enter>

    Using default response p.
    Partition number (1-4, default 1): <Enter>
    First sector (2048-N, default 2048): <Enter>
    Last sector, +sectors or +size{K,M,G,T,P} (2048-N, default N): <Enter>

    Created a new partition 1 of type 'Linux' and of size N MiB.

    Command (m for help): t
    Selected partition 1
    Partition type (type L to list all types): 0B
    Changed type of partition 'Linux' to 'W95 FAT32'.

    Command (m for help): a
    Selected partition 1
    The bootable flag on partition 1 is enabled now.

    Command (m for help): w
    The partition table has been altered.
    Syncing disks.
```

Create a filesystem (on Linux):

```
$ losetup disk.img --show -f -o 1048576 # will print a <device>
$ mkfs.fat -F 32 <device>
$ mount <device> /mnt
```

Create a filesystem (on macOS):

```
$ dd if=disk.img of=mbr.img bs=512 count=2047
$ dd if=disk.img of=fs.img bs=512 skip=2047
$ hdiutil attach -nomount fs.img # will print a <device>
$ newfs_msdos -F 32 <device>
$ hdiutil detach <device>
$ cat mbr.img fs.img > disk.img
$ hdiutil attach disk.img # will print a <mountpoint>
```

Install GRUB:

```
$ grub-install --modules="part_msdos biosdisk fat multiboot configfile" --root-directory="<mountpoint>" ./disk.img
```

Add a following `/boot/grub/grub.cfg` (can be modified in any ways possible):

```
set default=0
set timeout=0
menuentry VV4OS {
    multiboot /sys/vv4os/kernel.elf
    module /sys/vv4os/boot.cfg
    boot
}
```

and a following `/sys/vv4os/boot.cfg` (will be used later, currently just a signature):

```
notjunk
```

The disk image can now be run by your preferred emulator and will correctly boot `/sys/vv4os/kernel.elf`, which should be copied there after building.

## Deployment

Please note the CAPS part in the [license](LICENSE). It basically says that whatever bad happens to real hardware running VV4OS, it's not my fault.

```
$ dd if=disk.img of=/dev/your_usb_drive_and_not_your_hdd
```

Plug the usb drive into a PC which supports booting from USB (and has it set up) and it will probably boot. You can also do that with an internal HDD or similar stuff. Network or optical disc booting will most likely require tweaking the bootloader, however, it is possible, too.

## Authors

* **Vasily Svinko** - *All the work* - [vodozhaba](https://github.com/vodozhaba)

See also the list of [contributors](https://github.com/vodozhaba/vv4os/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details


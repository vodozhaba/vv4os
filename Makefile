# VV4OS Makefile. Here are the main targets:
# I.   all (can be called without specifying the target) - builds release
# version for x86.
# II.  (arch)-debug - builds debug build for (arch). Currently only x86 is
# supported.
# III. (arch)-release - builds release build for (arch).
# IV.  install (run as root) - copies the kernel to a disk image.
# V.   clean - removes the files that are created by this build system.
# and thus can be recovered.
# You can set up the build system by changing the build constants.

# Here come build constants.
# If you can't build VV4OS consider changing them.

# Used tools.

MAKE=make

MOUNT=mount
UMOUNT=umount

# I don't know if there are systems where RM, CP and/or MKDIR commands aren't standard.
# I've added them just in case.

RM=rm
RMFLAGS=-f

CP=cp
CPFLAGS=

MKDIR=mkdir
MKDIRFLAGS=-p

# Compilers (and the linker) and their flags.

ASM=nasm
ASMFLAGS=-felf32

CC=/home/alexander/opt/cross/bin/i686-elf-gcc
CFLAGS=-Wall -Wextra -Werror -ffreestanding -std=c11 -c -I.

LINKER=/home/alexander/opt/cross/bin/i686-elf-gcc
LFLAGS=-ffreestanding -nostdlib -lgcc -T linker.ld

# Files.

SOURCES=$(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c) \
		$(wildcard */*/*/*.c) $(wildcard */*/*/*/*.c) \
		$(wildcard *.asm) $(wildcard */*.asm) $(wildcard */*/*.asm) \
		$(wildcard */*/*/*.asm) $(wildcard */*/*/*/*.asm)

OBJECTS=$(SOURCES:=.o)

KERNEL=kernel.elf

DISK_IMG=disk.img
IMG_OFFSET=1048576 # Where the partition starts in bytes (NOT in sectors)
IMG_MNTDIR=/mnt/

# Destination paths.

KERNEL_DEST_PATH=/sys/vv4os/
KERNEL_DEST_NAME=kernel.elf

# Here comes the build system code.
# Do not modify it unless your IQ level is higher than 110.
# Mine is 122 so I'm allowed to do it.

# It suppresses Makefile built-in rule about .o suffixes.

.SUFFIXES:

# These targets are executed whenever they are called
# even if there are ready files with their names.

.PHONY: all Debug cleanDebug Release cleanRelease $(KERNEL) install clean

# Build everything (currently only the kernel).
# You can simply call make if you want to do it.

all: x86-release

x86-all: CFLAGS += -D__X86__
x86-all: $(KERNEL)

# Installs the kernel to a disk image.

install: $(KERNEL) $(DISK_IMG)
	$(MOUNT) -o loop,offset=$(IMG_OFFSET) $(DISK_IMG) $(IMG_MNTDIR)
	$(MKDIR) $(MKDIRFLAGS) $(IMG_MNTDIR)/$(KERNEL_DEST_PATH)
	$(CP) $(KERNEL) $(IMG_MNTDIR)/$(KERNEL_DEST_PATH)/$(KERNEL_DEST_NAME)
	$(UMOUNT) $(IMG_MNTDIR)

# Only builds the kernel.

$(KERNEL): $(OBJECTS)
	$(LINKER) -o $@ $^ $(LFLAGS)

# Assemblies assembly files with assembler.

%.asm.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Compiles C files with compiler.

%.c.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Removes all the files that are created by make (and thus can be recovered).

clean:
	$(RM) $(RMFLAGS) $(OBJECTS) $(KERNEL)

x86-debug: CFLAGS += -DDEBUG -g
x86-debug: x86-all

x86-release: CFLAGS += -O3 -DRELEASE
x86-release: x86-all

# Stuff for Code::Blocks

cleanDebug: clean
cleanRelease: clean

CC              := x86_64-elf-gcc
AS              := nasm
LD              := x86_64-elf-ld
OBJCOPY         := x86_64-elf-objcopy
GRUB_MKRESCUE   := grub-mkrescue
CFLAGS          := -c $(INCLUDES) -ffreestanding
CFLAGS_DEBUG    := $(CFLAGS) -g -O0 -DDEBUG
CFLAGS_RELEASE  := $(CFLAGS) -O2 -DNDEBUG

ASFLAGS         := -f elf64

LDFLAGS         := -n -T $(TARGETSDIR)/$(ARCH)/linker.ld

ifeq ($(BUILD_TYPE),debug)
    ACTIVE_CFLAGS := $(CFLAGS_DEBUG)
else
    ACTIVE_CFLAGS := $(CFLAGS_RELEASE)
endif
#ifndef KCONFIG_H
#define KCONFIG_H

// Option definitions for GrantOS kernel configuration
#define KERNEL_VERSION "1.0"
#define KERNEL_ARCH "x86_64"
#define KERNEL_DEBUG 1

// Compiler and linker settings
#define CC x86_64 - elf - gcc
#define CFLAGS -c - I src / intf - ffreestanding
#define LDFLAGS -n - o dist / x86_64 / kernel.bin - T targets / x86_64 / linker.ld

// Options for the C++ language standard
#define KERNEL_CFLAGS $(CFLAGS) - I src / impl / kernel
#define KERNEL_LDFLAGS $(LDFLAGS)

// Options for the utility programs
#define MODULE_CFLAGS $(CFLAGS) - I src / impl / utils
#define MODULE_LDFLAGS $(LDFLAGS)

#endif // KCONFIG_H
KERNEL_SOURCES := $(shell find $(KERNEL_SRCDIR) -name '*.c')

UTILS_SOURCES := $(shell find $(UTILS_SRCDIR) -name '*.c')

ARCH_C_SOURCES := $(shell find $(ARCH_SRCDIR) -name '*.c')

ARCH_ASM_SOURCES := $(shell find $(ARCH_SRCDIR) -name '*.asm')

ALL_SOURCES := $(KERNEL_SOURCES) $(UTILS_SOURCES) $(ARCH_C_SOURCES) $(ARCH_ASM_SOURCES)
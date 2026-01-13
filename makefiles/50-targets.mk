KERNEL_BIN      := $(DISTDIR)/$(ARCH)/kernel.bin
KERNEL_ISO      := $(DISTDIR)/$(ARCH)/kernel.iso
ISO_KERNEL_PATH := $(TARGETSDIR)/$(ARCH)/iso/boot/kernel.bin

.PHONY: build-x86_64
build-x86_64: $(ALL_OBJECTS)
	$(call create_dir,$(DISTDIR)/$(ARCH))
	$(LD) $(LDFLAGS) $(ALL_OBJECTS) -o $(KERNEL_BIN)
	cp $(KERNEL_BIN) $(ISO_KERNEL_PATH)
	$(GRUB_MKRESCUE) /usr/lib/grub/i386-pc -o $(KERNEL_ISO) $(TARGETSDIR)/$(ARCH)/iso

.PHONY: kernel build all
kernel: build-x86_64
build: build-x86_64
all: build-x86_64

.PHONY: compile
compile: $(ALL_OBJECTS)
	$(call create_dir,$(DISTDIR)/$(ARCH))
	$(LD) $(LDFLAGS) $(ALL_OBJECTS) -o $(KERNEL_BIN)

.PHONY: iso
iso: compile
	cp $(KERNEL_BIN) $(ISO_KERNEL_PATH)
	$(GRUB_MKRESCUE) /usr/lib/grub/i386-pc -o $(KERNEL_ISO) $(TARGETSDIR)/$(ARCH)/iso
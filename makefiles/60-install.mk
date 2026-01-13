INSTALL_PREFIX  ?= /usr/local
INSTALL_BINDIR  := $(INSTALL_PREFIX)/bin
INSTALL_ISODIR  := $(INSTALL_PREFIX)/share/$(KERNEL_NAME)

.PHONY: install
install: build-x86_64
	@echo "Installing $(KERNEL_NAME)..."
	install -d $(INSTALL_BINDIR)
	install -d $(INSTALL_ISODIR)
	install -m 755 $(KERNEL_BIN) $(INSTALL_BINDIR)/$(KERNEL_NAME).bin
	install -m 644 $(KERNEL_ISO) $(INSTALL_ISODIR)/$(KERNEL_NAME).iso

.PHONY: uninstall
uninstall:
	rm -f $(INSTALL_BINDIR)/$(KERNEL_NAME).bin
	rm -f $(INSTALL_ISODIR)/$(KERNEL_NAME).iso
	rmdir --ignore-fail-on-non-empty $(INSTALL_ISODIR)
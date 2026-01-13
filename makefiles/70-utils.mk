.PHONY: clean
clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILDDIR) $(DISTDIR)

.PHONY: clean-all
clean-all: clean
	@echo "Cleaning everything..."
	rm -f $(ISO_KERNEL_PATH)

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  build-x86_64  - Build the kernel for x86_64 (default)"
	@echo "  kernel        - Alias for build-x86_64"
	@echo "  compile       - Compile kernel without creating ISO"
	@echo "  iso           - Create ISO from compiled kernel"
	@echo "  install       - Install kernel and ISO"
	@echo "  uninstall     - Remove installed files"
	@echo "  clean         - Clean build files"
	@echo "  clean-all     - Clean everything including ISO files"
	@echo "  debug-vars    - Show debug information"
	@echo "  help          - Show this help"
	@echo ""
	@echo "Build types:"
	@echo "  make BUILD_TYPE=debug    - Debug build"
	@echo "  make BUILD_TYPE=release  - Release build (default)"

.PHONY: debug-vars
debug-vars:
	@echo "=== Configuration ==="
	@echo "KERNEL_NAME: $(KERNEL_NAME)"
	@echo "ARCH: $(ARCH)"
	@echo "BUILD_TYPE: $(BUILD_TYPE)"
	@echo "=== Directories ==="
	@echo "SRCDIR: $(SRCDIR)"
	@echo "BUILDDIR: $(BUILDDIR)"
	@echo "DISTDIR: $(DISTDIR)"
	@echo "=== Sources ==="
	@echo "KERNEL_SOURCES: $(words $(KERNEL_SOURCES)) files"
	@echo "UTILS_SOURCES: $(words $(UTILS_SOURCES)) files"
	@echo "ARCH_C_SOURCES: $(words $(ARCH_C_SOURCES)) files"
	@echo "ARCH_ASM_SOURCES: $(words $(ARCH_ASM_SOURCES)) files"
	@echo "=== Objects ==="
	@echo "ALL_OBJECTS: $(words $(ALL_OBJECTS)) files"
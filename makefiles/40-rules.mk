$(KERNEL_OBJECTS): $(KERNEL_BUILDDIR)/%.o : $(KERNEL_SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(ACTIVE_CFLAGS) $< -o $@

$(UTILS_OBJECTS): $(UTILS_BUILDDIR)/%.o : $(UTILS_SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(ACTIVE_CFLAGS) $< -o $@

$(ARCH_C_OBJECTS): $(ARCH_BUILDDIR)/%.o : $(ARCH_SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(ACTIVE_CFLAGS) $< -o $@

$(ARCH_ASM_OBJECTS): $(ARCH_BUILDDIR)/%.o : $(ARCH_SRCDIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

define create_dir
	@mkdir -p $(1)
endef
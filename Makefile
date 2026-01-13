# ================================================================
# Makefile - Kernel x86_64
# ================================================================

# Upload sources
include makefiles/00-config.mk
include makefiles/10-toolchain.mk
include makefiles/20-sources.mk
include makefiles/30-objects.mk
include makefiles/40-rules.mk
include makefiles/50-targets.mk
include makefiles/60-install.mk
include makefiles/70-utils.mk

# Target default
.DEFAULT_GOAL := build-x86_64
KERNEL_NAME     := mykernel
ARCH            := x86_64
BUILD_TYPE      ?= debug

SRCDIR          := src
BUILDDIR        := build
DISTDIR         := dist
TARGETSDIR      := targets

KERNEL_SRCDIR   := $(SRCDIR)/impl/kernel
UTILS_SRCDIR    := $(SRCDIR)/impl/utils
ARCH_SRCDIR     := $(SRCDIR)/impl/$(ARCH)

KERNEL_BUILDDIR := $(BUILDDIR)/kernel
UTILS_BUILDDIR  := $(BUILDDIR)/utils
ARCH_BUILDDIR   := $(BUILDDIR)/$(ARCH)

INCLUDES := -I $(SRCDIR)/intf \
            -I $(KERNEL_SRCDIR)/memory \
            -I $(KERNEL_SRCDIR)/drivers \
            -I $(KERNEL_SRCDIR)/drivers/keyboard \
            -I $(UTILS_SRCDIR)
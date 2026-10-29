# Out-of-tree build of @NAME@ against a configured kernel build folder.
#   make KDIR=<kernel build dir> [ARCH=riscv CROSS_COMPILE=riscv64-linux-]   (default KDIR: the running kernel)
#   klinux run .        build against the qemu-riscv64 kernel and boot-test test.cmds (from the 32_05_linux ID)
# Kernels >= 6.13 put the output in $(OUT) (MO=); older ones build in this folder.
KDIR ?= /usr/lib/modules/$(shell uname -r)/build
OUT ?= $(if $(KLINUX_BUILD),$(KLINUX_BUILD)/modules/@NAME@,$(CURDIR)/build)
# A kernel built with clang (e.g. some distribution kernels) needs LLVM=1 for its modules too.
ifneq ($(shell grep -s '^CONFIG_CC_IS_CLANG=y' $(KDIR)/.config),)
export LLVM := 1
endif

all:
	mkdir -p $(OUT)
	$(MAKE) -C $(KDIR) M=$(CURDIR) MO=$(OUT) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) MO=$(OUT) clean

.PHONY: all clean

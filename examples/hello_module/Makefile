# Out-of-tree module build. Output goes to $(OUT) via MO= (kernel >= 6.13), never into this folder.
#   make                              host kernel (/usr/lib/modules/$(uname -r)/build)
#   make BOARD=k230-mainline          cross, against a configured kbuild board output (source env.sh first)
#   make clean
BOX_OUT ?= $(BOX_V3)
KLINUX_BUILD ?= $(if $(BOX_OUT),$(BOX_OUT)/30_39_tools/32_embedded/32_05_linux/build,$(abspath ../../build))
BOARD ?=
ifeq ($(BOARD),)
KDIR ?= /usr/lib/modules/$(shell uname -r)/build
OUT  ?= $(KLINUX_BUILD)/examples/hello_module/host
else
include ../../boards/$(BOARD)/board.conf
ARCH := $(strip $(ARCH))
CROSS_COMPILE := $(strip $(CROSS_COMPILE))
export ARCH CROSS_COMPILE
KDIR ?= $(KLINUX_BUILD)/$(BOARD)
OUT  ?= $(KLINUX_BUILD)/examples/hello_module/$(BOARD)
endif
# A kernel built with clang (e.g. the CachyOS host kernel) needs LLVM=1 for its modules too.
ifneq ($(shell grep -s '^CONFIG_CC_IS_CLANG=y' $(KDIR)/.config),)
export LLVM := 1
endif

all:
	mkdir -p $(OUT)
	$(MAKE) -C $(KDIR) M=$(CURDIR) MO=$(OUT) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) MO=$(OUT) clean

.PHONY: all clean

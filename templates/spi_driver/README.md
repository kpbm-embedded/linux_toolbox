# @NAME@

SPI device driver skeleton (spi_driver, DT match, one register read helper); the test only loads and unloads it (no SPI controller in the test VM).

On a board: a child node of the SPI controller with `compatible = "vendor,@NAME_DASH@"; reg = <0>; spi-max-frequency = <1000000>;`. Not probed in the test VM (QEMU virt has no SPI controller).

```sh
klinux run .                                     # build for qemu-riscv64, boot it, run test.cmds (32_05_linux)
make KDIR=$KLINUX_BUILD/<board> ARCH=<arch> CROSS_COMPILE=<prefix>   # any configured kernel (kbuild <board> prepare)
```

`test.cmds` runs in the test VM: insmod, rmmod, write, cat, expect, exists, missing, gt, klog, fail, sleep
(see `examples/_qemu/boxinit.c` in 32_05_linux).

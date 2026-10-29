# @NAME@

I2C client driver (SMBus register access, sysfs attribute); the test uses the kernel's i2c-stub adapter.

On a board: `compatible = "vendor,@NAME_DASH@"; reg = <0x50>;` under the I2C controller node.

```sh
klinux run .                                     # build for qemu-riscv64, boot it, run test.cmds (32_05_linux)
make KDIR=$KLINUX_BUILD/<board> ARCH=<arch> CROSS_COMPILE=<prefix>   # any configured kernel (kbuild <board> prepare)
```

`test.cmds` runs in the test VM: insmod, rmmod, write, cat, expect, exists, missing, gt, klog, fail, sleep
(see `examples/_qemu/boxinit.c` in 32_05_linux).

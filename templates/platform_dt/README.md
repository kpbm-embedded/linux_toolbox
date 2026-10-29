# @NAME@

platform driver bound from the device tree (compatible "vendor,@NAME_DASH@"), with a property and a sysfs attribute.

`__name__.dtso` (here `@NAME@.dtso`) is the overlay the test boot applies; on a board, apply it in U-Boot or add the node to the board DT.

```sh
klinux run .                                     # build for qemu-riscv64, boot it, run test.cmds (32_05_linux)
make KDIR=$KLINUX_BUILD/<board> ARCH=<arch> CROSS_COMPILE=<prefix>   # any configured kernel (kbuild <board> prepare)
```

`test.cmds` runs in the test VM: insmod, rmmod, write, cat, expect, exists, missing, gt, klog, fail, sleep
(see `examples/_qemu/boxinit.c` in 32_05_linux).

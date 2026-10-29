# @NAME@

platform driver using GPIO lines from the DT (gpiod API): an output "enable" set at probe, sysfs to toggle it.


```sh
klinux run .                                     # build for qemu-riscv64, boot it, run test.cmds (32_05_linux)
make KDIR=$KLINUX_BUILD/<board> ARCH=<arch> CROSS_COMPILE=<prefix>   # any configured kernel (kbuild <board> prepare)
```

`test.cmds` runs in the test VM: insmod, rmmod, write, cat, expect, exists, missing, gt, klog, fail, sleep
(see `examples/_qemu/boxinit.c` in 32_05_linux).

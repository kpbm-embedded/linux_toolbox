# initramfs_boot

the test harness itself: boots the qemu-riscv64 kernel with an initramfs whose /init (examples/_qemu/boxinit.c, built with the kernel's nolibc) runs test.cmds. No module.

test.cmds commands: see the header of `../_qemu/boxinit.c` (insmod, rmmod, write, cat, expect, exists, missing, gt, klog, fail, sleep).

```sh
klinux examples test initramfs_boot      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build initramfs_boot     # build only (also: BOARD=<board> for another kernel)
```

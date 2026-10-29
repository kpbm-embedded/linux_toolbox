# sysfs_attr

kobject /sys/kernel/box_sysfs with a read-write attribute "value" and a read-only "doubled".


```sh
klinux examples test sysfs_attr      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build sysfs_attr     # build only (also: BOARD=<board> for another kernel)
```

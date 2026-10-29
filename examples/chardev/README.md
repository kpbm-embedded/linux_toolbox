# chardev

character device (cdev + class) with a 4 KiB buffer: what you write you read back, /dev/boxchar0.


```sh
klinux examples test chardev      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build chardev     # build only (also: BOARD=<board> for another kernel)
```

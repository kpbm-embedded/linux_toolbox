# misc_device

misc device /dev/boxmisc: each read returns "reads=N", writing "reset" sets N to 0, anything else is -EINVAL.


```sh
klinux examples test misc_device      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build misc_device     # build only (also: BOARD=<board> for another kernel)
```

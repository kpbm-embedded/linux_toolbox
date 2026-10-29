# debugfs

debugfs directory /sys/kernel/debug/box_debugfs: a u32 "counter" and a seq_file "stats".


```sh
klinux examples test debugfs      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build debugfs     # build only (also: BOARD=<board> for another kernel)
```

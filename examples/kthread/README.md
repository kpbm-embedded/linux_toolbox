# kthread

kernel thread that wakes every period_ms and counts ticks (read-only parameter "ticks"); stopped cleanly on unload.


```sh
klinux examples test kthread      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build kthread     # build only (also: BOARD=<board> for another kernel)
```

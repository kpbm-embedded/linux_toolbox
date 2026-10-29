# timer_workqueue

timer (softirq context) that queues work on its own workqueue (process context) every 50 ms; counter in the "works" parameter.


```sh
klinux examples test timer_workqueue      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build timer_workqueue     # build only (also: BOARD=<board> for another kernel)
```

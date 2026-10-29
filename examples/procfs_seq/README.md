# procfs_seq

/proc/box_seq through the seq_file iterator (start/next/stop/show): one line per item, "lines" module parameter.


```sh
klinux examples test procfs_seq      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build procfs_seq     # build only (also: BOARD=<board> for another kernel)
```

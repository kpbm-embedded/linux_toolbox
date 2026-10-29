# examples

Each folder with a `test.cmds` is tested by `klinux examples test [name ...]`: its module (`Kbuild`) is built against
the `qemu-riscv64` kernel, packed into an initramfs with `_qemu/boxinit.c` as `/init`, its `*.dtso` (and the files
listed in `overlays`) merged into the QEMU virt DT, in-tree modules listed in `kmods` added under `/k/`, and QEMU boots
it; `/init` runs `test.cmds` and prints `BOXTEST: PASS`. Output: `$KLINUX_BUILD/examples/<name>/` (`boot.log`).

| Folder | What | test.cmds checks |
|---|---|---|
| `hello_module/` | module parameter + platform driver for `box,hello` (overlay from `dt_overlay/`); also `make` for host/board | probe via overlay, parameter, unload |
| `dt_overlay/` | overlay adding a `box,hello` node; `./test.sh` applies it to the virt DT or a board DTB | (booted with hello_module) |
| `chardev/` | cdev + class, `/dev/boxchar0` | write, read back, node removed |
| `misc_device/` | misc device, read counter, write command | counter, reset, -EINVAL |
| `sysfs_attr/` | kobject attributes in `/sys/kernel/box_sysfs` | rw value, ro doubled |
| `debugfs/` | debugfs u32 + seq_file | write counter, read stats |
| `procfs_seq/` | `/proc/box_seq` seq_file iterator | lines=3 |
| `kthread/` | kernel thread with stop | ticks grow, clean stop |
| `timer_workqueue/` | timer -> workqueue | works grow, clean teardown |
| `platform_dt/` | DT-bound platform driver, properties, two nodes | both probes, sysfs |
| `gpio_consumer/` | gpiod in/out from DT, gpio-sim bank | input pull, output level |
| `i2c_client/` | I2C client on the i2c-stub adapter | new_device, SMBus round trip |
| `leds_overlay/` | gpio-leds via DT only | LED class, brightness |
| `hwmon_sensor/` | hwmon chip on a platform device | temp1_* attributes |
| `initramfs_boot/` | the harness itself | /proc, debugfs |
| `_qemu/boxinit.c` | the test `/init` (nolibc) | |

Build only, for another configured kernel: `BOARD=<board> klinux examples build [name ...]` (after `kbuild <board> config`
and `kbuild <board> prepare`).

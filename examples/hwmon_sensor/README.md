# hwmon_sensor

hwmon driver (hwmon_device_register_with_info) for a fake temperature sensor on a platform device created by the module itself: temp1_input, temp1_max (rw), temp1_label.


```sh
klinux examples test hwmon_sensor      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build hwmon_sensor     # build only (also: BOARD=<board> for another kernel)
```

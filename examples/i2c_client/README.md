# i2c_client

I2C client driver (SMBus byte/word access, sysfs attributes) tested against the kernel's i2c-stub fake adapter; the device is created at run time with new_device.

`kmods` lists in-tree modules the test boot needs (i2c-stub from the kernel build). On a board, describe the chip under its I2C controller in the DT (`compatible = "box,i2c-demo"; reg = <0x48>;`).

```sh
klinux examples test i2c_client      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build i2c_client     # build only (also: BOARD=<board> for another kernel)
```

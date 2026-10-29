# platform_dt

platform driver bound from the device tree (compatible "box,sensor"): reads string, u32-array and enum properties, one device per DT node, a sysfs attribute per device.

`box-sensor.dtso` adds two nodes; on a board, apply it with U-Boot `fdt apply` or put the nodes in the board DT. Binding:
`compatible = "box,sensor"`, `label` (string, required), `box,channels` (1-8 u32, required), `box,gain` ("low"|"mid"|"high", default mid).

```sh
klinux examples test platform_dt      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build platform_dt     # build only (also: BOARD=<board> for another kernel)
```

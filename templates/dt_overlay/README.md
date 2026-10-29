# @NAME@

Device-tree overlay `@NAME@.dtso`.

```sh
klinux run .                                                  # merged onto the QEMU virt DT, booted, test.cmds checks /proc/device-tree
dtc -@ -I dts -O dtb -o @NAME@.dtbo @NAME@.dtso               # for a board
fdtoverlay -i board.dtb -o merged.dtb @NAME@.dtbo             # check that it applies
```

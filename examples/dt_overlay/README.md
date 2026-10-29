# dt_overlay: device-tree overlay example

`hello-overlay.dtso` adds a `hello` node (`compatible = "box,hello"`, matched by `../hello_module/`) at `/`.
`test.sh` compiles it and applies it to the QEMU virt DT (default) or a board DTB from `$KLINUX_BUILD`, then checks the node is in the result. The boot test with `../hello_module` is `klinux examples test hello_module`.

```sh
source ../../env.sh
./test.sh                                          # default base: the QEMU riscv64 virt DT
./test.sh <board> <dtb path under arch/*/boot/dts/>
```

Output: `$KLINUX_BUILD/examples/dt_overlay/` (`hello-overlay.dtbo`, `merged.dtb`). On a board, load the
`.dtbo` from U-Boot (`fdt apply`) or put it where the bootloader expects overlays.

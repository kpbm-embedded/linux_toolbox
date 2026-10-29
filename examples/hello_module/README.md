# hello_module: out-of-tree kernel module example

`hello.c` logs on load/unload (`greeting=` parameter) and registers a platform driver for DT nodes with
`compatible = "box,hello"` (see `../dt_overlay/`). Copy this folder to start a new module.

```sh
source ../../env.sh
make -j2                          # host kernel -> $KLINUX_BUILD/examples/hello_module/host/hello.ko
make -j2 BOARD=k230-mainline      # cross, needs `kbuild k230-mainline config` + `build` done first
modinfo $KLINUX_BUILD/examples/hello_module/host/hello.ko
```

Build output never lands here (`MO=`, kernel 6.13+). Loading (`insmod`) needs root and is left to you.

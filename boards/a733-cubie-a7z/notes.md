# A733 notes (Radxa Cubie A7Z, 1GB and 8GB)

- Source: Radxa's BSP as packaged by radxa-pkg/linux-a733 @ 166c84e4: `src` = radxa/kernel `allwinner-aiot-linux-5.15` @ e4149989 (5.15.147),
  `bsp` = radxa/allwinner-bsp @ 2045a3ca, `device-a733` = radxa/allwinner-device @ 6d911d15, all under `upstream/linux-a733/`.
- `scripts/a733-assemble` applies the packaging's debian/patches and the pre_build glue from `.github/local/Makefile.local` (bsp symlink, `bsp.config`, board DT links, dtsi + dt-bindings copies, ramfs, autogen header). Run it once after fetching; it leaves `src/.box-a733-assembled`.
- Config = `defconfig` (which is `bsp.config` via `KBUILD_DEFCONFIG`) + `radxa.config` + `radxa_custom.config` + `kernel.fragment`. The fragment turns off the upstream IMX214/IMX219 drivers, which clash by module name with the BSP's VIN sensor drivers (`modules_check` error).
- Built 2026-10-24: Image 23.4 MB, A7Z/A7A/A7S DTBs, 28 compiler warnings from vendor code (GCC 14.3; Radxa uses Debian's cross GCC).
- Radxa also has `allwinner-aiot-linux-6.6` and `configs/cubie_a7z/linux-6.6/board.dts`, but its packaging still builds 5.15; mainline (v7.3) has no A733 (sun60i) support.
- Boot (not tried here): RadxaOS images ship this kernel as a .deb; to test, replace the kernel + DTB in a RadxaOS SD image.

# RK3506 notes (Luckfox Lyra B, EBYTE ECK41 ×2, IDO-SOM3506)

- Tree: `upstream/rockchip-linux-kernel`, rockchip-linux/kernel `develop-6.1` @ 77168c8d (6.1.172), shallow, plus one local commit on branch `box/luckfox-lyra` (6dc4ec06):
  the Luckfox board DTs (`rk3506-luckfox-lyra.dtsi`, `rk3506g-luckfox-lyra{,-sd,-plus,-plus-sd}.dts`) and `rk3506_luckfox_defconfig`, taken from the Luckfox Lyra SDK kernel
  (unofficial mirror github.com/0xd61/luckfox-linux-6.1-rk3506 @ e0846929; copies kept in `cache/downloads/luckfox-lyra-0xd61/`). Only DT/defconfig came from the mirror, no code.
- The SoC dtsi is Rockchip's (newer than Luckfox's: e.g. CAN compatible `rockchip,rk3506-can`). A few Luckfox-only symbols in the defconfig (BACKLIGHT_WAVESHARE, CH182H2_LED_TRIGGER_PHY, DRM_IGNORE_IOTCL_PERMIT) don't exist here and are dropped.
- Built 2026-10-24: zImage 6.9 MB, 5 DTBs, 0 warnings (GCC 14.3).
- ECK41: EBYTE publishes no DT; `rk3506g-evb1-v10.dtb` is built as the starting point. Ask EBYTE for their SDK, or derive one from the Lyra/EVB DTs and the ECK41 schematic.
- Mainline: v7.3 has `clk-rk3506`, `rst-rk3506` and RK3506 pinctrl/GMAC/SARADC/SPI bits but no `rk3506.dtsi`, so a mainline DT is the missing piece.
- Boot (not tried here): Luckfox's SDK packs zImage + DTB into a FIT `boot.img` for Rockchip U-Boot; the base DT boots a UBI rootfs on SPI NAND, the `-sd` DTs boot `/dev/mmcblk0p3`.

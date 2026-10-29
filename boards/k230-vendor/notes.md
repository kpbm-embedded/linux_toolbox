# K230 notes

Boards: Canaan CanMV-K230 (1GB/8GB, as ordered; check the silkscreen version: the vendor tree also has `k230-canmv-01studio` and the U-Boot overlay a `k230_canmv_v2` config) and Banana Pi BPI-CanMV-K230D-Zero (128MB).

## Kernels

| Entry | Tree | Defconfig | DTBs | State |
|---|---|---|---|---|
| `k230-vendor` | ruyisdk/linux-xuantie-kernel @ ae4a7e65 (6.6.36), same as kendryte/k230_linux_sdk `main` | `k230_defconfig` | k230-canmv, bananapi-canmv-k230d-zero, k230-canmv-01studio, k230d-canmv, k230-evb | builds (2026-10-24, 103 s) |
| `k230-mainline` | linux-7.3 | `defconfig` + `kernel.fragment` (K230 clk/pinctrl/reset are off in defconfig) | k230-canmv, k230-evb | builds (2026-10-24, 152 s); UART only, no SD/MMC/Ethernet in the DT |

v6.18 and v6.12 LTS have no K230 DT or drivers, so there is no LTS entry for this SoC.
Toolchain: kernel.org crosstool GCC 14.3 (`riscv64-linux-`). The SDK uses the T-Head Xuantie GCC (V2.10.1, gated download at xrvm.cn), which the kernel does not need.

## Boot flow (from the SDK config, not yet tried here)

The SDK builds U-Boot 2022.10 and OpenSBI 1.4 with the kernel as the OpenSBI payload (`BR2_TARGET_OPENSBI_LINUX_PAYLOAD=y`) and writes `sysimage-sdcard.img`.
To try a kernel from here: flash an SDK SD image, then swap in `Image` + the DTB (or rebuild the SDK with `LINUX_OVERRIDE_SRCDIR=upstream/linux-xuantie-kernel` in its `local.mk`).
The mainline DT declares 512MB of RAM; check what the bootloader passes on the 1GB board.

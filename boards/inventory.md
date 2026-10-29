# Linux-capable boards owned

Source: the AliExpress order history in `$BOX/50_59_projects/52_parts/52_04_alix_lets` (`aliexpress_orders_full.csv`), checked 2026-10-24.
"Mainline DT" = a board or SoC device tree in the trees under `upstream/` (v7.3, v6.18.58 LTS, v6.12.114 LTS), found by filename grep; a SoC-only entry means you'd write the board DT yourself.

| Board (as ordered) | SoC / arch | v6.12 LTS | v6.18 LTS | v7.3 | Vendor kernel (K230 checked; others unverified) | Board entry |
|---|---|---|---|---|---|---|
| Canaan CanMV-K230 1GB/8GB (2 bought) | K230, 2× C908 RISC-V (riscv) | — | — | `canaan/k230-canmv` (UART only) | ruyisdk/linux-xuantie-kernel 6.6.36 | `k230-vendor`, `k230-mainline` |
| Banana Pi BPI-CanMV-K230D-Zero | K230D, 128MB (riscv) | — | — | — | same, `canaan/bananapi-canmv-k230d-zero` | `k230-vendor` |
| LicheeRV Nano (B), 4 orders | SG2002 (riscv) | — | `sophgo/sg2002-licheerv-nano-b` | same | Sipeed SDK | — |
| Milk-V Duo (64MB) | CV1800B (riscv) | `sophgo/cv1800b-milkv-duo` | same | same | Milk-V SDK | — |
| Milk-V Duo Module 01 | SG2000 (riscv) | — | `sophgo/sg2000-milkv-duo-module-01-evb` | same | Milk-V SDK | — |
| Sipeed M1s / M1s Dock | BL808 (riscv) | — | — | — | openbouffalo/linux bl808/all (6.4-rc3) | `bl808-sipeed-m1s` |
| NanoPi NEO | H3 (arm) | `allwinner/sun8i-h3-nanopi-neo` | same | same | — | — |
| NanoPi Duo2 | H3 (arm) | `allwinner/sun8i-h3-nanopi-duo2` | same | same | — | — |
| Lichee Pi Zero | V3s (arm) | `allwinner/sun8i-v3s-licheepi-zero` | same | same | — | — |
| T113-S3/T113-i boards: EBYTE ECK31 (×2), COJXU ECK30, Forlinx FET113i-S, "Quanzhi T113-S3" board, loose T113-S3 chips | T113 (arm) | SoC (`sun8i-t113s.dtsi`) | SoC | SoC | Allwinner/Tina 5.4 | — |
| MangoPi MCore-H616, H616/H618 core boards, H618 1G+8G board | H616/H618 (arm64) | SoC + similar boards (orangepi-zero2/3) | same | same | — | — |
| BigTreeTech CB1 (with M8P V2) | H616 (arm64) | `allwinner/sun50i-h616-bigtreetech-cb1-manta` | same | same | BTT | — |
| Radxa Cubie A7Z (1GB, 8GB) | A733 (arm64) | — | — | — | Radxa BSP 5.15.147 (radxa-pkg/linux-a733) | `a733-cubie-a7z` |
| Luckfox Lyra B, EBYTE ECK41 (×2), IDO-SOM3506 | RK3506 (arm) | — | — | drivers only (clk/rst/pinctrl), no DT | rockchip-linux/kernel develop-6.1 (6.1.172) + Luckfox DTs | `rk3506-luckfox-lyra` |
| Luckfox Pico / Pico Mini (B) (several) | RV1103 (arm) | — | — | SoC (`rv1103b.dtsi`, only omega4 EVB) | Luckfox SDK 5.10 | — |
| RK3566 board (4GB/32GB) | RK3566 (arm64) | SoC + many boards | same | same | Rockchip | — |
| Liontron FI-3568 | RK3568 (arm64) | SoC + many boards | same | same | Rockchip | — |
| STM32MP13 COJXU ECK10 (512MB) | STM32MP131/135 (arm) | SoC + `st/stm32mp135f-dk` | same | same | ST 6.6 | — |
| Radxa Dragon Q6A 8GB | QCS6490 (arm64) | — | — | `qcom/qcs6490-radxa-dragon-q6a` | Radxa BSP | — |
| Dongshanpi-Picom | SSD210 (arm) | SSD202D boards only (`sigmastar/mstar-infinity2m-*`) | same | same | SigmaStar 4.9 | — |
| SZBOX EX4 mini PC | Intel N150 (x86_64) | generic x86 | same | same | — | — |

Not Linux targets (left out): RP2350/RP2040 boards, ESP32-P4, K210 boards, Tang Primer 25K (FPGA soft core), the STM32 flight controllers, the SSC338/378 IP camera module (SigmaStar vendor firmware), Radxa AICore / Maix4-HAT (accelerators).

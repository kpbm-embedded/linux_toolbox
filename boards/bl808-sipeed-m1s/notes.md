# BL808 notes (Sipeed M1s / M1s Dock)

- Tree: `upstream/openbouffalo-linux`, openbouffalo/linux `bl808/all` @ ce94420c (Linux 6.4-rc3, 2023-05; the branch has not moved since). Nothing for BL808 in mainline v7.3 or linux-next.
- `bl808_defconfig`, DTs `bouffalolab/bl808-sipeed-m1s` and `bl808-pine64-ox64`. Built 2026-10-24: Image 15.5 MB, 0 warnings (GCC 14.3).
- Linux runs on the D0 core (T-Head C906, rv64) with the 64MB PSRAM.
- Boot (not tried here): OpenBouffalo's flow loads low-load firmware on the M0 and D0 cores, then OpenSBI + U-Boot (or a direct payload) on D0. Their prebuilt images and loaders come from openbouffalo/buildroot_bouffalo; flash with the Bouffalo flasher over the M1s Dock's USB.

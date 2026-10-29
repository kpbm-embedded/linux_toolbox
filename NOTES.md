# 32_05_linux notes

## 2026_10_24
- Bootstrapped: layout, env.sh, scripts/kbuild (dry-run tested), boards/_template. No kernel trees or toolchains downloaded yet.
- Wrap-up: `KLINUX_BUILD` now defaults to `$BOX_OUT/30_39_tools/32_embedded/32_05_linux/build` (idisk03 on this host, same ID path; overridable). The local `build/` folder is unused. README no longer names other IDs (self-containment grep). `kbuild _template build --dry-run` checked: it prints `O=$BOX_OUT/...`.

## 2026_10_24 (K230)
- Toolchain: kernel.org crosstool GCC 14.3.0 riscv64 (sha256 checked against sha256sums.asc) in `toolchain/gcc-14.3.0-riscv64-linux`.
- Trees: `upstream/linux-stable.git` (bare, depth 1) with worktrees linux-7.3, linux-6.18.58, linux-6.12.114 (the latest mainline and two newest LTS per kernel.org releases.json); `upstream/linux-xuantie-kernel` at ae4a7e65 (6.6.36, the commit kendryte/k230_linux_sdk pins).
- K230 has no support in either LTS; mainline v7.3 has DTs for CanMV/EVB with clk/pinctrl/reset/UART only, and `defconfig` leaves the clk/pinctrl drivers off (enabled in `boards/k230-mainline/kernel.fragment`).
- Built `k230-vendor` (Image 18.7 MB + 5 DTBs, 103 s) and `k230-mainline` (Image 27.6 MB + 2 DTBs, 152 s). First vendor attempt failed: host lacked `bc` (installed by the user).
- QEMU smoke boot not run: qemu-system-riscv64 11.1.2 fails to init io_uring (ENOMEM).
- Board inventory from the AliExpress orders in `boards/inventory.md`.

## 2026_10_24 (examples)
- Added `boards/qemu-riscv64/` (example board: `kbuild qemu-riscv64 config` run for real, every fragment option is in the .config; `SOC_VIRT` is `ARCH_VIRT` in 6.18), `examples/hello_module/` (out-of-tree platform-driver module, built with `MO=` so no output lands in the source: host 7.2.10-cachyos OK with auto `LLVM=1` because that kernel is clang-built; cross for k230-mainline OK, RISC-V .ko) and `examples/dt_overlay/` (`test.sh`: dtc -@ + fdtoverlay onto k230-canmv.dtb, PASS). Not loaded or booted (insmod needs root; QEMU io_uring issue).

## 2026_10_24 (RK3506, A733, BL808)
- None of the three has a DT in mainline v7.3 or linux-next (v7.3 has RK3506 clk/rst/pinctrl drivers only), so each gets its vendor/community kernel.
- Toolchains: kernel.org crosstool GCC 14.3 aarch64-linux and arm-linux-gnueabi (sha256 checked).
- RK3506: rockchip-linux/kernel develop-6.1 @ 77168c8d + local commit 6dc4ec06 on `box/luckfox-lyra` (Luckfox Lyra DTs + rk3506_luckfox_defconfig from the 0xd61 mirror of the Luckfox SDK). Built: zImage + 5 DTBs, 104 s.
- A733: Radxa BSP pinned by radxa-pkg/linux-a733 (5.15.147); `scripts/a733-assemble` reproduces its patches + glue. First build failed at modules_check (upstream IMX214/IMX219 vs BSP sensor modules); disabled the upstream ones in the fragment. Built: Image + 3 DTBs.
- BL808: openbouffalo/linux bl808/all (6.4-rc3). Built: Image + 2 DTBs, 82 s.
- kbuild: `MAKE_ARGS` (optional) and multi-target `DEFCONFIG`.
- None of these kernels has been booted on hardware yet.
- 2026_10_25 readability pass (report: 00_09_meta/02_logs/2026_10_25_readability_embedded_core_report.md): kbuild --help (actions, boards, env), unknown action -> exit 2 with message, FRAGMENTS/DTBS optional under set -u; env.sh KLINUX_BUILD default via $BOX_V3. Dry-run command lines unchanged for all boards.
- 2026_10_25 readability pass 2 (report: 00_09_meta/02_logs/2026_10_25_readability2_embedded_core_report.md): kbuild options go anywhere (-n/--dry-run, -y/--yes, -h); `clean` is a dry run unless --yes; unknown options or extra arguments exit 2 (a third argument other than --dry-run was silently ignored); the action is validated before the board checks; --help lists only real boards (was `ls boards/`, incl. inventory.md and _template). Descriptor: --yes, apply_flag clean. README updated.

## 2026-10-29
- Build output from 2026-10-24 was gone (no `$KLINUX_BUILD` on idisk03). Rebuilt only `qemu-riscv64` (6.18.58 defconfig + fragment, 3 min 31 s with 8 jobs).
- `boards/qemu-riscv64/kernel.fragment`: initramfs, module unload, debugfs, configfs, gpio-sim, i2c-stub (m), LEDs (gpio, heartbeat), hwmon, SPI for the test boots.
- Test VM: `examples/_qemu/boxinit.c` is a static `/init` built with the kernel's `tools/include/nolibc` and the kernel.org compiler (no libc needed). It mounts proc/sys/devtmpfs/debugfs/configfs, runs `test.cmds` (insmod via finit_module, rmmod, write, expect, exists, missing, gt, klog, fail, sleep) and powers off. Boot to PASS: about 1.5 s under TCG. The io_uring ENOMEM of 2026-10-24 did not show up (same QEMU 11.1.2).
- 13 new examples: chardev, misc_device, sysfs_attr, debugfs, procfs_seq, kthread, timer_workqueue, platform_dt, gpio_consumer, i2c_client, leds_overlay, hwmon_sensor, initramfs_boot; hello_module got a test.cmds (booted with the dt_overlay node). DT overlays are merged onto `qemu-system-riscv64 -machine dumpdtb` output with fdtoverlay (target-path, no __symbols__ needed); gpio-sim banks stand in for GPIO controllers, i2c-stub for an I2C adapter. Found: gpio-sim lines can sleep, so leds-gpio sets them from a workqueue: the LED test waits 100 ms before reading the line. Result: 14/14 PASS.
- 13 templates (`templates/`, `klinux new`): module, chardev, misc, platform_dt, i2c_driver, spi_driver, gpio_consumer, sysfs, debugfs, kthread, procfs, dt_overlay, board. Version guards for class_create (< 6.4) and platform .remove (< 6.11). `tests/test_templates.sh`: 30/30 (12 instantiated + booted, board config dry run, template Makefile cross build, refusals).
- `scripts/klinux` (setup, update --check, doctor, examples, run, templates, new, test) and `sources.lock`: compilers by url + sha256 (cache renamed `gcc.tar.xz` -> `x86_64-gcc-14.3.0-nolibc-riscv64-linux.tar.xz`), stable worktrees by tag + commit, vendor trees by commit. `scripts/rk3506-luckfox-files` recreates the local Luckfox commit from 6 sha256-pinned files of the 0xd61 mirror + `boards/rk3506-luckfox-lyra/dts-makefile.patch`; same git tree hash as the original commit (e277e70a).
- kbuild: `prepare` action (modules_prepare), board as a folder path, `KLINUX_JOBS` (default 8; was nproc = 32).
- update --check on 2026-10-29: 6.18.59 out (pinned 6.18.58), crosstool GCC 16.2.0 out (pinned 14.3.0), linux-xuantie-kernel and allwinner-device heads moved; pins unchanged.
- Smoke 31/31, doctor 30 ok + 1 warning (ccache). Git repo prepared (`.gitignore`: upstream/, toolchain/, cache/, build/ are not committed).
- Addendum (Rust, Zig, BEAM): Zig yes: `examples/zig_userspace` + template `zig_userspace` (static riscv64-linux-musl program, raw syscalls, run in the test VM by the new boxinit `run` command; klinux builds `*.zig` with the Zig 0.17.0 of 32_24_microzig, reached via KLINUX_ZIG / BOX_MICROZIG / sibling ID, exit 3 = skip without it). Rust for Linux: not added: riscv HAVE_RUST needs a clang-built kernel, and 32_17_embedded_rust (rustc 1.99.1, profile minimal) has no rust-src and no bindgen; `klinux doctor` reports this. Third language corrected to Bend (not BEAM): nothing added, 32_31_bend does not exist yet and Bend's C output needs a hosted POSIX libc (only a riscv64/aarch64 userspace program would fit, untested); no BEAM items were added. Results after the addendum: examples 15/15, templates 32/32, smoke 31/31, doctor 31 ok + 2 warnings.

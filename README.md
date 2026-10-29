# 32_05_linux - embedded Linux kernels

Builds Linux kernels (and device trees) for embedded boards, out of tree, one `board.conf` per board, with cross compilers kept in
this folder. Use it when you need a kernel and DTBs for K230, RK3506 (Luckfox Lyra), Cubie A7Z, BL808 or a QEMU target, outside Yocto.

Status: **ready** (2026-10-29): 5 board kernels built earlier; the `qemu-riscv64` kernel built and booted in QEMU, where
16 examples and 14 templates are tested (modules loaded with insmod, DT overlays applied, a Zig program run); smoke
31/31, doctor 31 ok + 2 warnings (no ccache, no Rust for Linux).
No kernel has been booted on a real board yet (see Limitations). Self-contained: `scripts/klinux setup` fetches the
pinned compilers (sha256) and kernel trees (commits) listed in `sources.lock`; build output goes to `$KLINUX_BUILD`.

## What's included

| Component | Version | Notes |
|---|---|---|
| Cross compilers | kernel.org crosstool GCC 14.3.0 | riscv64 (`riscv64-linux-`), aarch64 (`aarch64-linux-`), arm (`arm-linux-gnueabi-`); no libc |
| Mainline kernels | v7.3, 6.18.58 LTS, 6.12.114 LTS | `upstream/linux-stable.git` (bare, shallow) with one worktree each |
| Vendor kernels | linux-xuantie-kernel 6.6.36 (K230), rockchip-linux develop-6.1 (+Luckfox DTs), Radxa BSP 5.15.147 (A733), openbouffalo 6.4-rc3 (BL808) | `upstream/` |
| Boards | `k230-vendor`, `k230-mainline`, `rk3506-luckfox-lyra`, `a733-cubie-a7z`, `bl808-sipeed-m1s`, `qemu-riscv64` | `boards/<board>/`, each has a `notes.md`; owned-board inventory: `boards/inventory.md` |
| Examples | 15: out-of-tree modules (char/misc device, sysfs, debugfs, procfs, kthread, timer + workqueue, platform driver with DT binding, GPIO consumer, I2C client, hwmon) and DT overlays | `examples/`, tested in QEMU |
| Examples | + `zig_userspace`: static Zig program for the board, built with the Zig 0.17 of 32_24_microzig (reached, not installed) | `examples/zig_userspace` |
| Templates | 14 kinds: module, chardev, misc, platform_dt, i2c_driver, spi_driver, gpio_consumer, sysfs, debugfs, kthread, procfs, dt_overlay, board, zig_userspace | `templates/`, `klinux new <kind> <name>` |
| Test VM | QEMU riscv64 `virt` + initramfs whose `/init` is `examples/_qemu/boxinit.c`, built with the kernel's own nolibc | `klinux examples test`, `klinux run <dir>` |

## Quick start

```sh
cd $BOX/30_39_tools/32_embedded/32_05_linux
scripts/klinux setup && scripts/klinux doctor   # fresh clone: compilers + kernel trees at their pins (~5 GB)
source env.sh                               # bash or zsh; puts kbuild, klinux and the cross compilers on PATH
kbuild qemu-riscv64 config && kbuild qemu-riscv64 build   # the test kernel (about 4 min with 8 jobs)
klinux examples test                        # 16 examples, each booted in QEMU (about 1 min)
klinux new chardev mydev --dir ~/work && klinux run ~/work/mydev
kbuild k230-mainline config --dry-run       # print the commands only
kbuild k230-mainline config                 # defconfig + fragments merged
kbuild k230-mainline build                  # kernel image -> $KLINUX_BUILD/k230-mainline/
kbuild k230-mainline dtbs
```

## Commands

`kbuild [-n|--dry-run] <board> <action> [-y|--yes]` (after `source env.sh`, or `box run embedded_linux ...` from anywhere).
Options go anywhere: `--dry-run` prints the make commands instead of running them; `clean` is a dry run unless `--yes`.

| Action | What it does | Example |
|---|---|---|
| `config` | DEFCONFIG, then merge the board's FRAGMENTS and `olddefconfig` | `kbuild k230-mainline config` |
| `menuconfig` | interactive kernel configuration | `kbuild k230-mainline menuconfig` |
| `build` | kernel image (make default target) | `kbuild k230-mainline build` |
| `dtbs` | the board's DTBS (default: all dtbs) | `kbuild k230-mainline dtbs` |
| `modules` | kernel modules | `kbuild k230-mainline modules` |
| `prepare` | `modules_prepare` only: enough to build out-of-tree modules | `kbuild qemu-riscv64 prepare` |
| `savedefconfig` | write a minimal defconfig into the build dir | `kbuild k230-mainline savedefconfig` |
| `clean` | `make clean` in the build dir; without `--yes` it only prints the command | `kbuild k230-mainline clean --yes` |

An unknown action or option, or a missing/extra argument, exits 2 with a message. `kbuild --help` lists the boards
(`boards/*/board.conf`, without `_template`); a board can also be a folder path with a `board.conf` (e.g. one made with
`klinux new board`). Parallel jobs: `KLINUX_JOBS` (default 8, the machine is shared).

`klinux [--dry-run] <command>` (also `scripts/klinux` without sourcing anything). Exit codes: 0 ok, 1 failure, 2 usage
error, 3 not ready (the qemu-riscv64 kernel is not built).

| Command | What it does | Example |
|---|---|---|
| `setup [name ...]` | fetch the pinned compilers (sha256, cached in `cache/downloads/`) and kernel trees (exact commits, shallow), then the local steps (`rk3506-luckfox-files`, `a733-assemble`); existing ones are kept | `klinux setup linux-6.18.58 gcc-14.3.0-riscv64-linux` |
| `update --check` | pins vs upstream: newest kernel.org releases, vendor branch heads, newest crosstool GCC | `klinux update --check` |
| `doctor` | compilers, trees at their pins, host tools, build dir, test kernel | `klinux doctor` |
| `examples list\|build\|test [ex ...]` | build the example modules against `qemu-riscv64` (`BOARD=<board>` for another kernel), boot them in QEMU and run each `test.cmds` | `klinux examples test chardev` |
| `run <dir>` | the same for any folder with `Kbuild` / `test.cmds` / `*.dtso` | `klinux run ~/work/mydev` |
| `templates`, `new <kind> <name> [--dir D]` | list / copy a template (`__name__`, `@NAME@`, `@NAME_DASH@` replaced); `board` goes to `boards/` | `klinux new i2c_driver tmp117 --dir ~/work` |
| `test [--all]` | `tests/test_smoke.sh`; `--all` adds `examples test` and `tests/test_templates.sh` | `klinux test --all` | `scripts/a733-assemble [--dry-run]` turns `upstream/linux-a733/{src,bsp,device-a733}` into a buildable tree; run it once before `kbuild a733-cubie-a7z`.

## Examples and templates

`klinux examples test` builds each module against the `qemu-riscv64` kernel, packs it with `examples/_qemu/boxinit.c`
(a static `/init` built with the kernel's `tools/include/nolibc`) into an initramfs, merges the example's `*.dtso` into
the QEMU virt DT (`-machine dumpdtb` + `fdtoverlay`) and boots it; `/init` runs `test.cmds` (insmod, write, expect,
klog, ...) and prints `BOXTEST: PASS`. Results and boot logs: `$KLINUX_BUILD/examples/<name>/`.

| Example | Shows | Tested in QEMU |
|---|---|---|
| `hello_module` + `dt_overlay` | module parameter, platform driver bound by an overlay node | insmod, probe message, parameter in sysfs, rmmod |
| `chardev` | cdev + class, `/dev/boxchar0` via devtmpfs, read/write buffer | write then read back, node removed on rmmod |
| `misc_device` | misc device, per-open read, command write, -EINVAL | read counter, reset, bad command fails |
| `sysfs_attr` | kobject in `/sys/kernel`, rw + ro attributes | write 21, read doubled 42, bad input fails |
| `debugfs` | `debugfs_create_u32`, seq_file via `DEFINE_SHOW_ATTRIBUTE` | write counter, read stats |
| `procfs_seq` | seq_file iterator (start/next/stop/show), module parameter | line count follows `lines=3` |
| `kthread` | kthread_run / kthread_stop, read-only counter parameter | ticks > 5 after 300 ms, clean stop |
| `timer_workqueue` | timer_list (softirq) handing work to a workqueue (process context) | works > 2, clean teardown |
| `platform_dt` | DT binding: string, u32 array, enum property, two instances, dev_groups | both probes, sysfs label/gain |
| `gpio_consumer` | gpiod output/input from DT on a gpio-sim bank | pull-up seen on input, output drives the sim line |
| `i2c_client` | i2c_driver, SMBus access, new_device/delete_device | against `i2c-stub` (in-tree module), register round trip |
| `leds_overlay` | DT only: gpio-leds + heartbeat trigger | LED class device, brightness drives the line |
| `hwmon_sensor` | hwmon_device_register_with_info, platform device from code | temp1_input/label/max, read-only input |
| `initramfs_boot` | the test harness itself | /proc/version, debugfs, cpuinfo |
| `zig_userspace` | Zig userspace program (std.os.linux syscalls), static `riscv64-linux-musl` | `run /bin/zig_hello`, its line in the kernel log |

`dt_overlay/test.sh` also applies the overlay to the QEMU virt DT or a board DTB (`./test.sh k230-mainline canaan/k230-canmv.dtb`).

Templates (`klinux new <kind> <name> [--dir D]`; each has a `test.cmds`, a `Makefile` for any kernel build dir and a README):

| Kind | Gives you | Template test |
|---|---|---|
| `module` | parameter, init/exit | boot: insmod with parameter, change it in sysfs |
| `chardev` | cdev + class (`class_create` for < 6.4 and >= 6.4) | boot: write/read `/dev/<name>0` |
| `misc` | misc device with read/write/ioctl | boot: write/read, bad input fails |
| `platform_dt` | platform driver, DT match, property, sysfs, `.remove` for < 6.11 and >= 6.11 | boot with its overlay |
| `i2c_driver` | i2c_driver, SMBus register attribute | boot: i2c-stub, new_device, register round trip |
| `spi_driver` | spi_driver with DT + id table, register read | boot: load/unload only (no SPI controller in QEMU virt) |
| `gpio_consumer` | gpiod output from DT, sysfs toggle | boot with a gpio-sim overlay |
| `sysfs` / `debugfs` / `procfs` | attribute files | boot: write/read |
| `kthread` | periodic kernel thread | boot: loop counter, clean stop |
| `dt_overlay` | overlay with target-path | boot: node in /proc/device-tree |
| `board` | board.conf + kernel.fragment + notes.md | `kbuild <folder> config --dry-run` |
| `zig_userspace` | Zig program for the board's ARCH (musl, static) | boot: run it, kernel log line (skipped without Zig) |

Languages: **Zig** yes (userspace programs above; Zig from 32_24_microzig via `$KLINUX_ZIG`, `$BOX_MICROZIG` or the
sibling ID, else the example is skipped with exit 3). **Rust**: no Rust-for-Linux module yet: 6.18 needs rustc +
rust-src + bindgen and, on riscv, a clang-built kernel (`HAVE_RUST if CC_IS_CLANG`); 32_17_embedded_rust has rustc
1.99.1 without rust-src or bindgen (`klinux doctor` reports it). **BEAM** (AtomVM/Erlang): not supported here, it is
board-rootfs userspace (Yocto or 32_29_atomvm), not a kernel concern.

Board-only parts (flashing, real GPIO/I2C/SPI wiring, U-Boot `fdt apply`) are not tested here; the QEMU test uses
gpio-sim and i2c-stub instead. The template `Makefile`s build against any configured kernel (`kbuild <board> prepare`),
so the same code builds for the vendor trees (6.1, 6.6) — only built and booted for 6.18 so far.

More:

- New board: `klinux new board <board>` (or `cp -r boards/_template boards/<board>`), edit `board.conf` (ARCH, cross prefix, kernel tree + ref, defconfig, FRAGMENTS, DTBS) and `kernel.fragment`. `board.conf` may also set `MAKE_ARGS` and list several `DEFCONFIG` targets.
- One kernel tree serves every board, because each board builds into its own `$KLINUX_BUILD/<board>/`.
- `boards/qemu-riscv64/`: example board (QEMU virt, 6.18 LTS). `kbuild qemu-riscv64 config`.
- `examples/hello_module/`: also builds against the host kernel (`make -j2`) or a board (`make -j2 BOARD=k230-mainline`).
- Update a kernel: `klinux update --check` shows newer releases; add a `stable linux-6.18.59 v6.18.59 <commit>` line to
  `sources.lock`, `klinux setup linux-6.18.59`, then point the board's `KERNEL_TREE`/`KERNEL_REF` at it.

## Configuration and environment variables

| Variable | Default | Meaning |
|---|---|---|
| `KLINUX_BUILD` | `$BOX_OUT/<this ID path>/build` (`<ID>/build` standalone or with `BOX_INTEGRATION=0`) | build output base (set before sourcing `env.sh` to override) |
| `KLINUX_JOBS` | 8 | parallel make jobs for kbuild and the example builds |
| `KLINUX_QEMU_BOARD`, `QEMU_TIMEOUT` | qemu-riscv64, 120 | board and per-boot timeout of the example tests |
| `KLINUX_ROOT`, `KLINUX_UPSTREAM`, `KLINUX_TOOLCHAIN` | set by `env.sh` | this ID, kernel trees, compilers |
| `CCACHE_DIR` | `cache/` | ccache directory; `kbuild` uses ccache automatically once it is installed (it is not yet) |

## Where files go

| What | Where |
|---|---|
| Source (in git) | `boards/`, `scripts/`, `examples/`, `templates/`, `tests/`, `sources.lock`, `env.sh` |
| Kernel trees | `upstream/` (`linux-stable.git` + worktrees `linux-7.3`, `linux-6.18.58`, `linux-6.12.114`; vendor forks in their own folders) |
| Compilers | `toolchain/<gcc-14.3.0-...>/bin` |
| Cache | `cache/` (ccache, compiler tarballs, Luckfox files; regenerable) |
| Build output | `$KLINUX_BUILD/<board>/` (`O=`, `.config`, `Image`/`zImage`, DTBs, modules); examples and their boot logs in `$KLINUX_BUILD/examples/`; this host: idisk03, same ID path |
| Logs | `$KLINUX_BUILD/<board>/kbuild.log` |

## Native mode

No native file exists for this ID, so `box native enable` does not apply. Per shell: `source $BOX_EMBEDDED_LINUX/env.sh`
(`$BOX_EMBEDDED_LINUX` comes from `box shell install --apply`; see Quick start). It sets the `KLINUX_*` variables and puts
`toolchain/*/bin` and `scripts/` on `PATH`. Without sourcing anything, `box run embedded_linux <board> <action>` runs `kbuild`.

## Tests

`klinux test --all` (2026-10-29): smoke 31/31 (help, usage errors, dry runs of every board, lock syntax, doctor),
examples 15/15 test.cmds boots (16 examples: `dt_overlay` is booted with `hello_module`), templates 32/32 (14 kinds
instantiated; 13 built and booted, board dry-run configured; template Makefile cross-built). `klinux doctor`: 31 ok,
2 warnings (ccache not installed, Rust for Linux prerequisites missing). Board kernels: 5 built on 2026-10-24 (k230-vendor 103 s, k230-mainline 152 s,
rk3506-luckfox-lyra 104 s, a733-cubie-a7z, bl808-sipeed-m1s 82 s); their build output was not kept, rebuild with kbuild.

## Troubleshooting

| Symptom | Fix |
|---|---|
| Vendor build fails: `bc` not found | install host packages `bc flex bison openssl libelf dtc pahole` (all present now) |
| A733 `modules_check` fails (IMX214/IMX219 vs BSP sensor modules) | the upstream sensor modules are disabled in the board fragment; run `scripts/a733-assemble` first |
| K230 mainline: no clk/pinctrl | `defconfig` leaves them off; enabled in `boards/k230-mainline/kernel.fragment` |
| QEMU: `Failed to initialize io_uring: Cannot allocate memory` | seen on 2026-10-24 with qemu-system-riscv64 11.1.2; not seen on 2026-10-29 (same QEMU) |
| `insmod` on the host needs root | use `klinux run` / `klinux examples test`: modules are loaded inside the QEMU test VM |
| `klinux examples test` exits 3 | the qemu-riscv64 kernel is not built: `kbuild qemu-riscv64 config && kbuild qemu-riscv64 build` |
| An example FAILs | its boot log: `$KLINUX_BUILD/examples/<name>/boot.log` (the failing test.cmds line follows `BOXTEST: FAIL at:`) |

## Limitations / needs you

- No kernel has been booted on a real board (no K230 SD image swap tested); only `qemu-riscv64` boots, in QEMU.
- The examples and templates are built and tested only on 6.18 (riscv64); vendor trees (6.1, 6.6, 5.15) have version guards for the known API changes but were not built with them.
- Mainline v7.3 has no DT for RK3506, A733 or BL808, so those use vendor/community trees.
- Next steps: boot the K230 kernels; board entries for NanoPi NEO, LicheeRV Nano, Milk-V Duo, STM32MP13; `kbuild <board> install` (staging folder); install ccache.
- Open items: `$BOX/00_09_meta/02_logs/2026_10_25_open_items.md`.

## Layout

| Folder | Holds |
|---|---|
| `boards/<board>/` | `board.conf` (ARCH, cross prefix, kernel tree + ref, defconfig, fragments, DTBs) and config fragments. `_template/` is the starting point |
| `scripts/kbuild` | the build driver; `scripts/klinux` setup/doctor/examples/templates; `scripts/a733-assemble`, `scripts/rk3506-luckfox-files` prepare vendor trees |
| `examples/`, `templates/`, `tests/` | examples (each with `test.cmds`), templates for `klinux new`, smoke + template tests |
| `sources.lock` | pinned compilers (url + sha256), kernel tags/commits, vendor tree commits |
| `upstream/` | kernel trees, shallow (not in git; `klinux setup`) |
| `toolchain/` | cross compilers, one folder each with a `bin/` |
| `cache/` | ccache and downloads; regenerable (`CACHEDIR.TAG`) |
| `env.sh` | sets `KLINUX_*`, `CCACHE_DIR`, PATH |

## Scripts

- `env.sh` - `source env.sh` (bash or zsh): `KLINUX_*` paths, this ID's cross compilers and `scripts/` on PATH.
- `scripts/kbuild` - configure/build a kernel for `boards/<board>/board.conf` out of tree; `--dry-run` prints the make commands,
  `clean` needs `--yes`; exit codes 0 ok, 1 failure, 2 usage error. Described in `.box.toml` (`[scripts.kbuild]`).
- `scripts/klinux` - setup (pinned sources), update --check, doctor, examples, run, templates/new, test.
- `scripts/a733-assemble` - turn `upstream/linux-a733/{src,bsp,device-a733}` into a buildable tree (`--dry-run` supported).
- `scripts/rk3506-luckfox-files` - add the Luckfox Lyra DTs + defconfig (sha256-pinned downloads) as a commit on `box/luckfox-lyra`.
- `tests/test_smoke.sh`, `tests/test_templates.sh` - run by `klinux test [--all]`.

## Links

- `boards/inventory.md`, `boards/<board>/notes.md`, `examples/README.md`, [NOTES.md](NOTES.md)
- Reports in `$BOX/00_09_meta/02_logs/`: `2026_10_25_readability_embedded_core_report.md`, `2026_10_25_open_items.md`

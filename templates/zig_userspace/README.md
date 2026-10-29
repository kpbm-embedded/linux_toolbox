# @NAME@

Zig userspace program, cross-compiled static for the board (`riscv64-linux-musl`, `aarch64-linux-musl`,
`arm-linux-musleabihf`). The Zig compiler is the one of 32_24_microzig (`$KLINUX_ZIG` / `$BOX_MICROZIG` / sibling ID).

```sh
klinux run .                      # built into /bin/@NAME@ of the test initramfs, run by test.cmds in QEMU
zig build-exe @NAME@.zig -target aarch64-linux-musl -O ReleaseSmall   # for an arm64 board
```

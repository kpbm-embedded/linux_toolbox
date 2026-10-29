# zig_userspace

Linux userspace program in Zig (`zig_hello.zig`), cross-compiled static for the board (`riscv64-linux-musl` for
qemu-riscv64; the target follows the board's ARCH) with the Zig 0.17 compiler of 32_24_microzig, which is reached,
not installed here (`$KLINUX_ZIG`, else `$BOX_MICROZIG`, else the sibling ID; without it the example is skipped).
It uses raw syscalls from `std.os.linux`: reads the kernel release and writes a line to `/dev/kmsg`. klinux puts it
in the test initramfs as `/bin/zig_hello`; `test.cmds` runs it and checks the kernel log.

```sh
klinux examples test zig_userspace
```

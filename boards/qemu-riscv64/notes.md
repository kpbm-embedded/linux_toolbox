# qemu-riscv64 (example board)

Reference board for `kbuild` and `examples/`: no hardware, QEMU builds the DT itself.

```sh
source env.sh
kbuild qemu-riscv64 config            # defconfig + kernel.fragment -> $KLINUX_BUILD/qemu-riscv64/.config
kbuild qemu-riscv64 build --dry-run   # print the make command (a real build takes a few minutes)
# boot (once built):
qemu-system-riscv64 -M virt -nographic -kernel $KLINUX_BUILD/qemu-riscv64/arch/riscv/boot/Image -append console=ttyS0
```

QEMU boot not verified on this host (io_uring ENOMEM, see README next steps).

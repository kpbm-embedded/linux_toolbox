#!/usr/bin/env bash
# Compile hello-overlay.dtso and apply it to a base DTB; fail unless the node shows up.
#   ./test.sh                 base: the QEMU riscv64 virt DT (qemu-system-riscv64 -machine dumpdtb)
#   ./test.sh <board> <dtb>   base: a DTB from a kbuild board build, e.g. k230-mainline canaan/k230-canmv.dtb
# The boot test of this overlay with ../hello_module is `klinux examples test hello_module`.
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd -P)"
# KLINUX_BUILD: from the environment, else the ID's env.sh (its path under the Box `out` location)
# shellcheck source=../../env.sh
[ -n "${KLINUX_BUILD:-}" ] || . "$here/../../env.sh"
out="$KLINUX_BUILD/examples/dt_overlay"; mkdir -p "$out"
if [[ $# -eq 0 ]]; then
  board=qemu-virt dtb=virt.dtb base="$out/virt.dtb"
  qemu-system-riscv64 -M virt -m 512 -smp 2 -nographic -machine dumpdtb="$base" >/dev/null 2>&1
else
  board=$1 dtb=${2:?give the dtb path under arch/*/boot/dts/}
  base=$(find "$KLINUX_BUILD/$board/arch" -path "*/boot/dts/$dtb" -print -quit)
fi
[[ -n "$base" && -f "$base" ]] || { echo "base DTB not found: $board $dtb (run kbuild $board dtbs)" >&2; exit 1; }
dtc -@ -I dts -O dtb -o "$out/hello-overlay.dtbo" "$here/hello-overlay.dtso"
fdtoverlay -i "$base" -o "$out/merged.dtb" "$out/hello-overlay.dtbo"
dtc -I dtb -O dts "$out/merged.dtb" 2>/dev/null | grep -A3 'hello {' | tee "$out/hello-node.txt"
grep -q 'box,hello' "$out/hello-node.txt" && echo "PASS overlay applied to $board $dtb"

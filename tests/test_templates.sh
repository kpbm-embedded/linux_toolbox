#!/usr/bin/env bash
# Template round trip: every templates/<kind> is instantiated with `klinux new` into a temp folder, then
#   module kinds + dt_overlay: built against the qemu-riscv64 kernel and boot-tested (klinux run: test.cmds in QEMU)
#   module: also its own Makefile (make KDIR=... cross)      board: kbuild <folder> config --dry-run
# Needs the qemu-riscv64 kernel built (kbuild qemu-riscv64 config && kbuild qemu-riscv64 build); exit 3 if it is not.
set -euo pipefail
here=$(cd "$(dirname "$0")/.." && pwd -P)
kl="$here/scripts/klinux"
# shellcheck source=../env.sh
source "$here/env.sh"
[[ -f "$KLINUX_BUILD/qemu-riscv64/arch/riscv/boot/Image" ]] || { echo "templates: qemu-riscv64 kernel not built (kbuild qemu-riscv64 build)"; exit 3; }
tmp=$(mktemp -d "$KLINUX_BUILD/tpl-test.XXXXXX")
pass=0 fail=0
t() { local name=$1; shift; if "$@" >"$tmp/$name.log" 2>&1; then pass=$((pass + 1)); echo "ok   $name"; else fail=$((fail + 1)); echo "FAIL $name (log $tmp/$name.log)"; fi; }
for d in "$here"/templates/*/; do
  kind=$(basename "$d")
  case "$kind" in
    board)
      t "new-$kind" "$kl" new board zz-board --dir "$tmp"
      t "dry-config-$kind" bash -c '"$1/scripts/kbuild" "$2/zz-board" config --dry-run | grep -q "merge_config.sh"' _ "$here" "$tmp" ;;
    *)
      t "new-$kind" "$kl" new "$kind" "zz_$kind" --dir "$tmp"
      t "run-$kind" "$kl" run "$tmp/zz_$kind" ;;
  esac
done
# the standalone Makefile of a module template, cross-built against the same kernel
t make-module make -C "$tmp/zz_module" KDIR="$KLINUX_BUILD/qemu-riscv64" ARCH=riscv CROSS_COMPILE=riscv64-linux- OUT="$tmp/zz_module_out"
t make-module-ko test -f "$tmp/zz_module_out/zz_module.ko"
# refusals: an existing target and a bad name
t new-existing-refused bash -c '! "$1" new module zz_module --dir "$2"' _ "$kl" "$tmp"
t new-bad-name-exits-2 bash -c '"$1" new module Bad-Name --dir "$2"; [ $? -eq 2 ]' _ "$kl" "$tmp"
echo "templates: $pass ok, $fail failed (temp files in $tmp)"
[ "$fail" -eq 0 ]

#!/usr/bin/env bash
# Smoke test of kbuild and klinux: help, usage errors (exit 2), dry runs for every board, template listing,
# sources.lock syntax, and (when the trees and compilers are there) doctor and one module build.
set -euo pipefail
here=$(cd "$(dirname "$0")/.." && pwd -P)
kb="$here/scripts/kbuild" kl="$here/scripts/klinux"
pass=0 fail=0
t() { local name=$1; shift; if "$@" >/dev/null 2>&1; then pass=$((pass + 1)); else fail=$((fail + 1)); echo "FAIL $name"; fi; }
t kbuild-help "$kb" --help
t klinux-help "$kl" --help
t kbuild-no-args-exits-2 bash -c '"$1"; [ $? -eq 2 ]' _ "$kb"
t kbuild-bad-action-exits-2 bash -c '"$1" qemu-riscv64 explode; [ $? -eq 2 ]' _ "$kb"
t kbuild-bad-option-exits-2 bash -c '"$1" qemu-riscv64 build --bogus; [ $? -eq 2 ]' _ "$kb"
t kbuild-unknown-board-exits-1 bash -c '"$1" no-such-board build --dry-run; [ $? -eq 1 ]' _ "$kb"
t klinux-unknown-exits-2 bash -c '"$1" explode; [ $? -eq 2 ]' _ "$kl"
t klinux-update-needs-check bash -c '"$1" update; [ $? -eq 2 ]' _ "$kl"
t klinux-new-unknown-kind bash -c '"$1" new nokind x; [ $? -eq 2 ]' _ "$kl"
for conf in "$here"/boards/*/board.conf; do
  b=$(basename "$(dirname "$conf")")
  [[ "$b" == _* ]] && continue
  t "dry-config-$b" bash -c '"$1" "$2" config --dry-run | grep -q "^+ make"' _ "$kb" "$b"
  t "dry-build-$b" bash -c '"$1" "$2" build --dry-run | grep -q "^+ make"' _ "$kb" "$b"
done
t clean-is-dry-without-yes bash -c '"$1" qemu-riscv64 clean | grep -q "dry run"' _ "$kb"
t prepare-dry bash -c '"$1" qemu-riscv64 prepare --dry-run | grep -q modules_prepare' _ "$kb"
t templates-listed bash -c '[ "$("$1" templates | wc -l)" -ge 10 ]' _ "$kl"
t new-dry-run bash -c '"$1" --dry-run new module x --dir /nonexistent | grep -q copy' _ "$kl"
t setup-dry-run bash -c '"$1" --dry-run setup gcc-14.3.0-riscv64-linux | grep -q "toolchain"' _ "$kl"
t examples-listed bash -c '[ "$("$1" examples list | wc -l)" -ge 10 ]' _ "$kl"
t lock-syntax bash -c 'grep -v "^\s*\(#\|$\)" "$1/sources.lock" | awk '\''$1!~/^(toolchain|stable|git|post)$/{exit 1}'\''' _ "$here"
t shellcheck-free-syntax bash -n "$kb"
t shellcheck-free-syntax-kl bash -n "$kl"
if [[ -d "$here/upstream/linux-6.18.58" && -d "$here/toolchain/gcc-14.3.0-riscv64-linux" ]]; then
  t doctor "$kl" doctor
fi
echo "smoke: $pass ok, $fail failed"
[ "$fail" -eq 0 ]

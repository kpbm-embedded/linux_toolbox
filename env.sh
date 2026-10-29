# shellcheck shell=bash
# Embedded Linux kernel workspace: source it (bash or zsh). Self-contained: uses only this ID's folders.
# shellcheck disable=SC2296  # zsh: ${(%):-%x} is the sourced file
if [ -n "${BASH_SOURCE:-}" ]; then _kl="${BASH_SOURCE[0]}"; else _kl="${(%):-%x}"; fi
KLINUX_ROOT="$(cd "$(dirname "$_kl")" && pwd -P)"; export KLINUX_ROOT; unset _kl
export KLINUX_UPSTREAM="$KLINUX_ROOT/upstream"     # kernel trees (one per vendor/fork), shallow clones
export KLINUX_TOOLCHAIN="$KLINUX_ROOT/toolchain"   # cross compilers, one folder each (bin/ inside)
_klinux_box_out_of() { # DIR -> DIR's path under the Box `out` location, or nothing (standalone / outside Box)
  # BOX: the environment, else the folder holding .boxroot above DIR (older trees: the one holding box_env.sh).
  # Base: $BOX_OUT_<NN_NN> (the ID's own [paths] out), else $BOX_OUT, else $BOX_V3 (old name); missing values come
  # from the generated box_env.d/locations.sh. BOX_INTEGRATION=0 forces standalone.
  local dir="$1" box="${BOX:-}" kit=00_09_meta/00_system/00_01_box_env rel id base idv idval="" sub
  [ "${BOX_INTEGRATION:-1}" != 0 ] || return 0
  while [ -z "$box" ] && [ "$dir" != / ] && [ -n "$dir" ]; do
    dir="$(dirname "$dir")"
    if [ -e "$dir/.boxroot" ] || [ -r "$dir/$kit/box_env.sh" ]; then box="$dir"; fi
  done
  [ -n "$box" ] && [ -d "$box" ] || return 0
  box="$(cd -P "$box" && pwd -P)"
  rel="${1#"$box"/}"
  [ "$rel" != "$1" ] || return 0
  id="${rel#*/}"; id="${id#*/}"; id="${id%%/*}"; idv="BOX_OUT_$(printf %.5s "$id")"
  base="${BOX_OUT:-${BOX_V3:-}}"
  eval "idval=\"\${$idv:-}\""
  if { [ -z "$base" ] || [ -z "$idval" ]; } && [ -r "$box/$kit/box_env.d/locations.sh" ]; then
    # shellcheck disable=SC1090,SC1091  # generated per host
    base="${base:-$(BOX="$box"; . "$box/$kit/box_env.d/locations.sh" >/dev/null 2>&1; printf %s "${BOX_OUT:-${BOX_V3:-}}")}"
    # shellcheck disable=SC1090,SC1091
    [ -n "$idval" ] || idval="$(BOX="$box"; . "$box/$kit/box_env.d/locations.sh" >/dev/null 2>&1; eval "printf %s \"\${$idv:-}\"")"
  elif [ -z "$base" ] && [ -r "$box/$kit/box_env.sh" ]; then        # a tree from before locations.sh
    # shellcheck disable=SC2016  # expanded by the child shell
    base="$(BOX="$box" bash -c '. "$1" >/dev/null 2>&1; printf %s "${BOX_V3:-}"' _ "$box/$kit/box_env.sh")"
  fi
  case "$id" in [0-9][0-9]_[0-9][0-9]_*) ;; *) idval="" ;; esac
  if [ -n "$idval" ]; then                                         # the ID's own out (+ DIR's path inside the ID)
    sub="${rel#*/}"; sub="${sub#*/}"
    case "$sub" in */*) sub="${sub#*/}" ;; *) sub="" ;; esac
    printf '%s\n' "$idval${sub:+/$sub}"; return 0
  fi
  if [ -n "$base" ] && [ -d "$base" ]; then printf '%s\n' "$base/$rel"; fi
}
# Out-of-tree builds (<board>/): $KLINUX_BUILD, else this ID's path under the Box `out` location + /build
# (_klinux_box_out_of), else KLINUX_ROOT/build. BOX_INTEGRATION=0 forces standalone (KLINUX_ROOT/build).
_klinux_default_build() {
  local o
  o="$(_klinux_box_out_of "$KLINUX_ROOT")"
  if [ -n "$o" ]; then printf '%s\n' "$o/build"; else printf '%s\n' "$KLINUX_ROOT/build"; fi
}
KLINUX_BUILD="${KLINUX_BUILD:-$(_klinux_default_build)}"; export KLINUX_BUILD; unset -f _klinux_default_build
export CCACHE_DIR="$KLINUX_ROOT/cache/ccache"
while IFS= read -r _tc; do PATH="$_tc:$PATH"; done < <(find "$KLINUX_TOOLCHAIN" -mindepth 2 -maxdepth 2 -type d -name bin 2>/dev/null); unset _tc
export PATH="$KLINUX_ROOT/scripts:$PATH"

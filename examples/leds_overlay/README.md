# leds_overlay

device-tree only, no module: a gpio-leds node on a simulated GPIO line; the in-kernel leds-gpio driver creates /sys/class/leds/box:green with the heartbeat trigger.

Needs CONFIG_LEDS_GPIO and CONFIG_LEDS_TRIGGER_HEARTBEAT (in `boards/qemu-riscv64/kernel.fragment`). On a board, use the real GPIO controller instead of the gpio-sim bank.

```sh
klinux examples test leds_overlay      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build leds_overlay     # build only (also: BOARD=<board> for another kernel)
```

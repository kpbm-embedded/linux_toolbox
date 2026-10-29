# gpio_consumer

GPIO consumer (gpiod API) on lines from the DT: an output "led" and an input "button", both on a simulated GPIO bank (gpio-sim) described in the overlay; sysfs attributes to drive and read them.

On a board, point `led-gpios` / `button-gpios` at a real controller (e.g. `<&gpio0 RK_PA3 GPIO_ACTIVE_HIGH>`) and drop the gpio-sim node.

```sh
klinux examples test gpio_consumer      # build against qemu-riscv64, boot it in QEMU, run test.cmds
klinux examples build gpio_consumer     # build only (also: BOARD=<board> for another kernel)
```

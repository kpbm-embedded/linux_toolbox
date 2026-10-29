// SPDX-License-Identifier: GPL-2.0
/*
 * box_gpio: GPIO consumer (gpiod API) on lines from the DT: an output "led" and an input "button", both on a simulated GPIO bank (gpio-sim) described in the overlay; sysfs attributes to drive and read them.
 */
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>

struct box_gpio {
	struct gpio_desc *led;
	struct gpio_desc *button;
};

static ssize_t led_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct box_gpio *g = dev_get_drvdata(dev);
	bool on;
	int ret = kstrtobool(buf, &on);

	if (ret)
		return ret;
	gpiod_set_value_cansleep(g->led, on);
	return count;
}

static ssize_t led_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct box_gpio *g = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%d\n", gpiod_get_value_cansleep(g->led));
}
static DEVICE_ATTR_RW(led);

static ssize_t button_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct box_gpio *g = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%d\n", gpiod_get_value_cansleep(g->button));
}
static DEVICE_ATTR_RO(button);

static struct attribute *box_attrs[] = {
	&dev_attr_led.attr,
	&dev_attr_button.attr,
	NULL,
};
ATTRIBUTE_GROUPS(box);

static int box_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct box_gpio *g;

	g = devm_kzalloc(dev, sizeof(*g), GFP_KERNEL);
	if (!g)
		return -ENOMEM;
	/* "led-gpios" / "button-gpios" in the DT; -EPROBE_DEFER until the GPIO controller is there */
	g->led = devm_gpiod_get(dev, "led", GPIOD_OUT_LOW);
	if (IS_ERR(g->led))
		return dev_err_probe(dev, PTR_ERR(g->led), "no led gpio\n");
	g->button = devm_gpiod_get(dev, "button", GPIOD_IN);
	if (IS_ERR(g->button))
		return dev_err_probe(dev, PTR_ERR(g->button), "no button gpio\n");
	platform_set_drvdata(pdev, g);
	dev_info(dev, "led=gpio%d button=gpio%d (global numbers), button reads %d\n",
		 desc_to_gpio(g->led), desc_to_gpio(g->button), gpiod_get_value_cansleep(g->button));
	return 0;
}

static const struct of_device_id box_of_match[] = {
	{ .compatible = "box,gpio-demo" },
	{ }
};
MODULE_DEVICE_TABLE(of, box_of_match);

static struct platform_driver box_driver = {
	.probe = box_probe,
	.driver = {
		.name = "box-gpio-demo",
		.of_match_table = box_of_match,
		.dev_groups = box_groups,
	},
};
module_platform_driver(box_driver);
MODULE_DESCRIPTION("GPIO consumer example");
MODULE_LICENSE("GPL");

// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: platform driver using GPIO lines from the DT (gpiod API): an output "enable" set at probe, sysfs to toggle it.
 */
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>

struct @NAME@_priv {
	struct gpio_desc *enable;
};

static ssize_t enable_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct @NAME@_priv *p = dev_get_drvdata(dev);
	bool on;
	int ret = kstrtobool(buf, &on);

	if (ret)
		return ret;
	gpiod_set_value_cansleep(p->enable, on);
	return count;
}

static ssize_t enable_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct @NAME@_priv *p = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%d\n", gpiod_get_value_cansleep(p->enable));
}
static DEVICE_ATTR_RW(enable);

static struct attribute *@NAME@_attrs[] = {
	&dev_attr_enable.attr,
	NULL,
};
ATTRIBUTE_GROUPS(@NAME@);

static int @NAME@_probe(struct platform_device *pdev)
{
	struct @NAME@_priv *p;

	p = devm_kzalloc(&pdev->dev, sizeof(*p), GFP_KERNEL);
	if (!p)
		return -ENOMEM;
	p->enable = devm_gpiod_get(&pdev->dev, "enable", GPIOD_OUT_HIGH);	/* "enable-gpios" */
	if (IS_ERR(p->enable))
		return dev_err_probe(&pdev->dev, PTR_ERR(p->enable), "no enable gpio\n");
	platform_set_drvdata(pdev, p);
	dev_info(&pdev->dev, "enable line is high\n");
	return 0;
}

static const struct of_device_id @NAME@_of_match[] = {
	{ .compatible = "vendor,@NAME_DASH@" },
	{ }
};
MODULE_DEVICE_TABLE(of, @NAME@_of_match);

static struct platform_driver @NAME@_driver = {
	.probe = @NAME@_probe,
	.driver = {
		.name = "@NAME_DASH@",
		.of_match_table = @NAME@_of_match,
		.dev_groups = @NAME@_groups,
	},
};
module_platform_driver(@NAME@_driver);
MODULE_DESCRIPTION("@NAME@ GPIO consumer");
MODULE_LICENSE("GPL");

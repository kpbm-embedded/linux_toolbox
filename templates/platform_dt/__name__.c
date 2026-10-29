// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: platform driver bound from the device tree (compatible "vendor,@NAME_DASH@"), with a property and a sysfs attribute.
 */
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/version.h>

struct @NAME@_priv {
	u32 rate;
};

static ssize_t rate_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct @NAME@_priv *p = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%u\n", p->rate);
}
static DEVICE_ATTR_RO(rate);

static struct attribute *@NAME@_attrs[] = {
	&dev_attr_rate.attr,
	NULL,
};
ATTRIBUTE_GROUPS(@NAME@);

static int @NAME@_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct @NAME@_priv *p;

	p = devm_kzalloc(dev, sizeof(*p), GFP_KERNEL);
	if (!p)
		return -ENOMEM;
	if (device_property_read_u32(dev, "vendor,rate", &p->rate))
		p->rate = 100;	/* optional property, default */
	platform_set_drvdata(pdev, p);
	dev_info(dev, "probed, rate %u\n", p->rate);
	return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 11, 0)
static void @NAME@_remove(struct platform_device *pdev)
#else
static int @NAME@_remove(struct platform_device *pdev)
#endif
{
	dev_info(&pdev->dev, "removed\n");
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 11, 0)
	return 0;
#endif
}

static const struct of_device_id @NAME@_of_match[] = {
	{ .compatible = "vendor,@NAME_DASH@" },
	{ }
};
MODULE_DEVICE_TABLE(of, @NAME@_of_match);

static struct platform_driver @NAME@_driver = {
	.probe = @NAME@_probe,
	.remove = @NAME@_remove,
	.driver = {
		.name = "@NAME_DASH@",
		.of_match_table = @NAME@_of_match,
		.dev_groups = @NAME@_groups,
	},
};
module_platform_driver(@NAME@_driver);
MODULE_DESCRIPTION("@NAME@ platform driver");
MODULE_LICENSE("GPL");

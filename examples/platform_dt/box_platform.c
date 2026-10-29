// SPDX-License-Identifier: GPL-2.0
/*
 * box_platform: platform driver bound from the device tree (compatible "box,sensor"): reads string, u32-array and enum properties, one device per DT node, a sysfs attribute per device.
 */
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/property.h>

#define BOX_MAX_CH 8

struct box_sensor {
	const char *label;
	u32 channels[BOX_MAX_CH];
	int nch;
	int gain;
};

static const char *const box_gains[] = { "low", "mid", "high" };

static ssize_t label_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct box_sensor *s = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%s\n", s->label);
}
static DEVICE_ATTR_RO(label);

static ssize_t gain_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct box_sensor *s = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%s\n", box_gains[s->gain]);
}
static DEVICE_ATTR_RO(gain);

static struct attribute *box_attrs[] = {
	&dev_attr_label.attr,
	&dev_attr_gain.attr,
	NULL,
};
ATTRIBUTE_GROUPS(box);

static int box_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct box_sensor *s;
	int ret;

	s = devm_kzalloc(dev, sizeof(*s), GFP_KERNEL);
	if (!s)
		return -ENOMEM;
	if (device_property_read_string(dev, "label", &s->label))
		return dev_err_probe(dev, -EINVAL, "missing label\n");
	s->nch = device_property_count_u32(dev, "box,channels");
	if (s->nch < 1 || s->nch > BOX_MAX_CH)
		return dev_err_probe(dev, -EINVAL, "box,channels needs 1..%d cells\n", BOX_MAX_CH);
	ret = device_property_read_u32_array(dev, "box,channels", s->channels, s->nch);
	if (ret)
		return ret;
	ret = device_property_match_property_string(dev, "box,gain", box_gains, ARRAY_SIZE(box_gains));
	s->gain = ret < 0 ? 1 : ret;	/* optional, default "mid" */
	platform_set_drvdata(pdev, s);
	dev_info(dev, "label=%s channels=%d first=%u gain=%s\n", s->label, s->nch, s->channels[0],
		 box_gains[s->gain]);
	return 0;
}

static const struct of_device_id box_of_match[] = {
	{ .compatible = "box,sensor" },
	{ }
};
MODULE_DEVICE_TABLE(of, box_of_match);

static struct platform_driver box_driver = {
	.probe = box_probe,
	.driver = {
		.name = "box-sensor",
		.of_match_table = box_of_match,
		.dev_groups = box_groups,
	},
};
module_platform_driver(box_driver);
MODULE_DESCRIPTION("platform driver with DT binding example");
MODULE_LICENSE("GPL");

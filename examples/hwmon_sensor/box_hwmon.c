// SPDX-License-Identifier: GPL-2.0
/*
 * box_hwmon: hwmon driver (hwmon_device_register_with_info) for a fake temperature sensor on a platform device created by the module itself: temp1_input, temp1_max (rw), temp1_label.
 */
#include <linux/err.h>
#include <linux/hwmon.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static int base_mc = 42000;
module_param(base_mc, int, 0444);
MODULE_PARM_DESC(base_mc, "reported temperature in milli-degrees C");

static struct platform_device *box_pdev;
static long box_max = 80000;

static umode_t box_is_visible(const void *data, enum hwmon_sensor_types type, u32 attr, int channel)
{
	switch (attr) {
	case hwmon_temp_input:
	case hwmon_temp_label:
		return 0444;
	case hwmon_temp_max:
		return 0644;
	default:
		return 0;
	}
}

static int box_read(struct device *dev, enum hwmon_sensor_types type, u32 attr, int channel, long *val)
{
	switch (attr) {
	case hwmon_temp_input:
		*val = base_mc;
		return 0;
	case hwmon_temp_max:
		*val = box_max;
		return 0;
	default:
		return -EOPNOTSUPP;
	}
}

static int box_write(struct device *dev, enum hwmon_sensor_types type, u32 attr, int channel, long val)
{
	if (attr != hwmon_temp_max)
		return -EOPNOTSUPP;
	box_max = clamp_val(val, -40000, 150000);
	return 0;
}

static int box_read_string(struct device *dev, enum hwmon_sensor_types type, u32 attr, int channel,
			   const char **str)
{
	*str = "box board";
	return 0;
}

static const struct hwmon_channel_info *const box_info[] = {
	HWMON_CHANNEL_INFO(temp, HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_LABEL),
	NULL
};

static const struct hwmon_ops box_ops = {
	.is_visible = box_is_visible,
	.read = box_read,
	.write = box_write,
	.read_string = box_read_string,
};

static const struct hwmon_chip_info box_chip = {
	.ops = &box_ops,
	.info = box_info,
};

static int __init box_init(void)
{
	struct device *hw;

	box_pdev = platform_device_register_simple("box-hwmon", -1, NULL, 0);
	if (IS_ERR(box_pdev))
		return PTR_ERR(box_pdev);
	hw = hwmon_device_register_with_info(&box_pdev->dev, "box_hwmon", NULL, &box_chip, NULL);
	if (IS_ERR(hw)) {
		platform_device_unregister(box_pdev);
		return PTR_ERR(hw);
	}
	platform_set_drvdata(box_pdev, hw);
	dev_info(hw, "registered\n");
	return 0;
}

static void __exit box_exit(void)
{
	hwmon_device_unregister(platform_get_drvdata(box_pdev));
	platform_device_unregister(box_pdev);
}

module_init(box_init);
module_exit(box_exit);
MODULE_DESCRIPTION("hwmon example");
MODULE_LICENSE("GPL");

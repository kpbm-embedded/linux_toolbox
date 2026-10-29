// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: I2C client driver (SMBus register access, sysfs attribute); the test uses the kernel's i2c-stub adapter.
 */
#include <linux/i2c.h>
#include <linux/module.h>

#define REG_CTRL 0x01

static ssize_t ctrl_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int v = i2c_smbus_read_byte_data(to_i2c_client(dev), REG_CTRL);

	return v < 0 ? v : sysfs_emit(buf, "%d\n", v);
}

static ssize_t ctrl_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	u8 v;
	int ret = kstrtou8(buf, 0, &v);

	if (!ret)
		ret = i2c_smbus_write_byte_data(to_i2c_client(dev), REG_CTRL, v);
	return ret ? ret : count;
}
static DEVICE_ATTR_RW(ctrl);

static struct attribute *@NAME@_attrs[] = {
	&dev_attr_ctrl.attr,
	NULL,
};
ATTRIBUTE_GROUPS(@NAME@);

static int @NAME@_probe(struct i2c_client *client)
{
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;
	dev_info(&client->dev, "probed at 0x%02x\n", client->addr);
	return 0;
}

static const struct i2c_device_id @NAME@_id[] = {
	{ "@NAME@" },
	{ }
};
MODULE_DEVICE_TABLE(i2c, @NAME@_id);

static const struct of_device_id @NAME@_of_match[] = {
	{ .compatible = "vendor,@NAME_DASH@" },
	{ }
};
MODULE_DEVICE_TABLE(of, @NAME@_of_match);

static struct i2c_driver @NAME@_driver = {
	.driver = {
		.name = "@NAME@",
		.of_match_table = @NAME@_of_match,
		.dev_groups = @NAME@_groups,
	},
	.probe = @NAME@_probe,
	.id_table = @NAME@_id,
};
module_i2c_driver(@NAME@_driver);
MODULE_DESCRIPTION("@NAME@ I2C driver");
MODULE_LICENSE("GPL");

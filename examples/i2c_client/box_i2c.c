// SPDX-License-Identifier: GPL-2.0
/*
 * box_i2c: I2C client driver (SMBus byte/word access, sysfs attributes) tested against the kernel's i2c-stub fake adapter; the device is created at run time with new_device.
 */
#include <linux/i2c.h>
#include <linux/module.h>

#define BOX_REG_ID	0x00
#define BOX_REG_CTRL	0x01

static ssize_t ctrl_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int v = i2c_smbus_read_byte_data(to_i2c_client(dev), BOX_REG_CTRL);

	return v < 0 ? v : sysfs_emit(buf, "%d\n", v);
}

static ssize_t ctrl_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	u8 v;
	int ret = kstrtou8(buf, 0, &v);

	if (!ret)
		ret = i2c_smbus_write_byte_data(to_i2c_client(dev), BOX_REG_CTRL, v);
	return ret ? ret : count;
}
static DEVICE_ATTR_RW(ctrl);

static struct attribute *box_attrs[] = {
	&dev_attr_ctrl.attr,
	NULL,
};
ATTRIBUTE_GROUPS(box);

static int box_probe(struct i2c_client *client)
{
	int ret;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;
	/* a real driver checks a chip ID here; i2c-stub starts with all registers 0 */
	ret = i2c_smbus_write_byte_data(client, BOX_REG_CTRL, 0x5a);
	if (ret)
		return ret;
	dev_info(&client->dev, "probed at 0x%02x, ctrl=0x%02x\n", client->addr,
		 i2c_smbus_read_byte_data(client, BOX_REG_CTRL));
	return 0;
}

static void box_remove(struct i2c_client *client)
{
	dev_info(&client->dev, "removed\n");
}

static const struct i2c_device_id box_id[] = {
	{ "box_i2c" },
	{ }
};
MODULE_DEVICE_TABLE(i2c, box_id);

static const struct of_device_id box_of_match[] = {
	{ .compatible = "box,i2c-demo" },
	{ }
};
MODULE_DEVICE_TABLE(of, box_of_match);

static struct i2c_driver box_driver = {
	.driver = {
		.name = "box_i2c",
		.of_match_table = box_of_match,
		.dev_groups = box_groups,
	},
	.probe = box_probe,
	.remove = box_remove,
	.id_table = box_id,
};
module_i2c_driver(box_driver);
MODULE_DESCRIPTION("I2C client example");
MODULE_LICENSE("GPL");

// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: kobject /sys/kernel/@NAME@ with a read-write attribute "value".
 */
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/sysfs.h>

static struct kobject *@NAME@_kobj;
static int value;

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%d\n", value);
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret = kstrtoint(buf, 0, &value);

	return ret ? ret : count;
}

static struct kobj_attribute value_attr = __ATTR_RW(value);

static struct attribute *@NAME@_attrs[] = {
	&value_attr.attr,
	NULL,
};

static const struct attribute_group @NAME@_group = {
	.attrs = @NAME@_attrs,
};

static int __init @NAME@_init(void)
{
	int ret;

	@NAME@_kobj = kobject_create_and_add("@NAME@", kernel_kobj);
	if (!@NAME@_kobj)
		return -ENOMEM;
	ret = sysfs_create_group(@NAME@_kobj, &@NAME@_group);
	if (ret)
		kobject_put(@NAME@_kobj);
	return ret;
}

static void __exit @NAME@_exit(void)
{
	kobject_put(@NAME@_kobj);
}

module_init(@NAME@_init);
module_exit(@NAME@_exit);
MODULE_DESCRIPTION("@NAME@ sysfs attributes");
MODULE_LICENSE("GPL");

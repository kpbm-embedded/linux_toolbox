// SPDX-License-Identifier: GPL-2.0
/*
 * box_sysfs: kobject /sys/kernel/box_sysfs with a read-write attribute "value" and a read-only "doubled".
 */
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/sysfs.h>

static struct kobject *box_kobj;
static int box_value;

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%d\n", box_value);
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret = kstrtoint(buf, 0, &box_value);

	return ret ? ret : count;
}

static ssize_t doubled_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%d\n", 2 * box_value);
}

static struct kobj_attribute value_attr = __ATTR_RW(value);
static struct kobj_attribute doubled_attr = __ATTR_RO(doubled);

static struct attribute *box_attrs[] = {
	&value_attr.attr,
	&doubled_attr.attr,
	NULL,
};

static const struct attribute_group box_group = {
	.attrs = box_attrs,
};

static int __init box_init(void)
{
	int ret;

	box_kobj = kobject_create_and_add("box_sysfs", kernel_kobj);
	if (!box_kobj)
		return -ENOMEM;
	ret = sysfs_create_group(box_kobj, &box_group);
	if (ret)
		kobject_put(box_kobj);
	return ret;
}

static void __exit box_exit(void)
{
	kobject_put(box_kobj);	/* also removes the attributes */
}

module_init(box_init);
module_exit(box_exit);
MODULE_DESCRIPTION("sysfs attribute example");
MODULE_LICENSE("GPL");

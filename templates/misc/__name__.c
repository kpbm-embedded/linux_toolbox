// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: misc device /dev/@NAME@ with read, write and an ioctl.
 */
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#define @NAME@_IOC_GET _IOR('b', 1, int)	/* put in a uapi header for user space */

static int value;

static ssize_t @NAME@_read(struct file *file, char __user *ubuf, size_t len, loff_t *off)
{
	char line[24];
	int n = scnprintf(line, sizeof(line), "%d\n", value);

	return simple_read_from_buffer(ubuf, len, off, line, n);
}

static ssize_t @NAME@_write(struct file *file, const char __user *ubuf, size_t len, loff_t *off)
{
	int ret = kstrtoint_from_user(ubuf, len, 0, &value);

	return ret ? ret : len;
}

static long @NAME@_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case @NAME@_IOC_GET:
		return put_user(value, (int __user *)arg);
	default:
		return -ENOTTY;
	}
}

static const struct file_operations @NAME@_fops = {
	.owner = THIS_MODULE,
	.read = @NAME@_read,
	.write = @NAME@_write,
	.unlocked_ioctl = @NAME@_ioctl,
	.compat_ioctl = compat_ptr_ioctl,
};

static struct miscdevice @NAME@_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "@NAME@",
	.fops = &@NAME@_fops,
};

module_misc_device(@NAME@_dev);
MODULE_DESCRIPTION("@NAME@ misc device");
MODULE_LICENSE("GPL");

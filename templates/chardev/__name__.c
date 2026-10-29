// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: character device /dev/@NAME@0 (cdev + class) with a buffer: write, then read it back.
 */
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#define BUF_SIZE 4096

static dev_t devno;
static struct cdev @NAME@_cdev;
static struct class *@NAME@_class;
static char buf[BUF_SIZE];
static size_t used;
static DEFINE_MUTEX(lock);

static ssize_t @NAME@_read(struct file *file, char __user *ubuf, size_t len, loff_t *off)
{
	ssize_t ret;

	mutex_lock(&lock);
	ret = simple_read_from_buffer(ubuf, len, off, buf, used);
	mutex_unlock(&lock);
	return ret;
}

static ssize_t @NAME@_write(struct file *file, const char __user *ubuf, size_t len, loff_t *off)
{
	ssize_t ret;

	mutex_lock(&lock);
	ret = simple_write_to_buffer(buf, sizeof(buf), off, ubuf, len);
	if (ret > 0 && *off > used)
		used = *off;
	mutex_unlock(&lock);
	return ret;
}

static const struct file_operations @NAME@_fops = {
	.owner = THIS_MODULE,
	.read = @NAME@_read,
	.write = @NAME@_write,
	.llseek = default_llseek,
};

static int __init @NAME@_init(void)
{
	struct device *dev;
	int ret;

	ret = alloc_chrdev_region(&devno, 0, 1, "@NAME@");
	if (ret)
		return ret;
	cdev_init(&@NAME@_cdev, &@NAME@_fops);
	ret = cdev_add(&@NAME@_cdev, devno, 1);
	if (ret)
		goto err_region;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
	@NAME@_class = class_create("@NAME@");
#else
	@NAME@_class = class_create(THIS_MODULE, "@NAME@");
#endif
	if (IS_ERR(@NAME@_class)) {
		ret = PTR_ERR(@NAME@_class);
		goto err_cdev;
	}
	dev = device_create(@NAME@_class, NULL, devno, NULL, "@NAME@0");
	if (IS_ERR(dev)) {
		ret = PTR_ERR(dev);
		goto err_class;
	}
	return 0;

err_class:
	class_destroy(@NAME@_class);
err_cdev:
	cdev_del(&@NAME@_cdev);
err_region:
	unregister_chrdev_region(devno, 1);
	return ret;
}

static void __exit @NAME@_exit(void)
{
	device_destroy(@NAME@_class, devno);
	class_destroy(@NAME@_class);
	cdev_del(&@NAME@_cdev);
	unregister_chrdev_region(devno, 1);
}

module_init(@NAME@_init);
module_exit(@NAME@_exit);
MODULE_DESCRIPTION("@NAME@ character device");
MODULE_LICENSE("GPL");

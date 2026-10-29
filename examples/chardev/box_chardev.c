// SPDX-License-Identifier: GPL-2.0
/*
 * box_chardev: character device (cdev + class) with a 4 KiB buffer: what you write you read back, /dev/boxchar0.
 */
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#define BOX_BUF_SIZE 4096

static dev_t devno;
static struct cdev box_cdev;
static struct class *box_class;
static char box_buf[BOX_BUF_SIZE];
static size_t box_used;
static DEFINE_MUTEX(box_lock);

static int box_open(struct inode *inode, struct file *file)
{
	if ((file->f_flags & O_ACCMODE) != O_RDONLY && (file->f_flags & O_TRUNC)) {
		mutex_lock(&box_lock);
		box_used = 0;
		mutex_unlock(&box_lock);
	}
	return 0;
}

static ssize_t box_read(struct file *file, char __user *ubuf, size_t len, loff_t *off)
{
	ssize_t ret;

	mutex_lock(&box_lock);
	ret = simple_read_from_buffer(ubuf, len, off, box_buf, box_used);
	mutex_unlock(&box_lock);
	return ret;
}

static ssize_t box_write(struct file *file, const char __user *ubuf, size_t len, loff_t *off)
{
	ssize_t ret;

	mutex_lock(&box_lock);
	ret = simple_write_to_buffer(box_buf, sizeof(box_buf), off, ubuf, len);
	if (ret > 0 && *off > box_used)
		box_used = *off;
	mutex_unlock(&box_lock);
	return ret;
}

static const struct file_operations box_fops = {
	.owner = THIS_MODULE,
	.open = box_open,
	.read = box_read,
	.write = box_write,
	.llseek = default_llseek,
};

static int __init box_init(void)
{
	struct device *dev;
	int ret;

	ret = alloc_chrdev_region(&devno, 0, 1, "boxchar");
	if (ret)
		return ret;
	cdev_init(&box_cdev, &box_fops);
	ret = cdev_add(&box_cdev, devno, 1);
	if (ret)
		goto err_region;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
	box_class = class_create("boxchar");
#else
	box_class = class_create(THIS_MODULE, "boxchar");
#endif
	if (IS_ERR(box_class)) {
		ret = PTR_ERR(box_class);
		goto err_cdev;
	}
	dev = device_create(box_class, NULL, devno, NULL, "boxchar0");
	if (IS_ERR(dev)) {
		ret = PTR_ERR(dev);
		goto err_class;
	}
	pr_info("box_chardev: /dev/boxchar0 is %d:%d\n", MAJOR(devno), MINOR(devno));
	return 0;

err_class:
	class_destroy(box_class);
err_cdev:
	cdev_del(&box_cdev);
err_region:
	unregister_chrdev_region(devno, 1);
	return ret;
}

static void __exit box_exit(void)
{
	device_destroy(box_class, devno);
	class_destroy(box_class);
	cdev_del(&box_cdev);
	unregister_chrdev_region(devno, 1);
	pr_info("box_chardev: removed\n");
}

module_init(box_init);
module_exit(box_exit);
MODULE_DESCRIPTION("Character device example");
MODULE_LICENSE("GPL");

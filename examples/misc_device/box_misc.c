// SPDX-License-Identifier: GPL-2.0
/*
 * box_misc: misc device /dev/boxmisc: each read returns "reads=N", writing "reset" sets N to 0, anything else is -EINVAL.
 */
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/uaccess.h>

static atomic_t box_reads = ATOMIC_INIT(0);

static ssize_t box_read(struct file *file, char __user *ubuf, size_t len, loff_t *off)
{
	char line[32];
	int n;

	/* one "reads=N" line per open file: count it when the read starts at offset 0 */
	n = scnprintf(line, sizeof(line), "reads=%d\n",
		      *off ? atomic_read(&box_reads) : atomic_inc_return(&box_reads));
	return simple_read_from_buffer(ubuf, len, off, line, n);
}

static ssize_t box_write(struct file *file, const char __user *ubuf, size_t len, loff_t *off)
{
	char cmd[16] = "";

	if (len >= sizeof(cmd) || copy_from_user(cmd, ubuf, len))
		return -EINVAL;
	if (!sysfs_streq(cmd, "reset"))
		return -EINVAL;
	atomic_set(&box_reads, 0);
	return len;
}

static const struct file_operations box_fops = {
	.owner = THIS_MODULE,
	.read = box_read,
	.write = box_write,
};

static struct miscdevice box_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "boxmisc",
	.fops = &box_fops,
	.mode = 0666,
};

static int __init box_init(void)
{
	int ret = misc_register(&box_misc);

	if (!ret)
		pr_info("box_misc: /dev/%s minor %d\n", box_misc.name, box_misc.minor);
	return ret;
}

static void __exit box_exit(void)
{
	misc_deregister(&box_misc);
}

module_init(box_init);
module_exit(box_exit);
MODULE_DESCRIPTION("misc device example");
MODULE_LICENSE("GPL");

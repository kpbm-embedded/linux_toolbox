// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: debugfs folder /sys/kernel/debug/@NAME@ with a u32 and a seq_file.
 */
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/seq_file.h>

static struct dentry *@NAME@_dir;
static u32 counter;

static int state_show(struct seq_file *s, void *unused)
{
	seq_printf(s, "counter=%u\n", counter);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(state);

static int __init @NAME@_init(void)
{
	@NAME@_dir = debugfs_create_dir("@NAME@", NULL);
	debugfs_create_u32("counter", 0600, @NAME@_dir, &counter);
	debugfs_create_file("state", 0400, @NAME@_dir, NULL, &state_fops);
	return 0;
}

static void __exit @NAME@_exit(void)
{
	debugfs_remove(@NAME@_dir);
}

module_init(@NAME@_init);
module_exit(@NAME@_exit);
MODULE_DESCRIPTION("@NAME@ debugfs");
MODULE_LICENSE("GPL");

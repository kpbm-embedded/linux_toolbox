// SPDX-License-Identifier: GPL-2.0
/*
 * box_debugfs: debugfs directory /sys/kernel/debug/box_debugfs: a u32 "counter" and a seq_file "stats".
 */
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/seq_file.h>

static struct dentry *box_dir;
static u32 box_counter;
static unsigned int box_opens;

static int stats_show(struct seq_file *s, void *unused)
{
	box_opens++;
	seq_printf(s, "counter=%u\nopens=%u\n", box_counter, box_opens);
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(stats);

static int __init box_init(void)
{
	box_dir = debugfs_create_dir("box_debugfs", NULL);
	debugfs_create_u32("counter", 0600, box_dir, &box_counter);
	debugfs_create_file("stats", 0400, box_dir, NULL, &stats_fops);
	pr_info("box_debugfs: ready\n");
	return 0;
}

static void __exit box_exit(void)
{
	debugfs_remove(box_dir);	/* recursive */
}

module_init(box_init);
module_exit(box_exit);
MODULE_DESCRIPTION("debugfs example");
MODULE_LICENSE("GPL");

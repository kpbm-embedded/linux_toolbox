// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: /proc/@NAME@ file with single_open (seq_file).
 */
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int @NAME@_show(struct seq_file *s, void *unused)
{
	seq_printf(s, "name=@NAME@\nuptime_ms=%u\n", jiffies_to_msecs(jiffies - INITIAL_JIFFIES));
	return 0;
}

static int __init @NAME@_init(void)
{
	return proc_create_single("@NAME@", 0444, NULL, @NAME@_show) ? 0 : -ENOMEM;
}

static void __exit @NAME@_exit(void)
{
	remove_proc_entry("@NAME@", NULL);
}

module_init(@NAME@_init);
module_exit(@NAME@_exit);
MODULE_DESCRIPTION("@NAME@ procfs");
MODULE_LICENSE("GPL");

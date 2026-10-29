// SPDX-License-Identifier: GPL-2.0
/*
 * box_proc: /proc/box_seq through the seq_file iterator (start/next/stop/show): one line per item, "lines" module parameter.
 */
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static unsigned int lines = 5;
module_param(lines, uint, 0444);
MODULE_PARM_DESC(lines, "number of lines in /proc/box_seq");

static void *box_start(struct seq_file *s, loff_t *pos)
{
	return *pos < lines ? pos : NULL;
}

static void *box_next(struct seq_file *s, void *v, loff_t *pos)
{
	++*pos;
	return *pos < lines ? pos : NULL;
}

static void box_stop(struct seq_file *s, void *v)
{
}

static int box_show(struct seq_file *s, void *v)
{
	loff_t i = *(loff_t *)v;

	seq_printf(s, "line %lld/%u\n", i + 1, lines);
	return 0;
}

static const struct seq_operations box_seq_ops = {
	.start = box_start,
	.next = box_next,
	.stop = box_stop,
	.show = box_show,
};

static int __init box_init(void)
{
	if (!proc_create_seq("box_seq", 0444, NULL, &box_seq_ops))
		return -ENOMEM;
	return 0;
}

static void __exit box_exit(void)
{
	remove_proc_entry("box_seq", NULL);
}

module_init(box_init);
module_exit(box_exit);
MODULE_DESCRIPTION("procfs seq_file example");
MODULE_LICENSE("GPL");

// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: kernel thread doing periodic work until the module is unloaded.
 */
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>

static unsigned int period_ms = 100;
module_param(period_ms, uint, 0444);

static unsigned int loops;
module_param(loops, uint, 0444);

static struct task_struct *@NAME@_task;

static int @NAME@_fn(void *data)
{
	while (!kthread_should_stop()) {
		WRITE_ONCE(loops, loops + 1);	/* the periodic work */
		msleep_interruptible(period_ms);
	}
	return 0;
}

static int __init @NAME@_init(void)
{
	@NAME@_task = kthread_run(@NAME@_fn, NULL, "@NAME@");
	return PTR_ERR_OR_ZERO(@NAME@_task);
}

static void __exit @NAME@_exit(void)
{
	kthread_stop(@NAME@_task);
	pr_info("@NAME@: stopped after %u loops\n", loops);
}

module_init(@NAME@_init);
module_exit(@NAME@_exit);
MODULE_DESCRIPTION("@NAME@ kthread");
MODULE_LICENSE("GPL");

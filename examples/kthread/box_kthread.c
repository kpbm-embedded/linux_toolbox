// SPDX-License-Identifier: GPL-2.0
/*
 * box_kthread: kernel thread that wakes every period_ms and counts ticks (read-only parameter "ticks"); stopped cleanly on unload.
 */
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>

static unsigned int period_ms = 20;
module_param(period_ms, uint, 0444);
MODULE_PARM_DESC(period_ms, "wake-up period");

static unsigned int ticks;
module_param(ticks, uint, 0444);
MODULE_PARM_DESC(ticks, "wake-ups so far (read only)");

static struct task_struct *box_task;

static int box_thread(void *data)
{
	pr_info("box_kthread: running as pid %d\n", current->pid);
	while (!kthread_should_stop()) {
		WRITE_ONCE(ticks, ticks + 1);
		msleep_interruptible(period_ms);
	}
	return 0;
}

static int __init box_init(void)
{
	box_task = kthread_run(box_thread, NULL, "box_kthread");
	return PTR_ERR_OR_ZERO(box_task);
}

static void __exit box_exit(void)
{
	kthread_stop(box_task);
	pr_info("box_kthread: stopped after %u ticks\n", ticks);
}

module_init(box_init);
module_exit(box_exit);
MODULE_DESCRIPTION("kthread example");
MODULE_LICENSE("GPL");

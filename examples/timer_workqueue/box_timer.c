// SPDX-License-Identifier: GPL-2.0
/*
 * box_timer: timer (softirq context) that queues work on its own workqueue (process context) every 50 ms; counter in the "works" parameter.
 */
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

static unsigned int works;
module_param(works, uint, 0444);
MODULE_PARM_DESC(works, "work items run so far (read only)");

static struct workqueue_struct *box_wq;
static struct timer_list box_timer;
static struct work_struct box_work;

static void box_work_fn(struct work_struct *work)
{
	if (!works)
		pr_info("box_timer: work ran in process context (pid %d)\n", current->pid);
	WRITE_ONCE(works, works + 1);
}

static void box_timer_fn(struct timer_list *t)
{
	queue_work(box_wq, &box_work);	/* no sleeping here: hand over to the workqueue */
	mod_timer(&box_timer, jiffies + msecs_to_jiffies(50));
}

static int __init box_init(void)
{
	box_wq = alloc_workqueue("box_wq", WQ_UNBOUND, 1);
	if (!box_wq)
		return -ENOMEM;
	INIT_WORK(&box_work, box_work_fn);
	timer_setup(&box_timer, box_timer_fn, 0);
	mod_timer(&box_timer, jiffies + msecs_to_jiffies(50));
	return 0;
}

static void __exit box_exit(void)
{
	timer_delete_sync(&box_timer);
	cancel_work_sync(&box_work);
	destroy_workqueue(box_wq);
	pr_info("box_timer: stopped after %u works\n", works);
}

module_init(box_init);
module_exit(box_exit);
MODULE_DESCRIPTION("timer + workqueue example");
MODULE_LICENSE("GPL");

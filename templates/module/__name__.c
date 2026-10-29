// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: minimal module: load/unload messages and a parameter.
 */
#include <linux/module.h>

static int level = 1;
module_param(level, int, 0644);
MODULE_PARM_DESC(level, "example parameter");

static int __init @NAME@_init(void)
{
	pr_info("@NAME@: loaded, level=%d\n", level);
	return 0;
}

static void __exit @NAME@_exit(void)
{
	pr_info("@NAME@: unloaded\n");
}

module_init(@NAME@_init);
module_exit(@NAME@_exit);
MODULE_DESCRIPTION("@NAME@");
MODULE_LICENSE("GPL");

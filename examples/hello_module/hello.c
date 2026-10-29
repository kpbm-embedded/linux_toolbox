// SPDX-License-Identifier: GPL-2.0
/*
 * hello: minimal out-of-tree kernel module example (32_05_linux examples/).
 * Logs on load/unload and registers a platform driver that binds to a DT node with
 * compatible = "box,hello" (added by examples/dt_overlay/hello-overlay.dtso).
 */
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>

static char *greeting = "hello";
module_param(greeting, charp, 0444);
MODULE_PARM_DESC(greeting, "word printed on load");

static int hello_probe(struct platform_device *pdev)
{
	u32 period = 0;

	of_property_read_u32(pdev->dev.of_node, "box,period-ms", &period);
	dev_info(&pdev->dev, "bound to %pOF, period %u ms\n", pdev->dev.of_node, period);
	return 0;
}

static const struct of_device_id hello_of_match[] = {
	{ .compatible = "box,hello" },
	{ }
};
MODULE_DEVICE_TABLE(of, hello_of_match);

static struct platform_driver hello_driver = {
	.probe = hello_probe,
	.driver = {
		.name = "box-hello",
		.of_match_table = hello_of_match,
	},
};

static int __init hello_init(void)
{
	pr_info("hello: %s from an out-of-tree module\n", greeting);
	return platform_driver_register(&hello_driver);
}

static void __exit hello_exit(void)
{
	platform_driver_unregister(&hello_driver);
	pr_info("hello: unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_DESCRIPTION("Minimal out-of-tree module example");
MODULE_LICENSE("GPL");

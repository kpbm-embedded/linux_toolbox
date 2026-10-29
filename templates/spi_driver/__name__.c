// SPDX-License-Identifier: GPL-2.0
/*
 * @NAME@: SPI device driver skeleton (spi_driver, DT match, one register read helper); the test only loads and unloads it (no SPI controller in the test VM).
 */
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

static int @NAME@_read_reg(struct spi_device *spi, u8 reg)
{
	return spi_w8r8(spi, reg | 0x80);	/* read: MSB set on many chips; check the datasheet */
}

static int @NAME@_probe(struct spi_device *spi)
{
	int id;

	spi->mode = SPI_MODE_0;
	spi->bits_per_word = 8;
	if (spi_setup(spi))
		return -EINVAL;
	id = @NAME@_read_reg(spi, 0x00);
	dev_info(&spi->dev, "probed, id register 0x%02x\n", id);
	return id < 0 ? id : 0;
}

static const struct of_device_id @NAME@_of_match[] = {
	{ .compatible = "vendor,@NAME_DASH@" },
	{ }
};
MODULE_DEVICE_TABLE(of, @NAME@_of_match);

static const struct spi_device_id @NAME@_id[] = {
	{ "@NAME_DASH@" },
	{ }
};
MODULE_DEVICE_TABLE(spi, @NAME@_id);

static struct spi_driver @NAME@_driver = {
	.driver = {
		.name = "@NAME@",
		.of_match_table = @NAME@_of_match,
	},
	.probe = @NAME@_probe,
	.id_table = @NAME@_id,
};
module_spi_driver(@NAME@_driver);
MODULE_DESCRIPTION("@NAME@ SPI driver");
MODULE_LICENSE("GPL");

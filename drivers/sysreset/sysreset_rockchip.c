// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 */

#include <dm.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <stdlib.h>
#include <sysreset.h>
#include <asm/arch-rockchip/clock.h>
#include <asm/arch-rockchip/hardware.h>
#include <linux/err.h>

int rockchip_sysreset_request(struct udevice *dev, enum sysreset_t type)
{
	struct sysreset_reg *offset = dev_get_priv(dev);
	unsigned long cru_base = (unsigned long)rockchip_get_cru();

	if (IS_ERR_VALUE(cru_base))
		return (int)cru_base;

	switch (type) {
	case SYSRESET_WARM:
		writel(0xeca8, cru_base + offset->glb_srst_snd_value);
		break;
	case SYSRESET_COLD:
		writel(0xfdb9, cru_base + offset->glb_srst_fst_value);
		break;
	default:
		return -EPROTONOSUPPORT;
	}

	return -EINPROGRESS;
}

static struct sysreset_ops rockchip_sysreset = {
	.request	= rockchip_sysreset_request,
};

U_BOOT_DRIVER(sysreset_rockchip) = {
	.name	= "rockchip_sysreset",
	.id	= UCLASS_SYSRESET,
	.ops	= &rockchip_sysreset,
};

int rockchip_sysreset_bind(struct udevice *pdev,
			   unsigned long cru_base,
			   unsigned int glb_rst_st,
			   unsigned int glb_srst_fst,
			   unsigned int glb_srst_snd)
{
	struct udevice *sysreset_dev;
	struct sysreset_reg *priv;
	int ret;

	ret = device_bind_driver(pdev, "rockchip_sysreset", "sysreset",
				 &sysreset_dev);
	if (ret)
		return ret;

	priv = malloc(sizeof(struct sysreset_reg));
	priv->glb_srst_fst_value = glb_srst_fst;
	priv->glb_srst_snd_value = glb_srst_snd;
	dev_set_priv(sysreset_dev, priv);

	return 0;
}

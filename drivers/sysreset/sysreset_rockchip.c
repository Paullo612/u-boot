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

enum {
	GLB_POR_RST,
	FST_GLB_RST_ST		= BIT(0),
	SND_GLB_RST_ST		= BIT(1),
	FST_GLB_TSADC_RST_ST	= BIT(2),
	SND_GLB_TSADC_RST_ST	= BIT(3),
	FST_GLB_WDT_RST_ST	= BIT(4),
	SND_GLB_WDT_RST_ST	= BIT(5),
	GLB_RST_ST_MASK		= GENMASK(5, 0),
};

struct sysreset_reg {
	unsigned long cru_base;
	unsigned int glb_rst_st;
	unsigned int glb_srst_fst;
	unsigned int glb_srst_snd;
};

static int rockchip_sysreset_request(struct udevice *dev, enum sysreset_t type)
{
	struct sysreset_reg *priv = dev_get_priv(dev);

	if (!priv->cru_base)
		return -EPROTONOSUPPORT;

	switch (type) {
	case SYSRESET_WARM:
		writel(0xeca8, priv->cru_base + priv->glb_srst_snd);
		break;
	case SYSRESET_COLD:
		writel(0xfdb9, priv->cru_base + priv->glb_srst_fst);
		break;
	default:
		return -EPROTONOSUPPORT;
	}

	return -EINPROGRESS;
}

static const char __maybe_unused *rockchip_sysreset_get_cause(u32 data)
{
	switch (data & GLB_RST_ST_MASK) {
	case GLB_POR_RST:
		return "POR";
	case FST_GLB_RST_ST:
	case SND_GLB_RST_ST:
		return "RST";
	case FST_GLB_TSADC_RST_ST:
	case SND_GLB_TSADC_RST_ST:
		return "THERMAL";
	case FST_GLB_WDT_RST_ST:
	case SND_GLB_WDT_RST_ST:
		return "WDOG";
	default:
		return NULL;
	}
}

static int __maybe_unused rockchip_sysreset_get_status(struct udevice *dev,
						       char *buf, int size)
{
	struct sysreset_reg *priv = dev_get_priv(dev);
	const char *cause;
	u32 data;

	if (!buf || size <= 0)
		return -EINVAL;

	if (!priv->cru_base || !priv->glb_rst_st)
		return -ENOSYS;

	data = readl(priv->cru_base + priv->glb_rst_st);
	cause = rockchip_sysreset_get_cause(data);
	if (!cause)
		return -ENODATA;

	strlcpy(buf, cause, size);

	return 0;
}

static struct sysreset_ops rockchip_sysreset = {
	.request	= rockchip_sysreset_request,
#if !IS_ENABLED(CONFIG_XPL_BUILD)
	.get_status	= rockchip_sysreset_get_status,
#endif
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
	priv->cru_base = cru_base;
	priv->glb_rst_st = glb_rst_st;
	priv->glb_srst_fst = glb_srst_fst;
	priv->glb_srst_snd = glb_srst_snd;
	dev_set_priv(sysreset_dev, priv);

	return 0;
}

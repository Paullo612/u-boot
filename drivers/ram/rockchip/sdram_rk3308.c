// SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
/*
 * (C) Copyright 2019 Rockchip Electronics Co., Ltd.
 */

#include <dm.h>
#include <ram.h>
#include <asm/arch-rockchip/grf_rk3308.h>
#include <asm/arch-rockchip/sdram.h>

#define GRF_BASE			0xff000000

static int rk3308_dmc_get_info(struct udevice *dev, struct ram_info *info)
{
	static struct rk3308_grf * const grf = (void *)GRF_BASE;

	info->base = CFG_SYS_SDRAM_BASE;
	info->size = rockchip_sdram_size((phys_addr_t)&grf->os_reg2);

	return 0;
}

static struct ram_ops rk3308_dmc_ops = {
	.get_info = rk3308_dmc_get_info,
};

static const struct udevice_id rk3308_dmc_ids[] = {
	{ .compatible = "rockchip,rk3308-dmc" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3308_dmc) = {
	.name = "rockchip_rk3308_dmc",
	.id = UCLASS_RAM,
	.of_match = rk3308_dmc_ids,
	.ops = &rk3308_dmc_ops,
};

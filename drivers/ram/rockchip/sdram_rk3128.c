// SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd.
 */

#include <dm.h>
#include <ram.h>
#include <asm/arch-rockchip/grf_rk3128.h>
#include <asm/arch-rockchip/sdram.h>

#define GRF_BASE			0x20008000

static int rk3128_dmc_get_info(struct udevice *dev, struct ram_info *info)
{
	static struct rk3128_grf * const grf = (void *)GRF_BASE;

	info->base = CFG_SYS_SDRAM_BASE;
	info->size = rockchip_sdram_size((phys_addr_t)&grf->os_reg[1]);

	return 0;
}

static struct ram_ops rk3128_dmc_ops = {
	.get_info = rk3128_dmc_get_info,
};

static const struct udevice_id rk3128_dmc_ids[] = {
	{ .compatible = "rockchip,rk3128-dmc" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3128_dmc) = {
	.name = "rockchip_rk3128_dmc",
	.id = UCLASS_RAM,
	.of_match = rk3128_dmc_ids,
	.ops = &rk3128_dmc_ops,
};

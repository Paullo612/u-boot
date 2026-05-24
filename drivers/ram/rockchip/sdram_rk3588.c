// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2021 Rockchip Electronics Co., Ltd.
 */

#include <dm.h>
#include <ram.h>
#include <asm/arch-rockchip/grf_rk3588.h>
#include <asm/arch-rockchip/sdram.h>

#define PMU1GRF_BASE			0xfd58a000

static int rk3588_dmc_get_info(struct udevice *dev, struct ram_info *info)
{
	static struct rk3588_pmu1grf * const pmugrf = (void *)PMU1GRF_BASE;

	info->base = CFG_SYS_SDRAM_BASE;
	info->size = rockchip_sdram_size((phys_addr_t)&pmugrf->os_reg[2]) +
		     rockchip_sdram_size((phys_addr_t)&pmugrf->os_reg[4]);

	return 0;
}

static struct ram_ops rk3588_dmc_ops = {
	.get_info = rk3588_dmc_get_info,
};

static const struct udevice_id rk3588_dmc_ids[] = {
	{ .compatible = "rockchip,rk3588-dmc" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3588_dmc) = {
	.name = "rockchip_rk3588_dmc",
	.id = UCLASS_RAM,
	.of_match = rk3588_dmc_ids,
	.ops = &rk3588_dmc_ops,
};

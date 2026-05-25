// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2021 Rockchip Electronics Co., Ltd.
 */

#include <dm.h>
#include <ram.h>
#include <asm/arch-rockchip/grf_rk3568.h>
#include <asm/arch-rockchip/sdram.h>

#define PMUGRF_BASE			0xfdc20000

#if IS_ENABLED(CONFIG_TPL_BUILD)

int sdram_init(void)
{
	return -ENOSYS;
}

#else /* IS_ENABLED(CONFIG_TPL_BUILD) */

static int rk3568_dmc_get_info(struct udevice *dev, struct ram_info *info)
{
	static struct rk3568_pmugrf * const pmugrf = (void *)PMUGRF_BASE;

	info->base = CFG_SYS_SDRAM_BASE;
	info->size = rockchip_sdram_size((phys_addr_t)&pmugrf->pmu_os_reg2);

	return 0;
}

static struct ram_ops rk3568_dmc_ops = {
	.get_info = rk3568_dmc_get_info,
};

static const struct udevice_id rk3568_dmc_ids[] = {
	{ .compatible = "rockchip,rk3568-dmc" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3568_dmc) = {
	.name = "rockchip_rk3568_dmc",
	.id = UCLASS_RAM,
	.of_match = rk3568_dmc_ids,
	.ops = &rk3568_dmc_ops,
};

#endif /* !IS_ENABLED(CONFIG_TPL_BUILD) */

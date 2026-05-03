// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2021 Rockchip Electronics Co., Ltd
 */

#include <dm.h>
#include <syscon.h>
#include <asm/arch-rockchip/clock.h>

static const struct udevice_id rk3568_syscon_ids[] = {
	{ .compatible = "rockchip,rk3568-grf", .data = ROCKCHIP_SYSCON_GRF },
	{ .compatible = "rockchip,rk3568-pmugrf", .data = ROCKCHIP_SYSCON_PMUGRF },
	{ }
};

U_BOOT_DRIVER(syscon_rk3568) = {
	.name = "rk3568_syscon",
	.id = UCLASS_SYSCON,
	.of_match = rk3568_syscon_ids,
#if CONFIG_IS_ENABLED(OF_REAL)
	.bind = dm_scan_fdt_dev,
#endif
};

#if CONFIG_IS_ENABLED(OF_PLATDATA)
static int rk3568_syscon_bind_of_plat(struct udevice *dev)
{
	dev->driver_data = dev->driver->of_match->data;
	debug("syscon: %s %d\n", dev->name, (uint)dev->driver_data);

	return 0;
}

U_BOOT_DRIVER(rockchip_rk3568_pmugrf) = {
	.name = "rockchip_rk3568_pmugrf",
	.id = UCLASS_SYSCON,
	.of_match = rk3568_syscon_ids + 1,
	.bind = rk3568_syscon_bind_of_plat,
};
#endif

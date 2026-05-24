// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2019 Rockchip Electronics Co., Ltd
 * Copyright (c) 2022 Edgeble AI Technologies Pvt. Ltd.
 */

#include <dm.h>
#include <log.h>
#include <syscon.h>
#include <asm/arch-rockchip/clock.h>

static const struct udevice_id rv1126_syscon_ids[] = {
	{ .compatible = "rockchip,rv1126-grf", .data = ROCKCHIP_SYSCON_GRF },
	{ }
};

U_BOOT_DRIVER(syscon_rv1126) = {
	.name = "rv1126_syscon",
	.id = UCLASS_SYSCON,
	.of_match = rv1126_syscon_ids,
#if CONFIG_IS_ENABLED(OF_REAL)
	.bind = dm_scan_fdt_dev,
#endif
};

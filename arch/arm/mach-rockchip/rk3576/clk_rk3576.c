// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd.
 */

#include <linux/kernel.h>
#include <asm/arch-rockchip/cru_rk3576.h>

void *rockchip_get_cru(void)
{
	return (void *)RK3576_CRU_BASE;
}

// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd.
 */

#include <linux/kernel.h>
#include <asm/arch-rockchip/cru_rk3588.h>

void *rockchip_get_cru(void)
{
	return (void *)CRU_BASE;
}

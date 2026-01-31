// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright Contributors to the U-Boot project.

#include <linux/kernel.h>
#include <asm/arch-rockchip/cru_rk3506.h>

void *rockchip_get_cru(void)
{
	return (void *)RK3506_CRU_BASE;
}

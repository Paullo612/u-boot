/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * (C) Copyright 2019 Amarula Solutions.
 * Author: Jagan Teki <jagan@amarulasolutions.com>
 */

#ifndef _ROCKCHIP_CLOCK_H
#define _ROCKCHIP_CLOCK_H

#ifndef __ASSEMBLY__
#include <linux/bitops.h>
#endif

#if defined(CONFIG_ROCKCHIP_RK3288)
# include <asm/arch-rockchip/cru_rk3288.h>
#elif defined(CONFIG_ROCKCHIP_RK3399)
# include <asm/arch-rockchip/cru_rk3399.h>
#endif

#define MHz		1000000

const char *get_reset_cause(void);

#endif /* _ROCKCHIP_CLOCK_H */

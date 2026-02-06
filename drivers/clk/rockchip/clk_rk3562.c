// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 * Author: Finley Xiao <finley.xiao@rock-chips.com>
 */

#define LOG_CATEGORY UCLASS_CLK

#include <clk-uclass.h>
#include <asm/arch-rockchip/clock.h>
#include <asm/arch-rockchip/cru_rk3562.h>
#include <asm/arch-rockchip/hardware.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <dt-bindings/clock/rockchip,rk3562-cru.h>

#define DIV_TO_RATE(input_rate, div)	((input_rate) / ((div) + 1))

static struct rockchip_pll_rate_table rk3562_pll_rates[] = {
	/* _mhz, _refdiv, _fbdiv, _postdiv1, _postdiv2, _dsmpd, _frac */
	RK3036_PLL_RATE(1608000000, 1, 67, 1, 1, 1, 0),
	RK3036_PLL_RATE(1416000000, 1, 118, 2, 1, 1, 0),
	RK3036_PLL_RATE(1296000000, 1, 108, 2, 1, 1, 0),
	RK3036_PLL_RATE(1200000000, 1, 100, 2, 1, 1, 0),
	RK3036_PLL_RATE(1188000000, 1, 99, 2, 1, 1, 0),
	RK3036_PLL_RATE(1104000000, 1, 92, 2, 1, 1, 0),
	RK3036_PLL_RATE(1008000000, 1, 84, 2, 1, 1, 0),
	RK3036_PLL_RATE(1000000000, 3, 250, 2, 1, 1, 0),
	RK3036_PLL_RATE(983040000, 1, 40, 1, 1, 0, 16106127),
	RK3036_PLL_RATE(912000000, 1, 76, 2, 1, 1, 0),
	RK3036_PLL_RATE(816000000, 1, 68, 2, 1, 1, 0),
	RK3036_PLL_RATE(705600000, 2, 235, 4, 1, 0, 3355443),
	RK3036_PLL_RATE(611520000, 4, 611, 6, 1, 0, 8724152),
	RK3036_PLL_RATE(600000000, 1, 100, 4, 1, 1, 0),
	RK3036_PLL_RATE(594000000, 1, 99, 4, 1, 1, 0),
	RK3036_PLL_RATE(500000000, 1, 125, 6, 1, 1, 0),
	RK3036_PLL_RATE(408000000, 1, 68, 2, 2, 1, 0),
	RK3036_PLL_RATE(312000000, 1, 78, 6, 1, 1, 0),
	RK3036_PLL_RATE(216000000, 1, 72, 4, 2, 1, 0),
	RK3036_PLL_RATE(96000000, 1, 96, 6, 4, 1, 0),
	{ /* sentinel */ },
};

static struct rockchip_pll_clock rk3562_pll_clks[] = {
	[APLL] = PLL(pll_rk3328, PLL_APLL, RK3562_PLL_CON(0),
		     RK3562_MODE_CON, 0, 10, 0, rk3562_pll_rates),
	[GPLL] = PLL(pll_rk3328, PLL_GPLL, RK3562_PLL_CON(24),
		     RK3562_MODE_CON, 2, 10, 0, rk3562_pll_rates),
	[VPLL] = PLL(pll_rk3328, PLL_VPLL, RK3562_PLL_CON(32),
		     RK3562_MODE_CON, 6, 10, 0, rk3562_pll_rates),
	[HPLL] = PLL(pll_rk3328, PLL_HPLL, RK3562_PLL_CON(40),
		     RK3562_MODE_CON, 8, 10, 0, rk3562_pll_rates),
	[CPLL] = PLL(pll_rk3328, PLL_CPLL, RK3562_PMU1_PLL_CON(0),
		     RK3562_PMU1_MODE_CON, 0, 10, 0, rk3562_pll_rates),
	[DPLL] = PLL(pll_rk3328, PLL_DPLL, RK3562_SUBDDR_PLL_CON(0),
		     RK3562_SUBDDR_MODE_CON, 0, 10, 0, NULL),
};

static ulong rk3562_pll_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	static void * const cru_base = (void *)RK3562_TOPCRU_BASE;

	switch (clk_id) {
	case ARMCLK:
	case PLL_APLL:
		return roundup(rockchip_pll_get_rate(&rk3562_pll_clks[APLL],
						     cru_base, APLL), 1000);
	case PLL_CPLL:
		if (!priv->cpll_hz) {
			priv->cpll_hz = rockchip_pll_get_rate(&rk3562_pll_clks[CPLL],
							      cru_base, CPLL);
			priv->cpll_hz = roundup(priv->cpll_hz, 1000);
		}
		return priv->cpll_hz;
	case PLL_DPLL:
		return roundup(rockchip_pll_get_rate(&rk3562_pll_clks[DPLL],
						     cru_base, DPLL), 1000);
	case PLL_GPLL:
		if (!priv->gpll_hz) {
			priv->gpll_hz = rockchip_pll_get_rate(&rk3562_pll_clks[GPLL],
							      cru_base, GPLL);
			priv->gpll_hz = roundup(priv->gpll_hz, 1000);
		}
		return priv->gpll_hz;
	case PLL_HPLL:
		if (!priv->hpll_hz) {
			priv->hpll_hz = rockchip_pll_get_rate(&rk3562_pll_clks[HPLL],
							      cru_base, HPLL);
			priv->hpll_hz = roundup(priv->hpll_hz, 1000);
		}
		return priv->hpll_hz;
	case PLL_VPLL:
		return roundup(rockchip_pll_get_rate(&rk3562_pll_clks[VPLL],
						     cru_base, VPLL), 1000);
	default:
		return -ENOENT;
	}
}

static ulong rk3562_pll_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	static void * const cru_base = (void *)RK3562_TOPCRU_BASE;

	switch (clk_id) {
	case ARMCLK:
	case PLL_APLL:
	case PLL_DPLL:
	case PLL_VPLL:
		break;
	case PLL_CPLL:
		rockchip_pll_set_rate(&rk3562_pll_clks[CPLL],
				      cru_base, CPLL, rate);
		priv->cpll_hz = 0;
		break;
	case PLL_GPLL:
		rockchip_pll_set_rate(&rk3562_pll_clks[GPLL],
				      cru_base, GPLL, rate);
		priv->gpll_hz = 0;
		break;
	case PLL_HPLL:
		rockchip_pll_set_rate(&rk3562_pll_clks[HPLL],
				      cru_base, HPLL, rate);
		priv->hpll_hz = 0;
		break;
	default:
		return -ENOENT;
	}

	return rk3562_pll_get_rate(priv, clk_id);
}

static ulong rk3562_bus_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, div, sel;
	ulong prate;

	switch (clk_id) {
	case ACLK_BUS:
		con = readl(RK3562_CLKSEL_CON(40));
		sel = FIELD_GET(ACLK_BUS_SEL_MASK, con);
		div = FIELD_GET(ACLK_BUS_DIV_MASK, con);
		break;
	case HCLK_BUS:
		con = readl(RK3562_CLKSEL_CON(40));
		sel = FIELD_GET(HCLK_BUS_SEL_MASK, con);
		div = FIELD_GET(HCLK_BUS_DIV_MASK, con);
		break;
	case PCLK_BUS:
		con = readl(RK3562_CLKSEL_CON(41));
		sel = FIELD_GET(PCLK_BUS_SEL_MASK, con);
		div = FIELD_GET(PCLK_BUS_DIV_MASK, con);
		break;
	default:
		return -ENOENT;
	}

	if (sel == ACLK_BUS_SEL_CPLL)
		prate = priv->cpll_hz;
	else
		prate = priv->gpll_hz;

	return DIV_TO_RATE(prate, div);
}

static ulong rk3562_bus_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	u32 div, sel;

	if ((priv->cpll_hz % rate) == 0) {
		sel = ACLK_BUS_SEL_CPLL;
		div = DIV_ROUND_UP(priv->cpll_hz, rate);
	} else {
		sel= ACLK_BUS_SEL_GPLL;
		div = DIV_ROUND_UP(priv->gpll_hz, rate);
	}

	switch (clk_id) {
	case ACLK_BUS:
		rk_clrsetreg(RK3562_CLKSEL_CON(40),
			     ACLK_BUS_SEL_MASK | ACLK_BUS_DIV_MASK,
			     FIELD_PREP(ACLK_BUS_SEL_MASK, sel) |
			     FIELD_PREP(ACLK_BUS_DIV_MASK, div - 1));
		break;
	case HCLK_BUS:
		rk_clrsetreg(RK3562_CLKSEL_CON(40),
			     HCLK_BUS_SEL_MASK | HCLK_BUS_DIV_MASK,
			     FIELD_PREP(HCLK_BUS_SEL_MASK, sel) |
			     FIELD_PREP(HCLK_BUS_DIV_MASK, div - 1));
		break;
	case PCLK_BUS:
		rk_clrsetreg(RK3562_CLKSEL_CON(41),
			     PCLK_BUS_SEL_MASK | PCLK_BUS_DIV_MASK,
			     FIELD_PREP(PCLK_BUS_SEL_MASK, sel) |
			     FIELD_PREP(PCLK_BUS_DIV_MASK, div - 1));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_bus_get_rate(priv, clk_id);
}

static ulong rk3562_peri_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, div, sel;
	ulong prate;

	switch (clk_id) {
	case ACLK_PERI:
		con = readl(RK3562_PERI_CLKSEL_CON(0));
		sel = FIELD_GET(ACLK_PERI_SEL_MASK, con);
		div = FIELD_GET(ACLK_PERI_DIV_MASK, con);
		break;
	case HCLK_PERI:
		con = readl(RK3562_PERI_CLKSEL_CON(0));
		sel = FIELD_GET(HCLK_PERI_SEL_MASK, con);
		div = FIELD_GET(HCLK_PERI_DIV_MASK, con);
		break;
	case PCLK_PERI:
		con = readl(RK3562_PERI_CLKSEL_CON(1));
		sel = FIELD_GET(PCLK_PERI_SEL_MASK, con);
		div = FIELD_GET(PCLK_PERI_DIV_MASK, con);
		break;
	default:
		return -ENOENT;
	}

	if (sel == ACLK_PERI_SEL_CPLL)
		prate = priv->cpll_hz;
	else
		prate = priv->gpll_hz;

	return DIV_TO_RATE(prate, div);
}

static ulong rk3562_peri_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				  ulong rate)
{
	u32 div, sel;

	if ((priv->cpll_hz % rate) == 0) {
		sel = ACLK_PERI_SEL_CPLL;
		div = DIV_ROUND_UP(priv->cpll_hz, rate);
	} else {
		sel= ACLK_PERI_SEL_GPLL;
		div = DIV_ROUND_UP(priv->gpll_hz, rate);
	}

	switch (clk_id) {
	case ACLK_PERI:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(0),
			     ACLK_PERI_SEL_MASK | ACLK_PERI_DIV_MASK,
			     FIELD_PREP(ACLK_PERI_SEL_MASK, sel) |
			     FIELD_PREP(ACLK_PERI_DIV_MASK, div - 1));
		break;
	case HCLK_PERI:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(0),
			     HCLK_PERI_SEL_MASK | HCLK_PERI_DIV_MASK,
			     FIELD_PREP(HCLK_PERI_SEL_MASK, sel) |
			     FIELD_PREP(HCLK_PERI_DIV_MASK, div - 1));
		break;
	case PCLK_PERI:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(1),
			     PCLK_PERI_SEL_MASK | PCLK_PERI_DIV_MASK,
			     FIELD_PREP(PCLK_PERI_SEL_MASK, sel) |
			     FIELD_PREP(PCLK_PERI_DIV_MASK, div - 1));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_peri_get_rate(priv, clk_id);
}

static ulong rk3562_i2c_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, div, sel;
	ulong prate;

	switch (clk_id) {
	case CLK_PMU0_I2C0:
		con = readl(RK3562_PMU0_CLKSEL_CON(3));
		sel = FIELD_GET(CLK_PMU0_I2C0_SEL_MASK, con);
		div = FIELD_GET(CLK_PMU0_I2C0_DIV_MASK, con);
		if (sel == CLK_PMU0_I2C0_SEL_200M)
			prate = 200 * MHz;
		else if (sel == CLK_PMU0_I2C0_SEL_24M)
			prate = OSC_HZ;
		else
			prate = 32768;
		return DIV_TO_RATE(prate, div);
	case CLK_I2C1:
	case CLK_I2C2:
	case CLK_I2C3:
	case CLK_I2C4:
	case CLK_I2C5:
	case CLK_I2C:
		con = readl(RK3562_CLKSEL_CON(41));
		sel = FIELD_GET(CLK_I2C_SEL_MASK, con);
		if (sel == CLK_I2C_SEL_200M)
			return 200 * MHz;
		else if (sel == CLK_I2C_SEL_100M)
			return 100 * MHz;
		else if (sel == CLK_I2C_SEL_50M)
			return 50 * MHz;
		else
			return OSC_HZ;
	default:
		return -ENOENT;
	}
}

static ulong rk3562_i2c_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	u32 div, sel;

	switch (clk_id) {
	case CLK_PMU0_I2C0:
		if (rate == OSC_HZ) {
			sel = CLK_PMU0_I2C0_SEL_24M;
			div = 1;
		} else if (rate == 32768) {
			sel = CLK_PMU0_I2C0_SEL_32K;
			div = 1;
		} else {
			sel = CLK_PMU0_I2C0_SEL_200M;
			div = DIV_ROUND_UP(200 * MHz, rate);
			assert(div - 1 <= 31);
		}
		rk_clrsetreg(RK3562_PMU0_CLKSEL_CON(3), CLK_PMU0_I2C0_DIV_MASK,
			     FIELD_PREP(CLK_PMU0_I2C0_DIV_MASK, div - 1));
		rk_clrsetreg(RK3562_PMU0_CLKSEL_CON(3), CLK_PMU0_I2C0_SEL_MASK,
			     FIELD_PREP(CLK_PMU0_I2C0_SEL_MASK, sel));
		break;
	case CLK_I2C:
	case CLK_I2C2:
	case CLK_I2C3:
	case CLK_I2C4:
	case CLK_I2C5:
		if (rate == 200 * MHz)
			sel = CLK_I2C_SEL_200M;
		else if (rate == 100 * MHz)
			sel = CLK_I2C_SEL_100M;
		else if (rate == 50 * MHz)
			sel = CLK_I2C_SEL_50M;
		else
			sel = CLK_I2C_SEL_24M;
		rk_clrsetreg(RK3562_CLKSEL_CON(41), CLK_I2C_SEL_MASK,
			     FIELD_PREP(CLK_I2C_SEL_MASK, sel));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_i2c_get_rate(priv, clk_id);
}

/*
 *
 * rational_best_approximation(31415, 10000,
 *		(1 << 8) - 1, (1 << 5) - 1, &n, &d);
 *
 * you may look at given_numerator as a fixed point number,
 * with the fractional part size described in given_denominator.
 *
 * for theoretical background, see:
 * http://en.wikipedia.org/wiki/Continued_fraction
 */
static void rational_best_approximation(unsigned long given_numerator,
					unsigned long given_denominator,
					unsigned long max_numerator,
					unsigned long max_denominator,
					unsigned long *best_numerator,
					unsigned long *best_denominator)
{
	unsigned long n, d, n0, d0, n1, d1;

	n = given_numerator;
	d = given_denominator;
	n0 = 0;
	d1 = 0;
	n1 = 1;
	d0 = 1;
	for (;;) {
		unsigned long t, a;

		if (n1 > max_numerator || d1 > max_denominator) {
			n1 = n0;
			d1 = d0;
			break;
		}
		if (d == 0)
			break;
		t = d;
		a = n / d;
		d = n % d;
		n = t;
		t = n0 + a * n1;
		n0 = n1;
		n1 = t;
		t = d0 + a * d1;
		d0 = d1;
		d1 = t;
	}
	*best_numerator = n1;
	*best_denominator = d1;
}

static ulong rk3562_uart_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, sel, src_div, src_sel;
	unsigned long d, n, reg;
	ulong prate;

	switch (clk_id) {
	case SCLK_PMU1_UART0:
		con = readl(RK3562_PMU1_CLKSEL_CON(2));
		sel = FIELD_GET(CLK_PMU1_UART0_SEL_MASK, con);
		src_div = FIELD_GET(CLK_PMU1_UART0_SRC_DIV_MASK, con);
		if (sel == CLK_UART_SEL_SRC) {
			return DIV_TO_RATE(priv->cpll_hz, src_div);
		} else if (sel == CLK_UART_SEL_FRAC) {
			con = readl(RK3562_PMU1_CLKSEL_CON(3));
			n = FIELD_GET(CLK_UART_FRAC_NUMERATOR_MASK, con);
			d = FIELD_GET(CLK_UART_FRAC_DENOMINATOR_MASK, con);
			return DIV_TO_RATE(priv->cpll_hz, src_div) * n / d;
		} else {
			return OSC_HZ;
		}
	case SCLK_UART1:
		reg = 21;
		break;
	case SCLK_UART2:
		reg = 23;
		break;
	case SCLK_UART3:
		reg = 25;
		break;
	case SCLK_UART4:
		reg = 27;
		break;
	case SCLK_UART5:
		reg = 29;
		break;
	case SCLK_UART6:
		reg = 31;
		break;
	case SCLK_UART7:
		reg = 33;
		break;
	case SCLK_UART8:
		reg = 35;
		break;
	case SCLK_UART9:
		reg = 37;
		break;
	default:
		return -ENOENT;
	}

	con = readl(RK3562_PERI_CLKSEL_CON(reg));
	src_sel = FIELD_GET(CLK_UART_SRC_SEL_MASK, con);
	src_div = FIELD_GET(CLK_UART_SRC_DIV_MASK, con);
	sel = FIELD_GET(CLK_UART_SEL_MASK, con);

	if (src_sel == CLK_UART_SRC_SEL_GPLL)
		prate = priv->gpll_hz;
	else
		prate = priv->cpll_hz;

	if (sel == CLK_UART_SEL_SRC) {
		return DIV_TO_RATE(prate, src_div);
	} else if (sel == CLK_UART_SEL_FRAC) {
		con = readl(RK3562_PERI_CLKSEL_CON(reg + 1));
		n = FIELD_GET(CLK_UART_FRAC_NUMERATOR_MASK, con);
		d = FIELD_GET(CLK_UART_FRAC_DENOMINATOR_MASK, con);
		return DIV_TO_RATE(prate, src_div) * n / d;
	} else {
		return OSC_HZ;
	}
}

static ulong rk3562_uart_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				  ulong rate)
{
	u32 con, sel, src_div, src_sel;
	unsigned long d = 0, n = 0, reg;

	switch (clk_id) {
	case SCLK_PMU1_UART0:
		if ((priv->cpll_hz % rate) == 0) {
			src_div = DIV_ROUND_UP(priv->cpll_hz, rate);
			sel = CLK_UART_SEL_SRC;
		} else if (rate == OSC_HZ) {
			src_div = 2;
			sel = CLK_UART_SEL_24M;
		} else {
			src_div = 2;
			sel = CLK_UART_SEL_FRAC;
			rational_best_approximation(rate, priv->cpll_hz / src_div,
						    GENMASK(16 - 1, 0),
						    GENMASK(16 - 1, 0),
						    &n, &d);
		}

		rk_clrsetreg(RK3562_PMU1_CLKSEL_CON(2),
			     CLK_PMU1_UART0_SEL_MASK | CLK_PMU1_UART0_SRC_DIV_MASK,
			     FIELD_PREP(CLK_PMU1_UART0_SRC_DIV_MASK, src_div - 1) |
			     FIELD_PREP(CLK_PMU1_UART0_SEL_MASK, sel));
		if (n && d) {
			con = FIELD_PREP(CLK_UART_FRAC_NUMERATOR_MASK, n) |
			      FIELD_PREP(CLK_UART_FRAC_DENOMINATOR_MASK, d);
			writel(con, RK3562_PMU1_CLKSEL_CON(3));
		}

		return rk3562_uart_get_rate(priv, clk_id);
	case SCLK_UART1:
		reg = 21;
		break;
	case SCLK_UART2:
		reg = 23;
		break;
	case SCLK_UART3:
		reg = 25;
		break;
	case SCLK_UART4:
		reg = 27;
		break;
	case SCLK_UART5:
		reg = 29;
		break;
	case SCLK_UART6:
		reg = 31;
		break;
	case SCLK_UART7:
		reg = 33;
		break;
	case SCLK_UART8:
		reg = 35;
		break;
	case SCLK_UART9:
		reg = 37;
		break;
	default:
		return -ENOENT;
	}

	if ((priv->gpll_hz % rate) == 0) {
		src_sel = CLK_UART_SRC_SEL_GPLL;
		src_div = DIV_ROUND_UP(priv->gpll_hz, rate);
		sel = CLK_UART_SEL_SRC;
	} else if ((priv->cpll_hz % rate) == 0) {
		src_sel = CLK_UART_SRC_SEL_CPLL;
		src_div = DIV_ROUND_UP(priv->cpll_hz, rate);
		sel = CLK_UART_SEL_SRC;
	} else if (rate == OSC_HZ) {
		src_sel = CLK_UART_SRC_SEL_GPLL;
		src_div = 2;
		sel = CLK_UART_SEL_24M;
	} else {
		src_sel = CLK_UART_SRC_SEL_GPLL;
		src_div = 2;
		sel = CLK_UART_SEL_FRAC;
		rational_best_approximation(rate, priv->gpll_hz / src_div,
					    GENMASK(16 - 1, 0),
					    GENMASK(16 - 1, 0),
					    &n, &d);
	}

	rk_clrsetreg(RK3562_PERI_CLKSEL_CON(reg),
		     CLK_UART_SRC_SEL_MASK | CLK_UART_SRC_DIV_MASK |
		     CLK_UART_SEL_MASK,
		     FIELD_PREP(CLK_UART_SRC_SEL_MASK, src_sel) |
		     FIELD_PREP(CLK_UART_SRC_DIV_MASK, src_div - 1) |
		     FIELD_PREP(CLK_UART_SEL_MASK, sel));
	if (n && d) {
		con = FIELD_PREP(CLK_UART_FRAC_NUMERATOR_MASK, n) |
		      FIELD_PREP(CLK_UART_FRAC_DENOMINATOR_MASK, d);
		writel(con, RK3562_PERI_CLKSEL_CON(reg + 1));
	}

	return rk3562_uart_get_rate(priv, clk_id);
}

static ulong rk3562_pwm_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, div, sel;
	ulong prate;

	switch (clk_id) {
	case CLK_PMU1_PWM0:
		con = readl(RK3562_PMU1_CLKSEL_CON(4));
		sel = FIELD_GET(CLK_PMU1_PWM0_SEL_MASK, con);
		div = FIELD_GET(CLK_PMU1_PWM0_DIV_MASK, con);
		if (sel == CLK_PMU1_PWM0_SEL_200M)
			prate = 200 * MHz;
		else if (sel == CLK_PMU1_PWM0_SEL_24M)
			prate = OSC_HZ;
		else
			prate = 32768;
		return DIV_TO_RATE(prate, div);
	case CLK_PWM1_PERI:
		con = readl(RK3562_PERI_CLKSEL_CON(40));
		sel = FIELD_GET(CLK_PWM1_PERI_SEL_MASK, con);
		break;
	case CLK_PWM2_PERI:
		con = readl(RK3562_PERI_CLKSEL_CON(40));
		sel = FIELD_GET(CLK_PWM2_PERI_SEL_MASK, con);
		break;
	case CLK_PWM3_PERI:
		con = readl(RK3562_PERI_CLKSEL_CON(40));
		sel = FIELD_GET(CLK_PWM3_PERI_SEL_MASK, con);
		break;
	default:
		return -ENOENT;
	}

	if (sel == CLK_PWM_SEL_100M)
		return 100 * MHz;
	else if (sel == CLK_PWM_SEL_50M)
		return 50 * MHz;
	else
		return OSC_HZ;
}

static ulong rk3562_pwm_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	u32 div, sel;

	if (rate == 100 * MHz)
		sel = CLK_PWM_SEL_100M;
	else if (rate == 50 * MHz)
		sel = CLK_PWM_SEL_50M;
	else
		sel = CLK_PWM_SEL_24M;

	switch (clk_id) {
	case CLK_PMU1_PWM0:
		if (rate == OSC_HZ) {
			sel = CLK_PMU1_PWM0_SEL_24M;
			div = 1;
		} else if (rate == 32768) {
			sel = CLK_PMU1_PWM0_SEL_32K;
			div = 1;
		} else {
			sel = CLK_PMU1_PWM0_SEL_200M;
			div = DIV_ROUND_UP(200 * MHz, rate);
			assert(div - 1 <= 3);
		}
		rk_clrsetreg(RK3562_PMU1_CLKSEL_CON(4),
			     CLK_PMU1_PWM0_SEL_MASK | CLK_PMU1_PWM0_DIV_MASK,
			     FIELD_PREP(CLK_PMU1_PWM0_SEL_MASK, sel) |
			     FIELD_PREP(CLK_PMU1_PWM0_DIV_MASK, div - 1));
		break;
	case CLK_PWM1_PERI:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(40), CLK_PWM1_PERI_SEL_MASK,
			     FIELD_PREP(CLK_PWM1_PERI_SEL_MASK, sel));
		break;
	case CLK_PWM2_PERI:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(40), CLK_PWM2_PERI_SEL_MASK,
			     FIELD_PREP(CLK_PWM2_PERI_SEL_MASK, sel));
		break;
	case CLK_PWM3_PERI:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(40), CLK_PWM3_PERI_SEL_MASK,
			     FIELD_PREP(CLK_PWM3_PERI_SEL_MASK, sel));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_pwm_get_rate(priv, clk_id);
}

static ulong rk3562_spi_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, div, sel;
	ulong prate;

	switch (clk_id) {
	case CLK_PMU1_SPI0:
		con = readl(RK3562_PMU1_CLKSEL_CON(4));
		sel = FIELD_GET(CLK_PMU1_SPI0_SEL_MASK, con);
		div = FIELD_GET(CLK_PMU1_SPI0_DIV_MASK, con);
		if (sel == CLK_PMU1_SPI0_SEL_200M)
			prate = 200 * MHz;
		else if (sel == CLK_PMU1_SPI0_SEL_24M)
			prate = OSC_HZ;
		else
			prate = 32768;
		return DIV_TO_RATE(prate, div);
	case CLK_SPI1:
		con = readl(RK3562_PERI_CLKSEL_CON(20));
		sel = FIELD_GET(CLK_SPI1_SEL_MASK, con);
		break;
	case CLK_SPI2:
		con = readl(RK3562_PERI_CLKSEL_CON(20));
		sel = FIELD_GET(CLK_SPI2_SEL_MASK, con);
		break;
	default:
		return -ENOENT;
	}

	if (sel == CLK_SPI_SEL_200M)
		return 200 * MHz;
	else if (sel == CLK_SPI_SEL_100M)
		return 100 * MHz;
	else if (sel == CLK_SPI_SEL_50M)
		return 50 * MHz;
	else
		return OSC_HZ;
}

static ulong rk3562_spi_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				 ulong rate)
{
	u32 div, sel;

	if (rate == 200 * MHz)
		sel = CLK_SPI_SEL_200M;
	else if (rate == 100 * MHz)
		sel = CLK_SPI_SEL_100M;
	else if (rate == 50 * MHz)
		sel = CLK_SPI_SEL_50M;
	else
		sel = CLK_SPI_SEL_24M;

	switch (clk_id) {
	case CLK_PMU1_SPI0:
		if (rate == 200 * MHz) {
			sel = CLK_PMU1_SPI0_SEL_200M;
			div = 1;
		} else if (rate == OSC_HZ) {
			sel = CLK_PMU1_SPI0_SEL_24M;
			div = 1;
		} else if (rate == 32768) {
			sel = CLK_PMU1_SPI0_SEL_32K;
			div = 1;
		} else {
			sel = CLK_PMU1_SPI0_SEL_200M;
			div = DIV_ROUND_UP(200 * MHz, rate);
			assert(div - 1 <= 3);
		}
		rk_clrsetreg(RK3562_PMU1_CLKSEL_CON(4),
			     CLK_PMU1_SPI0_SEL_MASK | CLK_PMU1_SPI0_DIV_MASK,
			     FIELD_PREP(CLK_PMU1_SPI0_SEL_MASK, sel) |
			     FIELD_PREP(CLK_PMU1_SPI0_DIV_MASK, div - 1));
		break;
	case CLK_SPI1:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(20), CLK_SPI1_SEL_MASK,
			     FIELD_PREP(CLK_SPI1_SEL_MASK, sel));
		break;
	case CLK_SPI2:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(20), CLK_SPI2_SEL_MASK,
			     FIELD_PREP(CLK_SPI2_SEL_MASK, sel));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_spi_get_rate(priv, clk_id);
}

static ulong rk3562_tsadc_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, div;

	switch (clk_id) {
	case CLK_TSADC_TSEN:
		con = readl(RK3562_CLKSEL_CON(43));
		div = FIELD_GET(CLK_TSADC_TSEN_DIV_MASK, con);
		return DIV_TO_RATE(OSC_HZ, div);
	case CLK_TSADC:
		con = readl(RK3562_CLKSEL_CON(43));
		div = FIELD_GET(CLK_TSADC_DIV_MASK, con);
		return DIV_TO_RATE(OSC_HZ, div);
	default:
		return -ENOENT;
	}
}

static ulong rk3562_tsadc_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				   ulong rate)
{
	u32 div = DIV_ROUND_UP(OSC_HZ, rate);

	switch (clk_id) {
	case CLK_TSADC_TSEN:
		rk_clrsetreg(RK3562_CLKSEL_CON(43), CLK_TSADC_TSEN_DIV_MASK,
			     FIELD_PREP(CLK_TSADC_TSEN_DIV_MASK, div - 1));
		break;
	case CLK_TSADC:
		rk_clrsetreg(RK3562_CLKSEL_CON(43), CLK_TSADC_DIV_MASK,
			     FIELD_PREP(CLK_TSADC_DIV_MASK, div - 1));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_tsadc_get_rate(priv, clk_id);
}

static ulong rk3562_saradc_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, div;

	switch (clk_id) {
	case CLK_SARADC_VCCIO156:
		con = readl(RK3562_CLKSEL_CON(44));
		div = FIELD_GET(CLK_SARADC_VCCIO156_DIV_MASK, con);
		break;
	case CLK_SARADC:
		con = readl(RK3562_PERI_CLKSEL_CON(46));
		div = FIELD_GET(CLK_SARADC_DIV_MASK, con);
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(OSC_HZ, div);
}

static ulong rk3562_saradc_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				    ulong rate)
{
	u32 div = DIV_ROUND_UP(OSC_HZ, rate);

	switch (clk_id) {
	case CLK_SARADC_VCCIO156:
		rk_clrsetreg(RK3562_CLKSEL_CON(44), CLK_SARADC_VCCIO156_DIV_MASK,
			     FIELD_PREP(CLK_SARADC_VCCIO156_DIV_MASK, div - 1));
		break;
	case CLK_SARADC:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(46), CLK_SARADC_DIV_MASK,
			     FIELD_PREP(CLK_SARADC_DIV_MASK, div - 1));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_saradc_get_rate(priv, clk_id);
}

static ulong rk3562_sfc_get_rate(struct rk3562_clk_priv *priv)
{
	u32 con, div, sel;
	ulong prate;

	con = readl(RK3562_PERI_CLKSEL_CON(20));
	sel = FIELD_GET(SCLK_SFC_SEL_MASK, con);
	div = FIELD_GET(SCLK_SFC_DIV_MASK, con);

	if (sel == SCLK_SFC_SRC_SEL_GPLL)
		prate = priv->gpll_hz;
	else if (sel == SCLK_SFC_SRC_SEL_CPLL)
		prate = priv->cpll_hz;
	else
		prate = OSC_HZ;

	return DIV_TO_RATE(prate, div);
}

static ulong rk3562_sfc_set_rate(struct rk3562_clk_priv *priv, ulong rate)
{
	int div, sel;

	if ((OSC_HZ % rate) == 0) {
		sel = SCLK_SFC_SRC_SEL_24M;
		div = DIV_ROUND_UP(OSC_HZ, rate);
	} else if ((priv->cpll_hz % rate) == 0) {
		sel = SCLK_SFC_SRC_SEL_CPLL;
		div = DIV_ROUND_UP(priv->cpll_hz, rate);
	} else {
		sel = SCLK_SFC_SRC_SEL_GPLL;
		div = DIV_ROUND_UP(priv->gpll_hz, rate);
	}

	rk_clrsetreg(RK3562_PERI_CLKSEL_CON(20),
		     SCLK_SFC_SEL_MASK | SCLK_SFC_DIV_MASK,
		     FIELD_PREP(SCLK_SFC_SEL_MASK, sel) |
		     FIELD_PREP(SCLK_SFC_DIV_MASK, div - 1));

	return rk3562_sfc_get_rate(priv);
}

static ulong rk3562_emmc_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, div, sel;
	ulong prate;

	switch (clk_id) {
	case CCLK_EMMC:
		con = readl(RK3562_PERI_CLKSEL_CON(18));
		sel = FIELD_GET(CCLK_EMMC_SEL_MASK, con);
		div = FIELD_GET(CCLK_EMMC_DIV_MASK, con);
		if (sel == CCLK_EMMC_SEL_GPLL)
			prate = priv->gpll_hz;
		else if (sel == CCLK_EMMC_SEL_CPLL)
			prate = priv->cpll_hz;
		else if (sel == CCLK_EMMC_SEL_HPLL)
			prate = priv->hpll_hz;
		else
			prate = OSC_HZ;
		break;
	case BCLK_EMMC:
		con = readl(RK3562_PERI_CLKSEL_CON(19));
		sel = FIELD_GET(BCLK_EMMC_SEL_MASK, con);
		div = FIELD_GET(BCLK_EMMC_DIV_MASK, con);
		if (sel == BCLK_EMMC_SEL_GPLL)
			prate = priv->gpll_hz;
		else
			prate = priv->cpll_hz;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(prate, div);
}

static ulong rk3562_emmc_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				  ulong rate)
{
	u32 div, sel;

	switch (clk_id) {
	case CCLK_EMMC:
		if ((OSC_HZ % rate) == 0) {
			sel = CCLK_EMMC_SEL_24M;
			div = DIV_ROUND_UP(OSC_HZ, rate);
		} else if ((priv->cpll_hz % rate) == 0) {
			sel = CCLK_EMMC_SEL_CPLL;
			div = DIV_ROUND_UP(priv->cpll_hz, rate);
		} else if ((priv->hpll_hz % rate) == 0) {
			sel = CCLK_EMMC_SEL_HPLL;
			div = DIV_ROUND_UP(priv->hpll_hz, rate);
		} else {
			sel = CCLK_EMMC_SEL_GPLL;
			div = DIV_ROUND_UP(priv->gpll_hz, rate);
		}
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(18),
			     CCLK_EMMC_SEL_MASK | CCLK_EMMC_DIV_MASK,
			     FIELD_PREP(CCLK_EMMC_SEL_MASK, sel) |
			     FIELD_PREP(CCLK_EMMC_DIV_MASK, div - 1));
		break;
	case BCLK_EMMC:
		if ((priv->cpll_hz % rate) == 0) {
			sel = BCLK_EMMC_SEL_CPLL;
			div = DIV_ROUND_UP(priv->cpll_hz, rate);
		} else {
			sel = BCLK_EMMC_SEL_GPLL;
			div = DIV_ROUND_UP(priv->gpll_hz, rate);
		}
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(19),
			     BCLK_EMMC_SEL_MASK | BCLK_EMMC_DIV_MASK,
			     FIELD_PREP(BCLK_EMMC_SEL_MASK, sel) |
			     FIELD_PREP(BCLK_EMMC_DIV_MASK, div - 1));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_emmc_get_rate(priv, clk_id);
}

static ulong rk3562_sdmmc_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, div, sel;
	ulong prate;

	switch (clk_id) {
	case HCLK_SDMMC0:
	case CCLK_SDMMC0:
	case SCLK_SDMMC0_SAMPLE:
		con = readl(RK3562_PERI_CLKSEL_CON(16));
		sel = FIELD_GET(CCLK_SDMMC0_SEL_MASK, con);
		div = FIELD_GET(CCLK_SDMMC0_DIV_MASK, con);
		break;
	case HCLK_SDMMC1:
	case CCLK_SDMMC1:
	case SCLK_SDMMC1_SAMPLE:
		con = readl(RK3562_PERI_CLKSEL_CON(17));
		sel = FIELD_GET(CCLK_SDMMC1_SEL_MASK, con);
		div = FIELD_GET(CCLK_SDMMC1_DIV_MASK, con);
		break;
	default:
		return -ENOENT;
	}

	if (sel == CCLK_SDMMC_SEL_GPLL)
		prate = priv->gpll_hz;
	else if (sel == CCLK_SDMMC_SEL_CPLL)
		prate = priv->cpll_hz;
	else if (sel == CCLK_SDMMC_SEL_HPLL)
		prate = priv->hpll_hz;
	else
		prate = OSC_HZ;

	return DIV_TO_RATE(prate, div);
}

static ulong rk3562_sdmmc_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				   ulong rate)
{
	u32 div, sel;

	if ((OSC_HZ % rate) == 0) {
		sel = CCLK_SDMMC_SEL_24M;
		div = DIV_ROUND_UP(OSC_HZ, rate);
	} else if ((priv->cpll_hz % rate) == 0) {
		sel = CCLK_SDMMC_SEL_CPLL;
		div = DIV_ROUND_UP(priv->cpll_hz, rate);
	} else if ((priv->hpll_hz % rate) == 0) {
		sel = CCLK_SDMMC_SEL_HPLL;
		div = DIV_ROUND_UP(priv->hpll_hz, rate);
	} else {
		sel = CCLK_SDMMC_SEL_GPLL;
		div = DIV_ROUND_UP(priv->gpll_hz, rate);
	}

	switch (clk_id) {
	case HCLK_SDMMC0:
	case CCLK_SDMMC0:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(16),
			     CCLK_SDMMC0_SEL_MASK | CCLK_SDMMC0_DIV_MASK,
			     FIELD_PREP(CCLK_SDMMC0_SEL_MASK, sel) |
			     FIELD_PREP(CCLK_SDMMC0_DIV_MASK, div - 1));
		break;
	case HCLK_SDMMC1:
	case CCLK_SDMMC1:
		rk_clrsetreg(RK3562_PERI_CLKSEL_CON(17),
			     CCLK_SDMMC1_SEL_MASK | CCLK_SDMMC1_DIV_MASK,
			     FIELD_PREP(CCLK_SDMMC1_SEL_MASK, sel) |
			     FIELD_PREP(CCLK_SDMMC1_DIV_MASK, div - 1));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_sdmmc_get_rate(priv, clk_id);
}

static ulong rk3562_gmac_get_rate(struct rk3562_clk_priv *priv, ulong clk_id)
{
	u32 con, sel, div;
	ulong prate;

	switch (clk_id) {
	case CLK_GMAC_125M_CRU_I:
		con = readl(RK3562_CLKSEL_CON(45));
		sel = FIELD_GET(CLK_GMAC_125M_SEL_MASK, con);
		if (sel == CLK_GMAC_125M)
			return 125000000;
		else
			return OSC_HZ;
	case CLK_GMAC_50M_CRU_I:
		con = readl(RK3562_CLKSEL_CON(45));
		sel = FIELD_GET(CLK_GMAC_50M_SEL_MASK, con);
		if (sel == CLK_GMAC_50M)
			return 50000000;
		else
			return OSC_HZ;
	case CLK_MAC100_50M_MATRIX:
		con = readl(RK3562_CLKSEL_CON(47));
		sel = FIELD_GET(CLK_GMAC_50M_SEL_MASK, con);
		if (sel == CLK_GMAC_50M)
			return 50000000;
		else
			return OSC_HZ;
	case CLK_GMAC_ETH_OUT2IO:
		con = readl(RK3562_CLKSEL_CON(46));
		sel = FIELD_GET(CLK_GMAC_ETH_OUT2IO_SEL_MASK, con);
		div = FIELD_GET(CLK_GMAC_ETH_OUT2IO_DIV_MASK, con);
		if (sel == CLK_GMAC_ETH_OUT2IO_GPLL)
			prate = priv->gpll_hz;
		else
			prate = priv->cpll_hz;
		break;
	default:
		return -ENOENT;
	}

	return DIV_TO_RATE(prate, div);
}

static ulong rk3562_gmac_set_rate(struct rk3562_clk_priv *priv, ulong clk_id,
				  ulong rate)
{
	u32 div, sel;

	switch (clk_id) {
	case CLK_GMAC_125M_CRU_I:
		if (rate == 125000000)
			sel = CLK_GMAC_125M;
		else
			sel = CLK_GMAC_24M;
		rk_clrsetreg(RK3562_CLKSEL_CON(45), CLK_GMAC_125M_SEL_MASK,
			     FIELD_PREP(CLK_GMAC_125M_SEL_MASK, sel));
		break;
	case CLK_GMAC_50M_CRU_I:
		if (rate == 50000000)
			sel = CLK_GMAC_50M;
		else
			sel = CLK_GMAC_24M;
		rk_clrsetreg(RK3562_CLKSEL_CON(45), CLK_GMAC_50M_SEL_MASK,
			     FIELD_PREP(CLK_GMAC_50M_SEL_MASK, sel));
		break;
	case CLK_MAC100_50M_MATRIX:
		if (rate == 50000000)
			sel = CLK_GMAC_50M;
		else
			sel = CLK_GMAC_24M;
		rk_clrsetreg(RK3562_CLKSEL_CON(47), CLK_GMAC_50M_SEL_MASK,
			     FIELD_PREP(CLK_GMAC_50M_SEL_MASK, sel));
		break;
	case CLK_GMAC_ETH_OUT2IO:
		if ((priv->cpll_hz % rate) == 0) {
			div = DIV_ROUND_UP(priv->cpll_hz, rate);
			sel = CLK_GMAC_ETH_OUT2IO_CPLL;
		} else {
			div = DIV_ROUND_UP(priv->gpll_hz, rate);
			sel = CLK_GMAC_ETH_OUT2IO_GPLL;
		}
		rk_clrsetreg(RK3562_CLKSEL_CON(46),
			     CLK_GMAC_ETH_OUT2IO_SEL_MASK | CLK_GMAC_ETH_OUT2IO_DIV_MASK,
			     FIELD_PREP(CLK_GMAC_ETH_OUT2IO_SEL_MASK, sel) |
			     FIELD_PREP(CLK_GMAC_ETH_OUT2IO_DIV_MASK, div - 1));
		break;
	default:
		return -ENOENT;
	}

	return rk3562_gmac_get_rate(priv, clk_id);
}

static ulong rk3562_clk_get_rate(struct clk *clk)
{
	struct rk3562_clk_priv *priv = dev_get_priv(clk->dev);
	ulong rate = 0;

	switch (clk->id) {
	case ARMCLK:
	case PLL_APLL:
	case PLL_CPLL:
	case PLL_DPLL:
	case PLL_GPLL:
	case PLL_HPLL:
	case PLL_VPLL:
		rate = rk3562_pll_get_rate(priv, clk->id);
		break;
	case ACLK_BUS:
	case HCLK_BUS:
	case PCLK_BUS:
		rate = rk3562_bus_get_rate(priv, clk->id);
		break;
	case ACLK_PERI:
	case HCLK_PERI:
	case PCLK_PERI:
		rate = rk3562_peri_get_rate(priv, clk->id);
		break;
	case CLK_PMU0_I2C0:
	case CLK_I2C1:
	case CLK_I2C2:
	case CLK_I2C3:
	case CLK_I2C4:
	case CLK_I2C5:
	case CLK_I2C:
		rate = rk3562_i2c_get_rate(priv, clk->id);
		break;
	case SCLK_PMU1_UART0:
	case SCLK_UART1:
	case SCLK_UART2:
	case SCLK_UART3:
	case SCLK_UART4:
	case SCLK_UART5:
	case SCLK_UART6:
	case SCLK_UART7:
	case SCLK_UART8:
	case SCLK_UART9:
		rate = rk3562_uart_get_rate(priv, clk->id);
		break;
	case CLK_PMU1_PWM0:
	case CLK_PWM1_PERI:
	case CLK_PWM2_PERI:
	case CLK_PWM3_PERI:
		rate = rk3562_pwm_get_rate(priv, clk->id);
		break;
	case CLK_PMU1_SPI0:
	case CLK_SPI1:
	case CLK_SPI2:
		rate = rk3562_spi_get_rate(priv, clk->id);
		break;
	case CLK_TSADC:
	case CLK_TSADC_TSEN:
		rate = rk3562_tsadc_get_rate(priv, clk->id);
		break;
	case CLK_SARADC:
	case CLK_SARADC_VCCIO156:
		rate = rk3562_saradc_get_rate(priv, clk->id);
		break;
	case SCLK_SFC:
		rate = rk3562_sfc_get_rate(priv);
		break;
	case CCLK_EMMC:
	case BCLK_EMMC:
		rate = rk3562_emmc_get_rate(priv, clk->id);
		break;
	case HCLK_SDMMC0:
	case HCLK_SDMMC1:
	case CCLK_SDMMC0:
	case CCLK_SDMMC1:
	case SCLK_SDMMC0_SAMPLE:
	case SCLK_SDMMC1_SAMPLE:
		rate = rk3562_sdmmc_get_rate(priv, clk->id);
		break;
	case CLK_GMAC_125M_CRU_I:
	case CLK_GMAC_50M_CRU_I:
	case CLK_GMAC_ETH_OUT2IO:
	case CLK_MAC100_50M_MATRIX:
		rate = rk3562_gmac_get_rate(priv, clk->id);
		break;
	case CLK_USB3OTG_REF:
	case CLK_WDTNS:
		rate = OSC_HZ;
		break;
	default:
		log_debug("unsupported clk id=%ld\n", clk->id);
		return -ENOENT;
	}

	return rate;
};

static ulong rk3562_clk_set_rate(struct clk *clk, ulong rate)
{
	struct rk3562_clk_priv *priv = dev_get_priv(clk->dev);
	ulong ret = 0;

	switch (clk->id) {
	case ARMCLK:
	case PLL_APLL:
	case PLL_CPLL:
	case PLL_DPLL:
	case PLL_GPLL:
	case PLL_HPLL:
	case PLL_VPLL:
		ret = rk3562_pll_set_rate(priv, clk->id, rate);
		break;
	case ACLK_BUS:
	case HCLK_BUS:
	case PCLK_BUS:
		ret = rk3562_bus_set_rate(priv, clk->id, rate);
		break;
	case ACLK_PERI:
	case HCLK_PERI:
	case PCLK_PERI:
		ret = rk3562_peri_set_rate(priv, clk->id, rate);
		break;
	case CLK_PMU0_I2C0:
	case CLK_I2C1:
	case CLK_I2C2:
	case CLK_I2C3:
	case CLK_I2C4:
	case CLK_I2C5:
	case CLK_I2C:
		ret = rk3562_i2c_set_rate(priv, clk->id, rate);
		break;
	case SCLK_PMU1_UART0:
	case SCLK_UART1:
	case SCLK_UART2:
	case SCLK_UART3:
	case SCLK_UART4:
	case SCLK_UART5:
	case SCLK_UART6:
	case SCLK_UART7:
	case SCLK_UART8:
	case SCLK_UART9:
		ret = rk3562_uart_set_rate(priv, clk->id, rate);
		break;
	case CLK_PMU1_PWM0:
	case CLK_PWM1_PERI:
	case CLK_PWM2_PERI:
	case CLK_PWM3_PERI:
		ret = rk3562_pwm_set_rate(priv, clk->id, rate);
		break;
	case CLK_PMU1_SPI0:
	case CLK_SPI1:
	case CLK_SPI2:
		ret = rk3562_spi_set_rate(priv, clk->id, rate);
		break;
	case CLK_TSADC:
	case CLK_TSADC_TSEN:
		ret = rk3562_tsadc_set_rate(priv, clk->id, rate);
		break;
	case CLK_SARADC:
	case CLK_SARADC_VCCIO156:
		ret = rk3562_saradc_set_rate(priv, clk->id, rate);
		break;
	case SCLK_SFC:
		ret = rk3562_sfc_set_rate(priv, rate);
		break;
	case CCLK_EMMC:
	case BCLK_EMMC:
		ret = rk3562_emmc_set_rate(priv, clk->id, rate);
		break;
	case HCLK_SDMMC0:
	case HCLK_SDMMC1:
	case CCLK_SDMMC0:
	case CCLK_SDMMC1:
		ret = rk3562_sdmmc_set_rate(priv, clk->id, rate);
		break;
	case CLK_GMAC_125M_CRU_I:
	case CLK_GMAC_50M_CRU_I:
	case CLK_GMAC_ETH_OUT2IO:
	case CLK_MAC100_50M_MATRIX:
		ret = rk3562_gmac_set_rate(priv, clk->id, rate);
		break;
	case CLK_USB3OTG_REF:
	case CLK_WDTNS:
		ret = OSC_HZ;
		break;
	default:
		log_debug("unsupported clk id=%ld rate=%ld\n", clk->id, rate);
		return -ENOENT;
	}

	return ret;
};

static int rk3562_clk_enable(struct clk *clk)
{
	switch (clk->id) {
	case HCLK_RK_RNG_NS:
		rk_clrreg(RK3562_PERI_CLKGATE_CON(12), HCLK_RK_RNG_NS_EN);
		break;
	case HCLK_TRNG_NS:
		rk_clrreg(RK3562_PERI_CLKGATE_CON(12), HCLK_TRNG_NS_EN);
		break;
	default:
		return -ENOSYS;
	}

	return 0;
}

static int rk3562_clk_disable(struct clk *clk)
{
	switch (clk->id) {
	case HCLK_RK_RNG_NS:
		rk_setreg(RK3562_PERI_CLKGATE_CON(12), HCLK_RK_RNG_NS_EN);
		break;
	case HCLK_TRNG_NS:
		rk_setreg(RK3562_PERI_CLKGATE_CON(12), HCLK_TRNG_NS_EN);
		break;
	default:
		return -ENOSYS;
	}

	return 0;
}

static struct clk_ops rk3562_clk_ops = {
	.get_rate = rk3562_clk_get_rate,
	.set_rate = rk3562_clk_set_rate,
	.enable = rk3562_clk_enable,
	.disable = rk3562_clk_disable,
};

static int rk3562_clk_probe(struct udevice *dev)
{
	struct rk3562_clk_priv *priv = dev_get_priv(dev);
	int ret;

	if (priv->gpll_hz != GPLL_HZ)
		rk3562_pll_set_rate(priv, PLL_GPLL, GPLL_HZ);

	if (priv->cpll_hz != CPLL_HZ)
		rk3562_pll_set_rate(priv, PLL_CPLL, CPLL_HZ);

	if (priv->hpll_hz != HPLL_HZ)
		rk3562_pll_set_rate(priv, PLL_HPLL, HPLL_HZ);

	/* Process 'assigned-{clocks/clock-parents/clock-rates}' properties */
	ret = clk_set_defaults(dev, 1);
	if (ret)
		log_debug("clk_set_defaults failed: ret=%d\n", ret);

	return 0;
}

static int rk3562_clk_bind(struct udevice *dev)
{
	struct udevice *sys_child;
	struct sysreset_reg *priv;
	int ret;

	/* The reset driver does not have a device node, so bind it here */
	ret = device_bind_driver(dev, "rockchip_sysreset", "sysreset",
				 &sys_child);
	if (ret) {
		log_debug("Warning: No sysreset driver: ret=%d\n", ret);
	} else {
		priv = malloc(sizeof(struct sysreset_reg));
		priv->glb_srst_fst_value = RK3562_GLB_SRST_FST;
		priv->glb_srst_snd_value = RK3562_GLB_SRST_SND;
		dev_set_priv(sys_child, priv);
	}

	return 0;
}

static const struct udevice_id rk3562_clk_ids[] = {
	{ .compatible = "rockchip,rk3562-cru" },
	{ }
};

U_BOOT_DRIVER(rockchip_rk3562_cru) = {
	.name		= "rockchip_rk3562_cru",
	.id		= UCLASS_CLK,
	.of_match	= rk3562_clk_ids,
	.priv_auto	= sizeof(struct rk3562_clk_priv),
	.ops		= &rk3562_clk_ops,
	.bind		= rk3562_clk_bind,
	.probe		= rk3562_clk_probe,
};

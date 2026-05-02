/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd
 * Copyright (c) 2024-2026, Pavel Golikov <paullo612@ya.ru>
 */

#ifndef _ASM_ARCH_SDRAM_RK3568_H
#define _ASM_ARCH_SDRAM_RK3568_H

#include <asm/arch-rockchip/dram_spec_timing.h>
#include <asm/arch-rockchip/sdram.h>
#include <asm/arch-rockchip/sdram_common.h>
#include <asm/arch-rockchip/sdram_msch.h>
#include <asm/arch-rockchip/sdram_pctl_px30.h>
#include <asm/arch-rockchip/sdram_phy_rk3568.h>

#define PATTERN				(0x5aa5f00f)

#define DPLL_MODE(n)			((0x3 << (2 + 16)) | ((n) << 2))

/* CRU_CLKSEL_CON10 */
#define CLK_MSCH_DIV_MASK		(3)
#define CLK_MSCH_DIV_SHIFT		0

/* CRU_DPLL_CON0 */
#define RK3036_PLLCON0_FBDIV_MASK	0xfff
#define RK3036_PLLCON0_FBDIV_SHIFT	0
#define RK3036_PLLCON0_POSTDIV1_MASK	(0x7 << 12)
#define RK3036_PLLCON0_POSTDIV1_SHIFT	12

/* CRU_DPLL_CON1 */
#define RK3036_PLLCON1_REFDIV_MASK	0x3f
#define RK3036_PLLCON1_REFDIV_SHIFT	0
#define RK3036_PLLCON1_POSTDIV2_MASK	(0x7 << 6)
#define RK3036_PLLCON1_POSTDIV2_SHIFT	6
#define RK3036_PLLCON1_DSMPD_MASK	(0x1 << 12)
#define RK3036_PLLCON1_DSMPD_SHIFT	12
#define RK3036_PLLCON1_PWRDOWN_SHIFT	13
#define RK3568_PLLCON1_LOCK(n)		(((n) >> 10) & 0x1)

/* CRU_DPLL_CON2 */
#define RK3036_PLLCON2_FRAC_MASK	0xffffff
#define RK3036_PLLCON2_FRAC_SHIFT	0

/* CRU_DPLL_CON3 */
#define SSMOD_SPREAD(n)			((0x1f << (8 + 16)) | ((n) << 8))
#define SSMOD_DIVVAL(n)			((0xf << (4 + 16)) | ((n) << 4))
#define SSMOD_DOWNSPREAD(n)		((0x1 << (3 + 16)) | ((n) << 3))
#define SSMOD_RESET(n)			((0x1 << (2 + 16)) | ((n) << 2))
#define SSMOD_DIS_SSCG(n)		((0x1 << (1 + 16)) | ((n) << 1))
#define SSMOD_BP(n)			((0x1 << (0 + 16)) | ((n) << 0))

/* DDR_GRF_CON0 */
#define DFI_INIT_START			BIT(2)
#define DFI_INIT_START_BY_GRF_EN	BIT(1)

/* DDR_GRF_SPLIT_CON */
#define SPLIT_MODE_MASK			(0x3)
#define SPLIT_MODE_OFFSET		(9)
#define SPLIT_BYPASS_MASK		(1)
#define SPLIT_BYPASS_OFFSET		(8)
#define SPLIT_SIZE_MASK			(0xff)
#define SPLIT_SIZE_OFFSET		(0)

/* SGRF_SOC_CON5 */
#define SGRF_SOC_CON5			0x14
#define UPCTL2_SRSTN_REQ_MASK		BIT(8)
#define UPCTL2_SRSTN_REQ_SHIFT		8
#define UPCTL2_ASRSTN_REQ_MASK		BIT(9)
#define UPCTL2_ASRSTN_REQ_SHIFT		9
#define UPCTL2_PSRSTN_REQ_MASK		BIT(11)
#define UPCTL2_PSRSTN_REQ_SHIFT		11

/* SCRU_SOFTRST_CON02 */
#define SCRU_SOFTRST_CON02		0x208
#define PRESETN_DDR_UPCTL_MASK		BIT(1)
#define PRESETN_DDR_UPCTL_SHIFT		1

/* CRU_SOFTRST_CON27 */
#define PRESETN_DDRPHY_MASK		BIT(7)
#define PRESETN_DDRPHY_SHIFT		7
#define RESETN_DDRPHY_MASK		BIT(8)
#define RESETN_DDRPHY_SHIFT		8

/* DDR_GRF_CON3 */
#define DQ_SWAP_EN_MASK			BIT(7)
#define DQ_SWAP_EN_SHIFT		7
#define DQ_SWAP_SEL0_MASK		(3 << 8)
#define DQ_SWAP_SEL0_SHIFT		8
#define DQ_SWAP_SEL1_MASK		(3 << 10)
#define DQ_SWAP_SEL1_SHIFT		10
#define DQ_SWAP_SEL2_MASK		(3 << 12)
#define DQ_SWAP_SEL2_SHIFT		12
#define DQ_SWAP_SEL3_MASK		(3 << 14)
#define DQ_SWAP_SEL3_SHIFT		14

/* PMUGRF */
#define PMUGRF_CON_DDRPHY_BUFFEREN_MASK		(0x3 << 12)
#define PMUGRF_CON_DDRPHY_BUFFEREN_SHIFT	12
#define PMUGRF_CON_DDRPHY_BUFFEREN_EN		0x1
#define PMUGRF_CON_DDRPHY_BUFFEREN_DIS		0x2

struct rk3568_ddrgrf {
	u32 ddr_grf_con[5];
	u32 grf_ddrsplit_con;
	u32 reserved1[(0x20 - 0x14) / 4 - 1];
	u32 ddr_grf_lp_con;
	u32 reserved2[(0x100 - 0x20) / 4 - 1];
	u32 ddr_grf_status[13];
};

struct msch_regs {
	u32 coreid;
	u32 revisionid;
	u32 deviceconf;
	u32 devicesize;
	u32 ddrtiminga0;
	u32 ddrtimingb0;
	u32 ddrtimingc0;
	u32 ddr4timing;
	u32 devtodev0;
	u32 ddrmode;
	u32 reserved;
	u32 agingx0;
	u32 aging0;
	u32 aging1;
	u32 aging2;
	u32 aging3;
};

struct sdram_msch_timings {
	union noc_ddrtiminga0 ddrtiminga0;
	union noc_ddrtimingb0 ddrtimingb0;
	union noc_ddrtimingc0 ddrtimingc0;
	union noc_ddr4timing ddr4timing;
	union noc_devtodev_rv1126 devtodev0;
	union noc_ddrmode ddrmode;

	u32 agingx0;
	u32 aging0;
	u32 aging1;
	u32 aging2;
	u32 aging3;
};

struct rk3568_sdram_channel {
	struct sdram_cap_info cap_info;
	struct sdram_msch_timings noc_timings;
};

struct rk3568_sdram_params {
	struct rk3568_sdram_channel ch;
	struct sdram_base_params base;
	struct ddr_pctl_regs pctl_regs;
	struct rk3568_ddr_phy_regs phy_regs;
};

struct rk3568_fsp_param {
	u32 flag;
	u32 freq_mhz;

	/* dram size */
	u32 dq_odt;
	u32 ca_odt;
	u32 ds_pdds;
	u32 vref_ca[2];
	u32 vref_dq[2];

	/* phy side */
	u32 wr_dq_drv;
	u32 wr_ca_drv;
	u32 wr_ckcs_drv;
	u32 rd_odt;
	u32 rd_odt_up_en;
	u32 rd_odt_down_en;
	u32 vref_inner;
	u32 vref_out;
	u32 lp4_drv_pd_en;

	u32 ca_prebit_skew[32];
	u32 reserved0[28];

	struct sdram_msch_timings noc_timings;
	u32 reserved1;
};

#define MAX_IDX			(4)
#define FSP_FLAG		(0xfead0002)
#define SHARE_MEM_BASE		(0x100000)
/*
 * Borrow share memory space to temporarily store FSP params.
 * In the stage of DDR init write FSP params to this space.
 * In the stage of trust init move FSP params to SRAM space from share memory space.
 */
#define FSP_PARAM_STORE_ADDR	(SHARE_MEM_BASE)

#endif /* _ASM_ARCH_SDRAM_RK3568_H */

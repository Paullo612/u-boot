/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * Copyright (c) 2024-2026, Pavel Golikov <paullo612@ya.ru>
 */

#ifndef _ASM_ARCH_SDRAM_PHY_RK3568_H
#define _ASM_ARCH_SDRAM_PHY_RK3568_H

#include <asm/arch-rockchip/sdram_phy_ron_rtt_rk3568.h>

/* DDRPHY_REG0 */
#define CHANNEL_EN_MASK			(0x1f << 8)
#define CHANNEL_EN_SHIFT		8
#define RANK4_EN_MASK			BIT(20)
#define RANK4_EN_SHIFT			20

/* DDRPHY_REG1 */
#define START_CALIB			BIT(0)
#define CALCS_SEL_MASK			(0xf << 2)
#define CALCS_SEL_SHIFT			2
#define WL_ENABLE			BIT(6)
#define WLCS_SEL_MASK			(0xf << 8)
#define WLCS_SEL_SHIFT			8
#define WL_LOADMODE_MASK		(0xffff << 16)
#define WL_LOADMODE_SHIFT		16

/* DDRPHY_REG3 */
#define CL_FRE_OPN_MASK(n)		(0x1f << (24 - 8 * (n)))
#define CL_FRE_OPN_SHIFT(n)		(24 - 8 * (n))

/* DDRPHY_REG4 */
#define CWL_FRE_OPN_MASK(n)		(0x1f << (24 - 8 * (n)))
#define CWL_FRE_OPN_SHIFT(n)		(24 - 8 * (n))

/* DDRPHY_REG5 */
#define FB1XCLK_INVDELA_MASK		(0x1f << 24)
#define FB1XCLK_INVDELA_SHIFT		24

/* DDRPHY_REG8 */
#define CAT_RANK_NUM_TWO_RANKS		0xc
#define CAT_RANK_NUM_RANK0		0xe
#define CAT_RANK_NUM_RANK1		0xd

#define CAT_BP_RANK_SEL_RANK0		0xe
#define CAT_BP_RANK_SEL_RANK1		0xd

#define CAT_ENABLE			BIT(0)
#define CAT_START			BIT(1)
#define CLK_DIV_CNT_MASK		(0x1f << 8)
#define CLK_DIV_CNT_SHIFT		8
#define CAT_RANK_NUM_MASK		(0xf << 15)
#define CAT_RANK_NUM_SHIFT		15
#define CAT_BP_RANK_SEL_MASK		(0xf << 20)
#define CAT_BP_RANK_SEL_SHIFT		20
#define CMD_RANK_SWITCH_DIS		BIT(28)

/* DDRPHY_REG9 */
#define DDRPHY_TCACD_MASK		0x1f
#define DDRPHY_TCACD_SHIFT		0
#define DDRPHY_TMRW_MASK		(0x1f << 5)
#define DDRPHY_TMRW_SHIFT		5
#define DDRPHY_TDSTRAIN_MASK		(0x1f << 10)
#define DDRPHY_TDSTRAIN_SHIFT		10
#define DDRPHY_TCKELCK_MASK		(0x1f << 15)
#define DDRPHY_TCKELCK_SHIFT		15
#define DDRPHY_TADR_MASK		(0x1f << 20)
#define DDRPHY_TADR_SHIFT		20
#define DDRPHY_TXCBT_MASK		(0x1f << 25)
#define DDRPHY_TXCBT_SHIFT		25

/* DDRPHY_REG10_0 */
#define DDRPHY_TFC_MASK			0xff
#define DDRPHY_TFC_SHIFT		0
#define DDRPHY_TCAENT_MASK		(0xff << 8)
#define DDRPHY_TCAENT_SHIFT		8
#define DDRPHY_TVREFCA_LONG_MASK	(0xff << 16)
#define DDRPHY_TVREFCA_LONG_SHIFT	16

/* DDRPHY_REGA */
#define DDRPHY_MR11_MASK		0xff
#define DDRPHY_MR11_SHIFT		0
#define DDRPHY_MR3_MASK			(0xff << 8)
#define DDRPHY_MR3_SHIFT		8
#define DDRPHY_MR2_MASK			(0xff << 16)
#define DDRPHY_MR2_SHIFT		16
#define DDRPHY_MR1_MASK			(0xff << 24)
#define DDRPHY_MR1_SHIFT		24

/* DDRPHY_REGB */
#define DDRPHY_MR22_MASK		0xff
#define DDRPHY_MR22_SHIFT		0
#define DDRPHY_MR14_MASK		(0xff << 8)
#define DDRPHY_MR14_SHIFT		8
#define DDRPHY_MR13_MASK		(0xff << 16)
#define DDRPHY_MR13_SHIFT		16

/* DDRPHY_REGC */
#define CAT_VREF_SCAN_DIS		BIT(30)

/* DDRPHY_REGD */
#define CA_PERBIT_SKEW_UPDATE		BIT(0)
#define CMD_PERBIT_SKEW_BP		BIT(1)
#define CAT_SKIP_FSPY			BIT(6)
#define LPDDR_CA_ODT_SEL_MASK		BIT(11)
#define LPDDR_CA_ODT_SEL_SHIFT		11
#define LPDDR_CA_ODT0_MASK		(0x3 << 12)
#define LPDDR_CA_ODT0_SHIFT		12
#define LPDDR_CA_ODT1_MASK		(0x3 << 14)
#define LPDDR_CA_ODT1_SHIFT		14

/* DDRPHY_REG10_1 */
#define CALIB_MODE_SEL			BIT(24)
#define FREQ_CHOOSE_T_MASK		(0x3 << 30)
#define FREQ_CHOOSE_T_SHIFT		30

/* DDRPHY_REG20 */
#define LP_BYPASS			BIT(15)
#define TRAIN_REG_UPDATE_EN		BIT(18)

/* DDRPHY_REG22 */
#define ZQCALI_BYPASS			BIT(1)
#define ZQCALI_CLEAR			BIT(2)
#define PD_ZQCALI			BIT(3)

/* DDRPHY_REG24 */
#define DQ_RD_TRAIN_EN			BIT(0)
#define RD_TRAIN_FREQ_UPDATE		BIT(2)
#define RD_TRAIN_CHECK_VALUE_EN		BIT(3)
#define RX_VREF_VALUE_UPDATE		BIT(7)
#define RDTRAIN_CS_SEL_MASK		(0xf << 8)
#define RDTRAIN_CS_SEL_SHIFT		8
#define RD_TRAIN_PREDEF_EN		BIT(14)

/* DDRPHY_REG27 */
#define DQ_WR_TRAIN_AUTO			BIT(0)
#define DQ_WR_TRAIN_EN				BIT(1)
#define WR_TRAIN_DQS_DEFAULT_BYPASS		BIT(4)
#define WRTRAIN_CS_SEL_MASK			(0xf << 6)
#define WRTRAIN_CS_SEL_SHIFT			6
#define WRTRAIN_CHECK_DATA_VALUE_RANDOM_GEN	BIT(10)
#define WRTRAIN_LPDDR4_VREF_RANGE_MASK		BIT(24)
#define WRTRAIN_LPDDR4_VREF_RANGE_SHIFT		24
#define DM_WR_TRAIN_EN				BIT(25)

/* DDRPHY_REG28 */
#define WR_TRAIN_ROW_ADDR_MASK		0xffff
#define WR_TRAIN_ROW_ADDR_SHIFT		0

/* DDRPHY_REG29 */
#define PHY_TRFC_MASK			(0x3ff << 8)
#define PHY_TRFC_SHIFT			8
#define PHY_TREFI_MASK			(0x1fff << 18)
#define PHY_TREFI_SHIFT			18

/* DDRPHY_REG2A */
#define FREQ_CHOOSE_B_EN		BIT(4)

/* DDRPHY_REG2F */
#define PVT_COMP_DIS			BIT(0)
#define CMD_T2_MODE_MASK		BIT(17)
#define CMD_T2_MODE_SHIFT		17
#define CMD_DELAY_ONE_UI_MASK		BIT(18)
#define CMD_DELAY_ONE_UI_SHIFT		18

/* DDRPHY_REG33 */
#define PLL_POSTDIV0_EN_MASK		BIT(3)
#define PLL_POSTDIV0_EN_SHIFT		3
#define PLL_POSTDIV0_MASK		(7 << 4)
#define PLL_POSTDIV0_SHIFT		4
#define PLL_POSTDIVN_EN_MASK(n)		(PLL_POSTDIV0_EN_MASK << (8 * (n)))
#define PLL_POSTDIVN_EN_SHIFT(n)	(PLL_POSTDIV0_EN_SHIFT + (8 * (n)))
#define PLL_POSTDIVN_MASK(n)		(PLL_POSTDIV0_MASK << (8 * (n)))
#define PLL_POSTDIVN_SHIFT(n)		(PLL_POSTDIV0_SHIFT + (8 * (n)))

/* DDRPHY_REG37 */
#define CMD_ABUTSLEWPD_MASK		(0x1f << 0)
#define CMD_ABUTSLEWPD_SHIFT		0
#define CMD_ABUTSLEWPU_MASK		(0x1f << 8)
#define CMD_ABUTSLEWPU_SHIFT		8

/* DDRPHY_REG38 */
#define CMD_ABUTPRCOMP_CK0_MASK		(0x1f << 0)
#define CMD_ABUTPRCOMP_CK0_SHIFT	0
#define CMD_ABUTNRCOMP_CK0_MASK		(0x1f << 8)
#define CMD_ABUTNRCOMP_CK0_SHIFT	8
#define CMD_ABUTPRCOMP_MASK		(0x1f << 16)
#define CMD_ABUTPRCOMP_SHIFT		16
#define CMD_ABUTNRCOMP_MASK		(0x1f << 24)
#define CMD_ABUTNRCOMP_SHIFT		24

/* DDRPHY_REG3B */
#define A0_INVDELAYSEL_MASK		(0xff << 24)
#define A0_INVDELAYSEL_SHIFT		24

/* undocumented0, DDRPHY_COM_REG_0x3e in rk3562 TRM */
#define CMD_ABUTPRCOMP_SPECIAL_MASK	0x1f
#define CMD_ABUTPRCOMP_SPECIAL_SHIFT	0
#define CMD_ABUTNRCOMP_SPECIAL_MASK	(0x1f << 8)
#define CMD_ABUTNRCOMP_SPECIAL_SHIFT	8
#define RAM_VREF1_MARGSEL_MASK		(0x1ff << 16)
#define RAM_VREF1_MARGSEL_SHIFT		16

/* DDRPHY_REG41 */
#define CK_INVDELAYSEL_MASK		(0xff << 16)
#define CK_INVDELAYSEL_SHIFT		16

/*
 * DDRPHY_REG54, DDRPHY_REG57, DDRPHY_REG5A, DDRPHY_REG5D, DDRPHY_REG60, DDRPHY_REG63, DDRPHY_REG66,
 * DDRPHY_REG69
 */
#define DQ_TRAIN_CHECK_DATA_VALUE_MASK(n)	(0xff << ((n) * 8))
#define DQ_TRAIN_CHECK_DATA_VALUE_SHIFT(n)	((n) * 8)

/* DDRPHY_REG6C, CMD_INVDELAYSEL_SEL_* description is based on rk3562 TRM. */
#define CMD_INVDELAYSEL_SEL_A_CA0_CS0		0
#define CMD_INVDELAYSEL_SEL_A_CA1_CS0		1
#define CMD_INVDELAYSEL_SEL_A_CA2_CS0		2
#define CMD_INVDELAYSEL_SEL_A_CA3_CS0		3
#define CMD_INVDELAYSEL_SEL_A_CA4_CS0		4
#define CMD_INVDELAYSEL_SEL_A_CA5_CS0		5
#define CMD_INVDELAYSEL_SEL_B_CA0_CS0		6
#define CMD_INVDELAYSEL_SEL_B_CA1_CS0		7
#define CMD_INVDELAYSEL_SEL_B_CA2_CS0		8
#define CMD_INVDELAYSEL_SEL_B_CA3_CS0		9
#define CMD_INVDELAYSEL_SEL_B_CA4_CS0		10
#define CMD_INVDELAYSEL_SEL_B_CA5_CS0		11
#define CMD_INVDELAYSEL_SEL_A_CA0_CS1		12
#define CMD_INVDELAYSEL_SEL_A_CA1_CS1		13
#define CMD_INVDELAYSEL_SEL_A_CA2_CS1		14
#define CMD_INVDELAYSEL_SEL_A_CA3_CS1		15
#define CMD_INVDELAYSEL_SEL_A_CA4_CS1		16
#define CMD_INVDELAYSEL_SEL_A_CA5_CS1		17
#define CMD_INVDELAYSEL_SEL_B_CA0_CS1		18
#define CMD_INVDELAYSEL_SEL_B_CA1_CS1		19
#define CMD_INVDELAYSEL_SEL_B_CA2_CS1		20
#define CMD_INVDELAYSEL_SEL_B_CA3_CS1		21
#define CMD_INVDELAYSEL_SEL_B_CA4_CS1		22
#define CMD_INVDELAYSEL_SEL_B_CA5_CS1		23
#define CMD_INVDELAYSEL_SEL_B_CA5_CS1		23
#define CMD_INVDELAYSEL_SEL_A_CK		24
#define CMD_INVDELAYSEL_SEL_A_CKB		25
#define CMD_INVDELAYSEL_SEL_A_CKE0		26
#define CMD_INVDELAYSEL_SEL_A_CKE1		27
#define CMD_INVDELAYSEL_SEL_A_CSB0		28
#define CMD_INVDELAYSEL_SEL_A_CSB1		29
#define CMD_INVDELAYSEL_SEL_A_ODT0		30
#define CMD_INVDELAYSEL_SEL_A_ODT1		31
#define CMD_INVDELAYSEL_SEL_B_CK		32
#define CMD_INVDELAYSEL_SEL_B_CKB		33
#define CMD_INVDELAYSEL_SEL_B_CKE0		34
#define CMD_INVDELAYSEL_SEL_B_CKE1		35
#define CMD_INVDELAYSEL_SEL_B_CSB0		36
#define CMD_INVDELAYSEL_SEL_B_CSB1		37
#define CMD_INVDELAYSEL_SEL_B_ODT0		38
#define CMD_INVDELAYSEL_SEL_B_ODT1		39

#define RDTRAIN_WAIT_VERF_VALID_CNT_MASK	0x1ff
#define RDTRAIN_WAIT_VERF_VALID_CNT_SHIFT	0
#define CMD_INVDELAYSEL_SEL_MASK		(0x3f << 10)
#define CMD_INVDELAYSEL_SEL_SHIFT		10

/* DDRPHY_REG7C */
#define TRAIN_TRUE_DONE			BIT(0)
#define TRAIN_STEP3_ERROR		BIT(1)
#define TRAIN_STEP2_ERROR		BIT(2)
#define TRAIN_STEP1_ERROR		BIT(3)
#define TRAIN_ALL_STEP_DONE		BIT(7)

/* DDRPHY_REG7D */
#define HALFUI_LOCK_CODE_TO_REG_MASK	(0xff << 24)
#define HALFUI_LOCK_CODE_TO_REG_SHIFT	24

/* DDRPHY_REG83 */
#define CALIB_DONE_BYTE_MASK		0x1f
#define CALIB_ERROR			BIT(5)
#define WL_DONE_BYTE_MASK		(0x1f << 8)
#define WL_DONE_BYTE_SHIFT		8

/* DDRPHY_REG85 */
#define CHB_CAT_DONE			BIT(2)
#define CHA_CAT_DONE			BIT(3)

/* undocumented1, DDRPHY_COM_REG_0x8c in rk3562 TRM */
#define WRTRAIN_VREF_MIN_VALUE_MASK	0xf7
#define WRTRAIN_VREF_MIN_VALUE_SHIFT	0
#define WRTRAIN_VREF_MAX_VALUE_MASK	(0xf7 << 8)
#define WRTRAIN_VREF_MAX_VALUE_SHIFT	8
#define CMD_INVDELAYSEL_MASK		(0xff << 16)
#define CMD_INVDELAYSEL_SHIFT		16

/* DDRPHY_REGAD */
#define TRAIN_ERROR_FOR_RD_BYTE_MASK	(0x1f << 10)
#define TRAIN_ERROR_FOR_RD_BYTE_SHIFT	10

/* The next ones are totally missing from rk3568 TRM. Based on rk3562 TRM. */

/* DDRPHY_BYTE_REG_0x0 */
#define ABC_LH_ENB_LP4MODE_MASK		BIT(7)
#define ABC_LH_ENB_LP4MODE_SHIFT	7
#define ABC_LH_ABUTSLEWPU_MASK		(0x1f << 8)
#define ABC_LH_ABUTSLEWPU_SHIFT		8
#define ABC_LH_VREF1_MARGSEL_MASK	(0x1ff << 23)
#define ABC_LH_VREF1_MARGSEL_SHIFT	23

/* DDRPHY_BYTE_REG_0x1 */
#define ABC_LH_ABUTODTPU_MASK		(0x1f << 0)
#define ABC_LH_ABUTODTPU_SHIFT		0
#define ABC_LH_ABUTODTPD_MASK		(0x1f << 8)
#define ABC_LH_ABUTODTPD_SHIFT		8
#define ABC_LH_ABUTPRCOMP_MASK		(0x1f << 16)
#define ABC_LH_ABUTPRCOMP_SHIFT		16
#define ABC_LH_ABUTNRCOMP_MASK		(0x1f << 24)
#define ABC_LH_ABUTNRCOMP_SHIFT		24

/* DDRPHY_BYTE_REG_0x2 */
#define ABC_LH_DQSWEAKP_PULL_UP		0
#define ABC_LH_DQSWEAKP_MIDDLE		1
#define ABC_LH_DQSWEAKP_HIGH_Z		2
#define ABC_LH_DQSWEAKP_PULL_DN		3

#define ABC_LH_DQSWEAKP_MASK		(3 << 5)
#define ABC_LH_DQSWEAKP_SHIFT		5
#define ABC_LH_LP4X_EN			BIT(8)
#define ABC_LH_RXEN_LP4			BIT(9)

/* DDRPHY_BYTE_REG_0xf, DDRPHY_BYTE_REG_0x13, DDRPHY_BYTE_REG_0x42, DDRPHY_BYTE_REG_0x46 */
#define ABC_LH_CSN_DQSB_INVDELAYSELRX_MASK	(0x7f << 8)
#define ABC_LH_CSN_DQSB_INVDELAYSELRX_SHIFT	8
#define ABC_LH_CSN_DQS_INVDELAYSELRX_MASK	(0x7f << 24)
#define ABC_LH_CSN_DQS_INVDELAYSELRX_SHIFT	24

/* DDRPHY_BYTE_REG_0xc, DDRPHY_BYTE_REG_0x14, DDRPHY_BYTE_REG_0x43, DDRPHY_BYTE_REG_0x4b */
#define ABC_LH_CSN_LOOP_INVDELAYSEL_MASK	(0x1f << 24)
#define ABC_LH_CSN_LOOP_INVDELAYSEL_SHIFT	24

/* DDRPHY_BYTE_REG_0x18 */
#define ABC_LH_TRAIN_DQS_DEFAULT_MASK		(0xff << 16)
#define ABC_LH_TRAIN_DQS_DEFAULT_SHIFT		16
#define ABC_LH_RD_TRAIN_DQS_DEFAULT_MASK	(0x1f << 24)
#define ABC_LH_RD_TRAIN_DQS_DEFAULT_SHIFT	24

/* DDRPHY_BYTE_REG_0x19 */
#define ABC_LH_RDTRAIN_CHECK_WRAP1_MASK		0xff
#define ABC_LH_RDTRAIN_CHECK_WRAP1_SHIFT	0
#define ABC_LH_RDTRAIN_CHECK_WRAP0_MASK		(0xff << 8)
#define ABC_LH_RDTRAIN_CHECK_WRAP0_SHIFT	8

/* DDRPHY_BYTE_REG_0x1c */
#define ABC_LH_TDQS_INVDELAYSEL1_MASK		0xff
#define ABC_LH_TDQS_INVDELAYSEL1_SHIFT		0
#define ABC_LH_TDQS_INVDELAYSEL0_MASK		(0xff << 16)
#define ABC_LH_TDQS_INVDELAYSEL0_SHIFT		16

/* DDRPHY_BYTE_REG_0x1e */
#define ABC_LH_CALIB_RESULT_CS0_MASK	(0x7ff << 16)
#define ABC_LH_CALIB_RESULT_CS0_SHIFT	16

/* DDRPHY_BYTE_REG_0x1f, DDRPHY_BYTE_REG_0x20, DDRPHY_BYTE_REG_0x54, DDRPHY_BYTE_REG_0x55 */
#define ABC_LH_CSN_VALUE_DQX_INVDELAYSEL_MASK	0xff
#define ABC_LH_CSN_VALUE_DQX_INVDELAYSEL_SHIFT  0

/* DDRPHY_BYTE_REG_0x27 */
#define ABC_LH_TRAIN_RESULT_FOR_RD_BASE_DQS_MASK	(0x7f << 24)
#define ABC_LH_TRAIN_RESULT_FOR_RD_BASE_DQS_SHIFT	24

struct rk3568_ddr_phy_regs {
	u32 phy[8][2];
};

struct rk3568_ddrphy_invdelaysel_regs {
	u32 reg3b;
	u32 reg3c;
	u32 reg3d;
	u32 reg3e;
	u32 reg3f;
	u32 reg40;
	u32 reg41;
	u32 reg42;
};

struct rk3568_ddrphy_dq_train_check_data_regs {
	u32 reg54;
	u32 reg55;
	u32 reg56;
	u32 reg57;
	u32 reg58;
	u32 reg59;
	u32 undocumented0[(0x1b0 - 0x164) / 4 - 1]; /* documented in rk3562 TRM */
};

struct rk3568_ddrphy_pad_group {
	u32 reg0;
	u32 reg1;
	u32 reg2;
	u32 reg3;
	u32 reg4;
	u32 reg5;
	u32 reg6;
	u32 reg7;
	u32 reg8;
	u32 reg9;
	u32 rega;
	u32 regb;
	u32 regc;
	u32 regd;
	u32 rege;
	u32 regf;
	u32 reg10;
	u32 reg11;
	u32 reg12;
	u32 reg13;
	u32 reg14;
	u32 reg15;
	u32 reg16;
	u32 reg17;
	u32 reg18;
	u32 reg19;
	u32 reserved0[(0x70 - 0x64) / 4 - 1];
	u32 tdqs_invdelaysel12;
	u32 reg1d;
	u32 reg1e;
	u32 reg1f;
	u32 reg20;
	u32 train_for_rd[6];
	u32 reg27;
	u32 rd_train_readback_data[4];
	u32 train_for_wr[6];
	u32 reg32;
	u32 reserved1[(0x100 - 0xc8) / 4 - 1];
	u32 reg40;
	u32 reg41;
	u32 reg42;
	u32 reg43;
	u32 reg44;
	u32 reg45;
	u32 reg46;
	u32 reg47;
	u32 reg48;
	u32 reg49;
	u32 reg4a;
	u32 reg4b;
	u32 reg4c;
	u32 reg4d;
	u32 reg4e;
	u32 reg4f;
	u32 reg50;
	u32 reg51;
	u32 reg52;
	u32 tdqs_invdelaysel23;
	u32 reg54;
	u32 reg55;
	u32 reg56;
	u32 reserved2[(0x180 - 0x158) / 4 - 1];
};

struct rk3568_ddrphy {
	u32 reg0;
	u32 reg1;
	u32 reg2;
	u32 reg3;
	u32 reg4;
	u32 reg5;
	u32 reg6;
	u32 reg7;
	u32 reg8;
	u32 reg9;
	u32 reg10_0;
	u32 rega;
	u32 regb;
	u32 regc;
	u32 regd;
	u32 rege;
	u32 regf;
	u32 reg10_1;
	u32 reg11;
	u32 reg12;
	u32 reg13;
	u32 reg14;
	u32 reg15;
	u32 reg16;
	u32 reg17;
	u32 reg18;
	u32 reg19;
	u32 reg1a;
	u32 reg1b;
	u32 reg1c;
	u32 reg1d;
	u32 reg1e;
	u32 reg1f;
	u32 reg20;
	u32 reg21;
	u32 reg22;
	u32 reg23;
	u32 reg24;
	u32 reg25;
	u32 reg26;
	u32 reg27;
	u32 reg28;
	u32 reg29;
	u32 reg2a;
	u32 reg2b;
	u32 reg2c;
	u32 reg2d;
	u32 reg2e;
	u32 reg2f;
	u32 reg30;
	u32 reg31;
	u32 reg32;
	u32 reg33;
	u32 reg34;
	u32 reg35;
	u32 reg36;
	u32 reserved0[(0x0f0 - 0x0dc) / 4 - 1];
	u32 reg37;
	u32 reg38;
	u32 undocumented0; /* documented in rk3562 TRM */
	u32 reserved1[(0x104 - 0x0f8) / 4 - 1];
	union {
		struct rk3568_ddrphy_invdelaysel_regs invdelaysel_regs;
		u32 reg_n_invdelaysel[8];
	};
	u32 reserved2[(0x150 - 0x120) / 4 - 1];
	union {
		struct rk3568_ddrphy_dq_train_check_data_regs dq_train_check_data_regs;
		u32 reg_n_dq_train_check_data[8][3];
	};
	u32 reg6c;
	u32 reserved4;
	u32 reg6e;
	u32 reserved5[(0x1f0 - 0x1b8) / 4 - 1];
	u32 reg7c;
	u32 reg7d;
	u32 reg7e;
	u32 reg7f;
	u32 reserved6[(0x20c - 0x1fc) / 4 - 1];
	u32 reg83;
	u32 reg84;
	u32 reg85;
	u32 reserved7[(0x230 - 0x214) / 4 - 1];
	u32 undocumented1; /* documented in rk3562 TRM */
	u32 cmd_training_result[2][7];
	u32 reserved8[(0x2b4 - 0x268) / 4 - 1];
	u32 regad;
	u32 reserved9[(0x300 - 0x2b4) / 4 - 1];
	/* 2 * x8 for channel A, 2 * x8 for channel B and x8 for channel C (ECC) */
	struct rk3568_ddrphy_pad_group pad_group[5];
};

#endif /* _ASM_ARCH_SDRAM_PHY_RK3568_H */

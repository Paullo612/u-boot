0x12345678,
2, /* version */
(0  << 0) | (1  << 8) | (9  << 16) | (8 << 24), /* cpu_gen,global index */
(0  << 0) | (0  << 8) | (0  << 16) | (0 << 24), /* d2,d3 index */
(17 << 0) | (9  << 8) | (0  << 16) | (0 << 24), /* d4,d5 index */
(0  << 0) | (0  << 8) | (0  << 16) | (0 << 24), /* lp2,lp3 index */
(26 << 0) | (13 << 8) | (0  << 16) | (0 << 24), /* lp4,lp5 index */
(0  << 0) | (0  << 8) | (39 << 16) | (8 << 24), /* skew index, dq_map index */
(0  << 0) | (0  << 8) | (0  << 16) | (0 << 24), /*lp4x index*/
/* global info */
0,
(93 << 16) | 13,/* sr_idle << 16 | pd_idle */
0,/* channel info */
(1 << 26) | 1, /* derate, 2t info */
0, /* reserved */
(0 << 3), /* pageclose */
0, 0, /* reserved */

/* ddr4 */
(1056 << DDR_FREQ_F0_SHIFT) | (324 << DDR_FREQ_F1_SHIFT),
(528 << DDR_FREQ_F2_SHIFT) | (780 << DDR_FREQ_F3_SHIFT),
(0 << DDR_FREQ_F4_SHIFT) | (0 << DDR_FREQ_F5_SHIFT),
/* drv when odt on */
(37 << PHY_DQ_DRV_SHIFT) | (37 << PHY_CA_DRV_SHIFT) |
	(37 << PHY_CLK_DRV_SHIFT) | (34 << DRAM_DQ_DRV_SHIFT),
/* drv when odt off */
(37 << PHY_DQ_DRV_SHIFT) | (37 << PHY_CA_DRV_SHIFT) |
	(37 << PHY_CLK_DRV_SHIFT) | (34 << DRAM_DQ_DRV_SHIFT),
/* odt info */
(120 << DRAM_ODT_SHIFT) | (139 << PHY_ODT_SHIFT) |
	(1 << PHY_ODT_PUUP_EN_SHIFT) | (1 << PHY_ODT_PUDN_EN_SHIFT),
/* odt enable freq */
(625 << DRAM_ODT_EN_FREQ_SHIFT) | (625 << PHY_ODT_EN_FREQ_SHIFT),
/* slew rate when odt enable */
(0x0 << PHY_DQ_SR_SHIFT) | (0x1 << PHY_CA_SR_SHIFT) |
	(0x1 << PHY_CLK_SR_SHIFT),
/* slew  ratee when odt disable */
(0x0 << PHY_DQ_SR_SHIFT) | (0x1 << PHY_CA_SR_SHIFT) |
	(0x1 << PHY_CLK_SR_SHIFT),

/* lpddr4 */
(1056 << DDR_FREQ_F0_SHIFT) | (324 << DDR_FREQ_F1_SHIFT),
(528 << DDR_FREQ_F2_SHIFT) | (780 << DDR_FREQ_F3_SHIFT),
(0 << DDR_FREQ_F4_SHIFT) | (0 << DDR_FREQ_F5_SHIFT),

/* drv when odt on */
(30 << PHY_DQ_DRV_SHIFT) | (38 << PHY_CA_DRV_SHIFT) |
	(38 << PHY_CLK_DRV_SHIFT) | (40 << DRAM_DQ_DRV_SHIFT),
/* drv when odt off */
(30 << PHY_DQ_DRV_SHIFT) | (38 << PHY_CA_DRV_SHIFT) |
	(38 << PHY_CLK_DRV_SHIFT) | (40 << DRAM_DQ_DRV_SHIFT),
/* odt info and PU-cal info */
(80 << DRAM_ODT_SHIFT) | (60 << PHY_ODT_SHIFT) |
	(120 << LP4_CA_ODT_SHIFT) |
	(LPDDR4_VDDQ_3 << LP4_DRV_PU_CAL_ODTEN_SHIFT) |
	(LPDDR4_VDDQ_3 << LP4_DRV_PU_CAL_ODTOFF_SHIFT) |
	(0 << PHY_LP4_DRV_PULLDOWN_EN_ODTEN_SHIFT) |
	(0 << PHY_LP4_DRV_PULLDOWN_EN_ODTOFF_SHIFT),
/* odt enable freq */
(800 << PHY_LP4_ODT_EN_FREQ_SHIFT) | (800 << LP4_DQ_ODT_EN_FREQ_SHIFT),
/* slew rate when odt enable */
(0 << PHY_DQ_SR_SHIFT) | (0 << PHY_CA_SR_SHIFT) |
	(0 << PHY_CLK_SR_SHIFT),
/* slew  rate when odt disable */
(0 << PHY_DQ_SR_SHIFT) | (0 << PHY_CA_SR_SHIFT) |
	(0 << PHY_CLK_SR_SHIFT),
/* ca odt en freq */
(800 << LP4_CA_ODT_EN_FREQ_SHIFT),
/* cs drv info and ca odt info */
(0 << PHY_LP4_CS_DRV_ODTEN_SHIFT) |
	(0 << PHY_LP4_CS_DRV_ODTOFF_SHIFT) |
	(1 << LP4_ODTE_CK_SHIFT) | (1 << LP4_ODTE_CS_EN_SHIFT) |
	(0 << LP4_ODTD_CA_EN_SHIFT),
/* vref info when odt enable */
(166 << PHY_LP4_DQ_VREF_SHIFT) | (300 << LP4_DQ_VREF_SHIFT) |
	(380 << LP4_CA_VREF_SHIFT),
/* vref info when odt disable */
(420 << PHY_LP4_DQ_VREF_SHIFT) | (420 << LP4_DQ_VREF_SHIFT) |
	(420 << LP4_CA_VREF_SHIFT),

/* ddr4 map << 0 | ddr3 map << 24 */
((0x3 << 6) | (0x2 << 4) | (0x1 << 2) | (0x0 << 0)) | (0 << 8) | (0 << 16),
/* lp3 map << 16 | lp4 map << 24 */
((0x3 << 6) | (0x2 << 4) | (0x1 << 2) | (0x0 << 0)) << 24,
/* lp3 dq0-7 map */
0,
/* lp2 dq0-7 map */
0,
/* ddr4 dq map */
/* cs0 dq0-15 */
	((0 << 0 | 2 << 2 | 0 << 4 | 2 << 6) << 0) |
	((3 << 0 | 1 << 2 | 3 << 4 | 1 << 6) << 8) |
	((3 << 0 | 1 << 2 | 3 << 4 | 1 << 6) << 16) |
	((2 << 0 | 0 << 2 | 2 << 4 | 0 << 6) << 24),
/* cs0 dq16-31 */
	((3 << 0 | 1 << 2 | 3 << 4 | 1 << 6) << 0) |
	((0 << 0 | 2 << 2 | 0 << 4 | 2 << 6) << 8) |
	((0 << 0 | 2 << 2 | 0 << 4 | 2 << 6) << 16) |
	((1 << 0 | 3 << 2 | 3 << 4 | 1 << 6) << 24),
/* cs1 dq0-15 */
	((0 << 0 | 2 << 2 | 0 << 4 | 2 << 6) << 0) |
	((3 << 0 | 1 << 2 | 3 << 4 | 1 << 6) << 8) |
	((3 << 0 | 1 << 2 | 3 << 4 | 1 << 6) << 16) |
	((2 << 0 | 0 << 2 | 2 << 4 | 0 << 6) << 24),
/* cs1 dq16-31 */
	((3 << 0 | 1 << 2 | 3 << 4 | 1 << 6) << 0) |
	((0 << 0 | 2 << 2 | 0 << 4 | 2 << 6) << 8) |
	((0 << 0 | 2 << 2 | 0 << 4 | 2 << 6) << 16) |
	((1 << 0 | 3 << 2 | 3 << 4 | 1 << 6) << 24),

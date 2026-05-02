{
	{
		{
			.rank = 2,
			.col = 10,
			.bk = 3,
			.bw = 2,
			.dbw = 1,
			.row_3_4 = 0,
			.cs0_row = 17,
			.cs1_row = 17,
			.cs0_high16bit_row = 0,
			.cs1_high16bit_row = 0,
			.ddrconfig = 0
		},
		{
			{0x40231420},
			{0x15050b07},
			{0x00000c04},
			{0x00000000},
			{0x00001111},
			{0x00000054},
			0x000000ff
		}
	},
	{
		.ddr_freq = 1056,	/* clock rate(MHz) */
		.dramtype = LPDDR4,
		.num_channels = 1,
		.stride = 0,
		.odt = 0
	},
	{
		{
			{0x00000000, 0x8f080020},	/* MSTR */
			{0x00000064, 0x004000c9},	/* RFSHTMG */
			{0x00000068, 0x00300000},	/* RFSHTMG1 */
			{0x000000d0, 0x00030409},	/* INIT0 */
			{0x000000d4, 0x00690000},	/* INIT1 */
			{0x000000d8, 0x00000206},	/* INIT2 */
			{0x000000dc, 0x0034001b},	/* INIT3 */
			{0x000000e0, 0x00310000},	/* INIT4 */
			{0x000000e8, 0x00230000},	/* INIT6 */
			{0x000000ec, 0x00000000},	/* INIT7 */
			{0x000000f4, 0x000f022f},	/* RANKCTL */
			{0x00000100, 0x14161117},	/* DRAMTMG0 */
			{0x00000104, 0x00040421},	/* DRAMTMG1 */
			{0x00000108, 0x050a0e0f},	/* DRAMTMG2 */
			{0x0000010c, 0x00808000},	/* DRAMTMG3 */
			{0x00000110, 0x0a04060a},	/* DRAMTMG4 */
			{0x00000114, 0x02040808},	/* DRAMTMG5 */
			{0x00000118, 0x01010005},	/* DRAMTMG6 */
			{0x0000011c, 0x00000401},	/* DRAMTMG7 */
			{0x00000120, 0x00000808},	/* DRAMTMG8 */
			{0x00000130, 0x00020000},	/* DRAMTMG12 */
			{0x00000134, 0x0a100002},	/* DRAMTMG13 */
			{0x00000138, 0x000000cd},	/* DRAMTMG14 */
			{0x00000180, 0x02100010},	/* ZQCTL0 */
			{0x00000184, 0x01b00000},	/* ZQCTL1 */
			{0x00000190, 0x07070001},	/* DFITMG0 */
			{0x00000198, 0x0a000101},	/* DFILPCFG0 */
			{0x000001a0, 0xc0400003},	/* DFIUPD0 */
			{0x00000240, 0x0b050d3c},	/* ODTCFG */
			{0x00000244, 0x00000101},	/* ODTMAP */
			{0x00000250, 0x00001f00},	/* SCHED */
			{0x00000490, 0x00000001},	/* PCTRL_0 */
			{0xffffffff, 0xffffffff}
		}
	},
	{
		{
			{0x00000000, 0x00001fd7},	/* DDRPHY_REG0 */
			{0x00000008, 0x00000000},	/* DDRPHY_REG2 */
			{0x0000000c, 0x14000000},	/* DDRPHY_REG3 */
			{0x00000010, 0x0a000000},	/* DDRPHY_REG4 */
			{0xffffffff, 0xffffffff}
		}
	}
},

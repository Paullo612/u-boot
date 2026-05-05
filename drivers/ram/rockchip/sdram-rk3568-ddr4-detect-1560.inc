{
	{
		{
			.rank = 1,
			.col = 10,
			.bk = 2,
			.bw = 1,
			.dbw = 0,
			.row_3_4 = 0,
			.cs0_row = 17,
			.cs1_row = 0,
			.cs0_high16bit_row = 17,
			.cs1_high16bit_row = 0,
			.ddrconfig = 0
		},
		{
			{0x622c1c25},
			{0x10020a05},
			{0x00000002},
			{0x00000252},
			{0x00001111},
			{0x0000000c},
			0x000000ff
		}
	},
	{
		.ddr_freq = 1560,	/* clock rate(MHz) */
		.dramtype = DDR4,
		.num_channels = 1,
		.stride = 0,
		.odt = 0
	},
	{
		{
			{0x00000000, 0x4f041010},	/* MSTR */
			{0x00000064, 0x00be0111},	/* RFSHTMG */
			{0x000000d0, 0x0002017e},	/* INIT0 */
			{0x000000d4, 0x009a0000},	/* INIT1 */
			{0x000000d8, 0x00000100},	/* INIT2 */
			{0x000000dc, 0x0d540201},	/* INIT3 */
			{0x000000e0, 0x00280000},	/* INIT4 */
			{0x000000e4, 0x00110000},	/* INIT5 */
			{0x000000e8, 0x00000420},	/* INIT6 */
			{0x000000ec, 0x00001000},	/* INIT7 */
			{0x000000f4, 0x000f022f},	/* RANKCTL */
			{0x00000100, 0x16113419},	/* DRAMTMG0 */
			{0x00000104, 0x00050625},	/* DRAMTMG1 */
			{0x00000108, 0x080c0710},	/* DRAMTMG2 */
			{0x0000010c, 0x0040400c},	/* DRAMTMG3 */
			{0x00000110, 0x0c04040d},	/* DRAMTMG4 */
			{0x00000114, 0x08080504},	/* DRAMTMG5 */
			{0x00000120, 0x0a0a110a},	/* DRAMTMG8 */
			{0x00000124, 0x0002020c},	/* DRAMTMG9 */
			{0x00000180, 0x01000040},	/* ZQCTL0 */
			{0x00000184, 0x00000000},	/* ZQCTL1 */
			{0x00000190, 0x070a0006},	/* DFITMG0 */
			{0x00000198, 0x0a000101},	/* DFILPCFG0 */
			{0x000001a0, 0xc0400003},	/* DFIUPD0 */
			{0x00000240, 0x06000620},	/* ODTCFG */
			{0x00000244, 0x00000201},	/* ODTMAP */
			{0x00000250, 0x00001f00},	/* SCHED */
			{0x00000490, 0x00000001},	/* PCTRL_0 */
			{0xffffffff, 0xffffffff}
		}
	},
	{
		{
			{0x00000000, 0x00001fc7},	/* DDRPHY_REG0 */
			{0x00000008, 0x00000000},	/* DDRPHY_REG2 */
			{0x0000000c, 0x18000000},	/* DDRPHY_REG3 */
			{0x00000010, 0x10000000},	/* DDRPHY_REG4 */
			{0xffffffff, 0xffffffff}
		}
	}
},

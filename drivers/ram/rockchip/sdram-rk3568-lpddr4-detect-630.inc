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
			{0x35160c13},
			{0x0d030805},
			{0x00000c04},
			{0x00000000},
			{0x00001111},
			{0x00000054},
			0x000000ff
		}
	},
	{
		.ddr_freq = 630,	/* clock rate(MHz) */
		.dramtype = LPDDR4,
		.num_channels = 1,
		.stride = 0,
		.odt = 0
	},
	{
		{
			{0x00000000, 0x8f080020},	/* MSTR */
			{0x00000064, 0x00260078},	/* RFSHTMG */
			{0x00000068, 0x001d0000},	/* RFSHTMG1 */
			{0x000000d0, 0x00020269},	/* INIT0 */
			{0x000000d4, 0x003f0000},	/* INIT1 */
			{0x000000d8, 0x00000204},	/* INIT2 */
			{0x000000dc, 0x00240012},	/* INIT3 */
			{0x000000e0, 0x00310000},	/* INIT4 */
			{0x000000e8, 0x00000000},	/* INIT6 */
			{0x000000ec, 0x00000000},	/* INIT7 */
			{0x000000f4, 0x000f022f},	/* RANKCTL */
			{0x00000100, 0x0f0d0a0e},	/* DRAMTMG0 */
			{0x00000104, 0x00030414},	/* DRAMTMG1 */
			{0x00000108, 0x04070b0d},	/* DRAMTMG2 */
			{0x0000010c, 0x00505000},	/* DRAMTMG3 */
			{0x00000110, 0x06040406},	/* DRAMTMG4 */
			{0x00000114, 0x02030505},	/* DRAMTMG5 */
			{0x00000118, 0x01010004},	/* DRAMTMG6 */
			{0x0000011c, 0x00000301},	/* DRAMTMG7 */
			{0x00000120, 0x00000505},	/* DRAMTMG8 */
			{0x00000130, 0x00020000},	/* DRAMTMG12 */
			{0x00000134, 0x00100002},	/* DRAMTMG13 */
			{0x00000138, 0x0000007b},	/* DRAMTMG14 */
			{0x00000180, 0x013b000a},	/* ZQCTL0 */
			{0x00000184, 0x01000000},	/* ZQCTL1 */
			{0x00000190, 0x07040000},	/* DFITMG0 */
			{0x00000198, 0x0a000101},	/* DFILPCFG0 */
			{0x000001a0, 0xc0400003},	/* DFIUPD0 */
			{0x00000240, 0x0a040b28},	/* ODTCFG */
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
			{0x0000000c, 0x0e000000},	/* DDRPHY_REG3 */
			{0x00000010, 0x08000000},	/* DDRPHY_REG4 */
			{0xffffffff, 0xffffffff}
		}
	}
},

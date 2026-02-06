// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright Contributors to the U-Boot project.

#define LOG_CATEGORY LOGC_ARCH

#include <dm.h>
#include <misc.h>
#include <asm/armv8/mmu.h>
#include <asm/arch-rockchip/bootrom.h>
#include <asm/arch-rockchip/hardware.h>

#define FIREWALL_DDR_BASE		0xfef00000
#define FW_DDR_MST4_REG			0x30
#define FW_DDR_MST5_REG			0x34
#define FW_DDR_MST6_REG			0x38

#define SYS_SGRF_BASE			0xff020000

#define GRF_PERI_BASE			0xff040000
#define USB3OTG_CON1			0x0094

const char * const boot_devices[BROM_LAST_BOOTSOURCE + 1] = {
	[BROM_BOOTSOURCE_EMMC] = "/soc/mmc@ff870000",
	[BROM_BOOTSOURCE_SPINOR] = "/soc/spi@ff860000/flash@0",
	[BROM_BOOTSOURCE_SD] = "/soc/mmc@ff880000",
};

static struct mm_region rk3562_mem_map[] = {
	{
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0xfc000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xfc000000UL,
		.phys = 0xfc000000UL,
		.size = 0x4000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0x100000000UL,
		.phys = 0x100000000UL,
		.size = 0x100000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0x300000000UL,
		.phys = 0x300000000UL,
		.size = 0x40000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = rk3562_mem_map;

void board_debug_uart_init(void)
{
}

int arch_cpu_init(void)
{
	u32 val;

	if (!IS_ENABLED(CONFIG_SPL_BUILD))
		return 0;

	/* Select non-secure OTPC */
	rk_clrreg(SYS_SGRF_BASE + 0x34, BIT(3));

	/* Set the emmc to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST4_REG);
	writel(val & 0x0000ffff, FIREWALL_DDR_BASE + FW_DDR_MST4_REG);

	/* Set the fspi to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST5_REG);
	writel(val & 0x00ffffff, FIREWALL_DDR_BASE + FW_DDR_MST5_REG);

	/* Set the sdmmc to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST6_REG);
	writel(val & 0xff0000ff, FIREWALL_DDR_BASE + FW_DDR_MST6_REG);

	/* Set the usb to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST6_REG);
	writel(val & 0x00ffffff, FIREWALL_DDR_BASE + FW_DDR_MST6_REG);

	/* Disable USB3OTG U3 port, later enabled in COMBPHY driver */
	writel(0xffff0181, GRF_PERI_BASE + USB3OTG_CON1);

	return 0;
}

#define HP_TIMER_BASE			CONFIG_ROCKCHIP_STIMER_BASE
#define HP_CTRL_REG			0x04
#define TIMER_EN			BIT(0)
#define HP_LOAD_COUNT0_REG		0x14
#define HP_LOAD_COUNT1_REG		0x18

void rockchip_stimer_init(void)
{
	u32 reg;

	if (!IS_ENABLED(CONFIG_XPL_BUILD))
		return;

	reg = readl(HP_TIMER_BASE + HP_CTRL_REG);
	if (reg & TIMER_EN)
		return;

	asm volatile("msr cntfrq_el0, %0" : : "r" (CONFIG_COUNTER_FREQUENCY));
	writel(0xffffffff, HP_TIMER_BASE + HP_LOAD_COUNT0_REG);
	writel(0xffffffff, HP_TIMER_BASE + HP_LOAD_COUNT1_REG);
	writel(TIMER_EN, HP_TIMER_BASE + HP_CTRL_REG);
}

#define RK3562_OTP_CPU_CODE_OFFSET		0x02
#define RK3562_OTP_SPECIFICATION_OFFSET		0x07

int checkboard(void)
{
	u8 cpu_code[2], specification;
	struct udevice *dev;
	char suffix[2];
	int ret;

	if (!IS_ENABLED(CONFIG_ROCKCHIP_OTP) || !CONFIG_IS_ENABLED(MISC))
		return 0;

	ret = uclass_get_device_by_driver(UCLASS_MISC,
					  DM_DRIVER_GET(rockchip_otp), &dev);
	if (ret) {
		log_debug("Could not find otp device, ret=%d\n", ret);
		return 0;
	}

	/* cpu-code: SoC model, e.g. 0x35 0x62 */
	ret = misc_read(dev, RK3562_OTP_CPU_CODE_OFFSET, cpu_code, 2);
	if (ret < 0) {
		log_debug("Could not read cpu-code, ret=%d\n", ret);
		return 0;
	}

	/* specification: SoC variant, e.g. 0xA for RK3562J */
	ret = misc_read(dev, RK3562_OTP_SPECIFICATION_OFFSET, &specification, 1);
	if (ret < 0) {
		log_debug("Could not read specification, ret=%d\n", ret);
		return 0;
	}
	specification &= 0x1f;

	/* for RK3562J i.e. '@' + 0xA = 'J' */
	suffix[0] = specification > 1 ? '@' + specification : '\0';
	suffix[1] = '\0';

	printf("SoC:   RK%02x%02x%s\n", cpu_code[0], cpu_code[1], suffix);

	return 0;
}

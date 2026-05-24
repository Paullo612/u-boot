// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * (C) Copyright 2019 Amarula Solutions(India)
 * Author: Jagan Teki <jagan@amarulasolutions.com>
 */

#include <dm.h>
#include <env.h>
#include <sysreset.h>
#include <asm/arch-rockchip/cru.h>

const char *get_reset_cause(void)
{
	static char cause[16];
	struct udevice *dev;
	int ret;

	ret = uclass_get_device_by_driver(UCLASS_SYSRESET,
			DM_DRIVER_GET(sysreset_rockchip), &dev);
	if (ret)
		return "unknown reset";

	cause[0] = '\0';
	ret = sysreset_get_status(dev, cause, sizeof(cause));
	if (ret)
		return "unknown reset";

	return cause;
}

#if IS_ENABLED(CONFIG_DISPLAY_CPUINFO)
int print_cpuinfo(void)
{
	const char *cause = get_reset_cause();

	printf("SoC: Rockchip %s\n", CONFIG_SYS_SOC);
	printf("Reset cause: %s\n", cause);

	/**
	 * reset_reason env is used by rk3288, due to special use case
	 * to figure it the boot behavior. so keep this as it is.
	 */
	env_set("reset_reason", cause);

	/* TODO print operating temparature and clock */

	return 0;
}
#endif

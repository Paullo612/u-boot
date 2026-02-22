// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright Contributors to the U-Boot project.

#define LOG_CATEGORY UCLASS_ETH

#include <console.h>
#include <env.h>
#include <hexdump.h>
#include <memalign.h>
#include <time.h>
#include <usb.h>
#include <linux/delay.h>
#include <linux/if_ether.h>
#include <linux/usb/cdc.h>
#include "cdc_common.h"

int cdc_bulk_xfer(struct usb_device *udev, u32 pipe, void *data, int length,
		  int *actual_length, int timeout)
{
	int ret;

	*actual_length = 0;
	ret = usb_bulk_msg(udev, pipe, data, length, actual_length, timeout);

	log_debug("Xfer: length = %u, actual = %u, ret = %d, status = %lx\n",
		  length, *actual_length, ret, udev->status);

	if (ret && udev->status & USB_ST_STALLED)
		usb_clear_halt(udev, pipe);

	return ret;
}

int cdc_get_mac_address(struct usb_device *udev, int index, u8 *data)
{
	u8 tmpbuf[13];
	int ret;

	/* The string descriptor holds the 48bit Ethernet MAC address */
	ret = usb_string(udev, index, tmpbuf, sizeof(tmpbuf));
	if (ret == 12 && !hex2bin(data, tmpbuf, ETH_ALEN))
		return 0;

	memset(data, 0, ETH_ALEN);
	return -EINVAL;
}

int cdc_set_ethernet_packet_filter(struct usb_device *udev, u16 index, u16 value)
{
	int ret;

	/* 6.2.4 SetEthernetPacketFilter [USBECM12] */
	ret = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			      USB_CDC_SET_ETHERNET_PACKET_FILTER,
			      USB_TYPE_CLASS | USB_RECIP_INTERFACE,
			      value, index, NULL, 0, USB_CTRL_SET_TIMEOUT);
	log_debug("SetEthernetPacketFilter(%x): ret=%d, status=%lx\n",
		  value, ret, udev->status);

	return ret;
}

int cdc_set_interface(struct usb_device *udev, u16 index, u16 value)
{
	int ret;

	/* 9.4.10 Set Interface [USB20] */
	ret = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			      USB_REQ_SET_INTERFACE, USB_RECIP_INTERFACE,
			      value, index, NULL, 0, USB_CTRL_SET_TIMEOUT);
	log_debug("SetInterface(%u, %u): ret=%d, status=%lx\n",
		  index, value, ret, udev->status);

	return ret;
}

int cdc_wait_on_connection(struct usb_device *udev, u32 pipe, int interval)
{
	struct usb_cdc_notification *notification;
	ulong start, timeout;
	int maxpacket, ret;
	ALLOC_CACHE_ALIGN_BUFFER(u8, tmpbuf, 64);

	if (!pipe)
		return 0;

	maxpacket = usb_maxpacket(udev, pipe);
	if (maxpacket < sizeof(struct usb_cdc_notification) || maxpacket > 64)
		return -EINVAL;

	timeout = env_get_ulong("phy_aneg_timeout", 10,
				config_opt_enabled(CONFIG_PHYLIB,
						   CONFIG_PHY_ANEG_TIMEOUT, 4000));
	start = get_timer(0);
	do {
		if (ctrlc())
			return -EINTR;

		ret = usb_int_msg(udev, pipe, tmpbuf, maxpacket, interval, false);
		if (ret < 0)
			return ret;

		notification = (struct usb_cdc_notification *)tmpbuf;
		if (!notification->bmRequestType) {
			log_warning("Invalid notification: [%x,%x,%x,%u,%u]\n",
				    notification->bmRequestType,
				    notification->bNotificationType,
				    le16_to_cpu(notification->wValue),
				    le16_to_cpu(notification->wIndex),
				    le16_to_cpu(notification->wLength));
			return -EINVAL;
		}

		switch (notification->bNotificationType) {
		/* 6.3.1 NetworkConnection [USBCDC12] */
		case USB_CDC_NOTIFY_NETWORK_CONNECTION:
			if (le16_to_cpu(notification->wValue) == 1) {
				log_debug("Network connection established\n");
				return 0;
			}
			mdelay(50);
			break;
		}
	} while (get_timer(start) < timeout);

	return -ETIMEDOUT;
}

struct usb_config_descriptor *
cdc_get_config_descriptor(struct usb_device *udev, int cfgno)
{
	struct usb_config_descriptor *config;
	int length, ret;

	length = usb_get_configuration_len(udev, cfgno);
	if (length <= 0)
		return NULL;

	config = malloc_cache_aligned(length);
	if (!config)
		return NULL;

	/* 9.4.3 Get Descriptor [USB20] */
	ret = usb_get_configuration_no(udev, cfgno, (u8 *)config, length);
	if (ret < 0 ||
	    config->bDescriptorType != USB_DT_CONFIG ||
	    config->bLength != USB_DT_CONFIG_SIZE) {
		free(config);
		return NULL;
	}

	return config;
}

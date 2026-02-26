// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright Contributors to the U-Boot project.

#define LOG_CATEGORY UCLASS_ETH

#include <dm.h>
#include <memalign.h>
#include <net-common.h>
#include <usb.h>
#include <linux/usb/cdc.h>
#include "cdc_common.h"

#define RX_BUFFER_SIZE			SZ_2K

struct cdc_ecm_priv {
	struct usb_cdc_ether_desc ether_desc;
	bool normal_operation;
	u16 control;
	u16 data;
	u16 data_default;
	u16 data_normal;
	u32 intpipe;
	int intinterval;
	u32 bulkinpipe;
	u32 bulkoutpipe;
	u8 *rx_buffer;
};

static void cdc_ecm_stop(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ecm_priv *priv = dev_get_priv(dev);

	/* 6.2.4 SetEthernetPacketFilter [USBECM12] */
	cdc_set_ethernet_packet_filter(udev, priv->control, 0);
}

static int cdc_ecm_start(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ecm_priv *priv = dev_get_priv(dev);
	int ret;

	/* 6.2.4 SetEthernetPacketFilter [USBECM12] */
	cdc_set_ethernet_packet_filter(udev, priv->control,
				       USB_CDC_PACKET_TYPE_DIRECTED |
				       USB_CDC_PACKET_TYPE_BROADCAST |
				       USB_CDC_PACKET_TYPE_MULTICAST);

	if (!priv->normal_operation) {
		/* Set non-default state to enable normal operation */
		ret = cdc_set_interface(udev, priv->data, priv->data_normal);
		if (ret < 0)
			return ret;

		priv->normal_operation = true;
	}

	ret = cdc_wait_on_connection(udev, priv->intpipe, priv->intinterval);
	if (ret < 0)
		cdc_ecm_stop(dev);

	return ret;
}

static int cdc_ecm_send(struct udevice *dev, void *packet, int length)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ecm_priv *priv = dev_get_priv(dev);
	int actual_length;

	if (length < ETH_HLEN || length > priv->ether_desc.wMaxSegmentSize)
		return -EINVAL;

	return cdc_bulk_xfer(udev, priv->bulkoutpipe, packet, length,
			     &actual_length, USB_BULK_SEND_TIMEOUT);
}

static int cdc_ecm_recv(struct udevice *dev, int flags, uchar **packetp)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ecm_priv *priv = dev_get_priv(dev);
	int actual_length, ret;

	if (flags & ETH_RECV_CHECK_DEVICE) {
		ret = cdc_bulk_xfer(udev, priv->bulkinpipe,
				    priv->rx_buffer, RX_BUFFER_SIZE,
				    &actual_length, USB_BULK_RECV_TIMEOUT);
		if (!ret && actual_length >= ETH_HLEN) {
			*packetp = priv->rx_buffer;
			return actual_length;
		}
	}

	*packetp = NULL;
	return -EAGAIN;
}

static int cdc_ecm_read_rom_hwaddr(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ecm_priv *priv = dev_get_priv(dev);
	struct eth_pdata *pdata = dev_get_plat(dev);

	return cdc_get_mac_address(udev, priv->ether_desc.iMACAddress,
				   pdata->enetaddr);
}

static int cdc_ecm_parse_config_descriptor(struct udevice *dev,
					   struct usb_config_descriptor *config)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ecm_priv *priv = dev_get_priv(dev);
	struct usb_interface_descriptor *iface_desc;
	struct usb_endpoint_descriptor *ep_desc;
	struct usb_descriptor_header *head;
	bool comm_if_found = false;
	bool data_if_found = false;
	u8 *buffer = (u8 *)config;
	int ep, index;

	priv->control = U16_MAX;
	priv->data = U16_MAX;
	priv->data_default = U16_MAX;
	priv->data_normal = U16_MAX;
	priv->intpipe = 0;
	priv->intinterval = 0;
	priv->bulkinpipe = 0;
	priv->bulkoutpipe = 0;
	memset(&priv->ether_desc, 0, sizeof(priv->ether_desc));

	index = config->bLength;
	head = (struct usb_descriptor_header *)&buffer[index];
	while (index + 1 < config->wTotalLength && head->bLength) {
		if (index + head->bLength > config->wTotalLength)
			return -EINVAL;
		switch (head->bDescriptorType) {
		/* 9.6.5 Interface [USB20] */
		case USB_DT_INTERFACE:
			if (head->bLength != USB_DT_INTERFACE_SIZE)
				return -EINVAL;
			iface_desc = (struct usb_interface_descriptor *)head;
			comm_if_found = (iface_desc->bInterfaceClass == USB_CLASS_COMM &&
				 	 iface_desc->bInterfaceSubClass == USB_CDC_SUBCLASS_ETHERNET &&
					 iface_desc->bInterfaceProtocol == USB_CDC_PROTO_NONE);
			data_if_found = (iface_desc->bInterfaceClass == USB_CLASS_CDC_DATA &&
					 iface_desc->bInterfaceSubClass == 0 &&
					 iface_desc->bInterfaceProtocol == USB_CDC_PROTO_NONE);
			if (comm_if_found) {
				priv->control = iface_desc->bInterfaceNumber;
			} else if (data_if_found && iface_desc->bNumEndpoints == 2) {
				priv->data = iface_desc->bInterfaceNumber;
				priv->data_normal = iface_desc->bAlternateSetting;
			} else if (data_if_found && iface_desc->bNumEndpoints == 0) {
				priv->data_default = iface_desc->bAlternateSetting;
			}
			break;
		/* 9.6.6 Endpoint [USB20] */
		case USB_DT_ENDPOINT:
			if (head->bLength != USB_DT_ENDPOINT_SIZE &&
			    head->bLength != USB_DT_ENDPOINT_AUDIO_SIZE)
				return -EINVAL;
			ep_desc = (struct usb_endpoint_descriptor *)head;
			if (comm_if_found && usb_endpoint_is_int_in(ep_desc)) {
				ep = usb_endpoint_num(ep_desc);
				priv->intpipe = usb_rcvintpipe(udev, ep);
				priv->intinterval = ep_desc->bInterval;
			} else if (data_if_found && usb_endpoint_is_bulk_in(ep_desc)) {
				ep = usb_endpoint_num(ep_desc);
				priv->bulkinpipe = usb_rcvbulkpipe(udev, ep);
			} else if (data_if_found && usb_endpoint_is_bulk_out(ep_desc)) {
				ep = usb_endpoint_num(ep_desc);
				priv->bulkoutpipe = usb_sndbulkpipe(udev, ep);
			}
			break;
		/* 5.2.3 Functional Descriptors [USBCDC12] */
		case USB_DT_CS_INTERFACE:
			if (head->bLength < 4)
				return -EINVAL;
			if (!comm_if_found)
				break;
			switch (buffer[index + 2]) { /* bDescriptorSubType */
			/* 5.4 Ethernet Networking Functional Descriptor [USBECM12] */
			case USB_CDC_ETHERNET_TYPE:
				memcpy(&priv->ether_desc, head, sizeof(priv->ether_desc));
				le32_to_cpus(&priv->ether_desc.bmEthernetStatistics);
				le16_to_cpus(&priv->ether_desc.wMaxSegmentSize);
				le16_to_cpus(&priv->ether_desc.wNumberMCFilters);
				break;
			}
			break;
		}
		index += head->bLength;
		head = (struct usb_descriptor_header *)&buffer[index];
	}

	if (priv->control == U16_MAX || priv->data == U16_MAX ||
	    priv->data_default == U16_MAX || priv->data_normal == U16_MAX ||
	    !priv->bulkinpipe || !priv->bulkoutpipe)
		return -ENODEV;

	return 0;
}

static int cdc_ecm_probe(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ecm_priv *priv = dev_get_priv(dev);
	bool configuration_found = false;
	int cfgno, ret;

	/* Find the CDC ECM configuration */
	for (cfgno = 0; cfgno < udev->descriptor.bNumConfigurations; cfgno++) {
		struct usb_config_descriptor *config;

		/* 9.4.3 Get Descriptor [USB20] */
		config = cdc_get_config_descriptor(udev, cfgno);
		if (!config)
			return -ENODEV;

		ret = cdc_ecm_parse_config_descriptor(dev, config);
		if (!ret) {
			configuration_found = true;
			free(config);
			break;
		}

		free(config);
	}
	if (!configuration_found)
		return -ENODEV;

	/* 9.4.7 Set Configuration [USB20] */
	ret = usb_select_configuration_no(udev, cfgno);
	if (ret < 0)
		return -ENODEV;

	priv->rx_buffer = malloc_cache_aligned(RX_BUFFER_SIZE);
	if (!priv->rx_buffer)
		return -ENOMEM;

	return 0;
}

static int cdc_ecm_remove(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ecm_priv *priv = dev_get_priv(dev);

	if (priv->normal_operation) {
		/* Set default state to disable normal operation */
		cdc_set_interface(udev, priv->data, priv->data_default);
	}

	free(priv->rx_buffer);

	return 0;
}

static const struct eth_ops cdc_ecm_eth_ops = {
	.start = cdc_ecm_start,
	.send = cdc_ecm_send,
	.recv = cdc_ecm_recv,
	.stop = cdc_ecm_stop,
	.read_rom_hwaddr = cdc_ecm_read_rom_hwaddr,
};

U_BOOT_DRIVER(usb_cdc_2_ecm) = {
	.name = "cdc_ecm",
	.id = UCLASS_ETH,
	.probe = cdc_ecm_probe,
	.remove = cdc_ecm_remove,
	.ops = &cdc_ecm_eth_ops,
	.priv_auto = sizeof(struct cdc_ecm_priv),
	.plat_auto = sizeof(struct eth_pdata),
};

static const struct usb_device_id cdc_ecm_id_table[] = {
	/* ASIX AX88179A, AX88772D, AX88279 */
	{ USB_DEVICE_VER(0x0b95, 0x1790, 0x0200, 0x0400), },
	/* Realtek RTL8152, RTL8153, RTL8156, RTL8157 */
	{ USB_DEVICE(0x0bda, 0x8152), },
	{ USB_DEVICE(0x0bda, 0x8153), },
	{ USB_DEVICE(0x0bda, 0x8156), },
	{ USB_DEVICE(0x0bda, 0x8157), },
	/* CDC ECM */
	{ USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_ETHERNET,
			     USB_CDC_PROTO_NONE), },
	{ /* sentinel */ }
};

U_BOOT_USB_DEVICE(usb_cdc_2_ecm, cdc_ecm_id_table);

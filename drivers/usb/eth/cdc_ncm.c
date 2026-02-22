// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright Contributors to the U-Boot project.

#define LOG_CATEGORY UCLASS_ETH

#include <dm.h>
#include <memalign.h>
#include <net-common.h>
#include <usb.h>
#include <linux/usb/cdc.h>
#include "cdc_common.h"

struct cdc_ncm_priv {
	struct usb_cdc_ether_desc ether_desc;
	struct usb_cdc_ncm_desc ncm_desc;
	struct usb_cdc_ncm_ntb_parameters ncm_ntb_parameters;
	bool normal_operation;
	u16 control;
	u16 data;
	u32 intpipe;
	int intinterval;
	u32 bulkinpipe;
	u32 bulkoutpipe;
	u32 rx_size;
	u8 *rx_buffer;
	struct usb_cdc_ncm_dpe16 *rx_dpe;
	u16 tx_ndp_alignment;
	u16 tx_sequence;
};

static void cdc_ncm_stop(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ncm_priv *priv = dev_get_priv(dev);

	if (priv->ncm_desc.bmNetworkCapabilities & USB_CDC_NCM_NCAP_ETH_FILTER) {
		/* 6.2.4 SetEthernetPacketFilter [USBECM12] */
		cdc_set_ethernet_packet_filter(udev, priv->control, 0);
	}
}

static int cdc_ncm_start(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ncm_priv *priv = dev_get_priv(dev);
	int ret;

	if (priv->ncm_desc.bmNetworkCapabilities & USB_CDC_NCM_NCAP_ETH_FILTER) {
		/* 6.2.4 SetEthernetPacketFilter [USBECM12] */
		cdc_set_ethernet_packet_filter(udev, priv->control,
					       USB_CDC_PACKET_TYPE_DIRECTED |
					       USB_CDC_PACKET_TYPE_BROADCAST |
					       USB_CDC_PACKET_TYPE_MULTICAST);
	}

	if (!priv->normal_operation) {
		/* Reset transmit sequence */
		priv->tx_sequence = 0;

		/* 7.2.6 SetNtbInputSize [USBNCM11] */
		cdc_set_ntb_input_size(udev, priv->control, priv->rx_size);

		/* Set non-initial state (AltSet=1) to enable normal operation */
		ret = cdc_set_interface(udev, priv->data, 1);
		if (ret < 0)
			return ret;

		priv->normal_operation = true;
	}

	ret = cdc_wait_on_connection(udev, priv->intpipe, priv->intinterval);
	if (ret < 0)
		cdc_ncm_stop(dev);

	return ret;
}

static int cdc_ncm_send(struct udevice *dev, void *packet, int length)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ncm_priv *priv = dev_get_priv(dev);
	struct usb_cdc_ncm_nth16 *nth;
	struct usb_cdc_ncm_ndp16 *ndp;
	struct usb_cdc_ncm_dpe16 *dpe;
	int actual_length, offset;
	ALLOC_CACHE_ALIGN_BUFFER(u8, tmpbuf, SZ_2K);

	if (length < ETH_HLEN || length > SZ_2K)
		return -EINVAL;

	/* 3.2.1 NTH for 16-bit NTB (NTH16) [USBNCM11] */
	nth = (struct usb_cdc_ncm_nth16 *)tmpbuf;
	nth->dwSignature = cpu_to_le32(USB_CDC_NCM_NTH16_SIGN);
	nth->wHeaderLength = cpu_to_le16(sizeof(struct usb_cdc_ncm_nth16));
	nth->wSequence = cpu_to_le16(priv->tx_sequence++);
	offset = ALIGN(le16_to_cpu(nth->wHeaderLength), priv->tx_ndp_alignment);
	nth->wNdpIndex = cpu_to_le16(offset);

	/* 3.3.1 NDP for 16-bit NTBs (NDP16) [USBNCM11] */
	ndp = (struct usb_cdc_ncm_ndp16 *)(tmpbuf + offset);
	ndp->dwSignature = cpu_to_le32(USB_CDC_NCM_NDP16_NOCRC_SIGN);
	ndp->wLength = cpu_to_le16(sizeof(struct usb_cdc_ncm_ndp16) +
				   sizeof(struct usb_cdc_ncm_dpe16) * 2);
	ndp->wNextNdpIndex = cpu_to_le16(0);

	dpe = (struct usb_cdc_ncm_dpe16 *)ndp->dpe16;
	// FIXME: align using wNdpOutDivisor and wNdpOutPayloadRemainder ?
	offset += ALIGN(le16_to_cpu(ndp->wLength), 16);
	dpe[0].wDatagramIndex = cpu_to_le16(offset);
	dpe[0].wDatagramLength = cpu_to_le16(length);

	/* 3.7 Null NCM Datagram Pointer Entries [USBNCM11] */
	dpe[1].wDatagramIndex = cpu_to_le16(0);
	dpe[1].wDatagramLength = cpu_to_le16(0);

	nth->wBlockLength = cpu_to_le16(offset + length);
	if (le16_to_cpu(nth->wBlockLength) > SZ_2K)
		return -EINVAL;

	memcpy(tmpbuf + offset, packet, length);

	return cdc_bulk_xfer(udev, priv->bulkoutpipe,
			     tmpbuf, le16_to_cpu(nth->wBlockLength),
			     &actual_length, USB_BULK_SEND_TIMEOUT);
}

static struct usb_cdc_ncm_dpe16 *cdc_ncm_validate_nth(void *buffer, int size)
{
	struct usb_cdc_ncm_nth16 *nth = buffer;
	struct usb_cdc_ncm_ndp16 *ndp;
	struct usb_cdc_ncm_dpe16 *dpe;

	/* 3.2.1 NTH for 16-bit NTB (NTH16) [USBNCM11] */
	if (le32_to_cpu(nth->dwSignature) != USB_CDC_NCM_NTH16_SIGN ||
	    le16_to_cpu(nth->wHeaderLength) != sizeof(struct usb_cdc_ncm_nth16) ||
	    le16_to_cpu(nth->wBlockLength) > size ||
	    le16_to_cpu(nth->wNdpIndex) % 4 ||
	    le16_to_cpu(nth->wNdpIndex) < sizeof(struct usb_cdc_ncm_nth16) ||
	    le16_to_cpu(nth->wBlockLength) < (le16_to_cpu(nth->wNdpIndex) +
					      sizeof(struct usb_cdc_ncm_ndp16))) {
		log_warning("Invalid NTH16: [%08x,%u,%u,%u,%u]\n",
			    le32_to_cpu(nth->dwSignature),
			    le16_to_cpu(nth->wHeaderLength),
			    le16_to_cpu(nth->wSequence),
			    le16_to_cpu(nth->wBlockLength),
			    le16_to_cpu(nth->wNdpIndex));
		return NULL;
	}

	/* 3.3.1 NDP for 16-bit NTBs (NDP16) [USBNCM11] */
	buffer += le16_to_cpu(nth->wNdpIndex);
	ndp = (struct usb_cdc_ncm_ndp16 *)buffer;
	if (le32_to_cpu(ndp->dwSignature) != USB_CDC_NCM_NDP16_NOCRC_SIGN ||
	    le16_to_cpu(ndp->wLength) % 4 ||
	    le16_to_cpu(ndp->wLength) < (sizeof(struct usb_cdc_ncm_ndp16) +
					 sizeof(struct usb_cdc_ncm_dpe16) * 2) ||
	    le16_to_cpu(ndp->wNextNdpIndex) != 0 ||
	    le16_to_cpu(nth->wBlockLength) < (le16_to_cpu(nth->wNdpIndex) +
					      le16_to_cpu(ndp->wLength))) {
		log_warning("Invalid NDP16: [%08x,%u,%u]\n",
			    le32_to_cpu(ndp->dwSignature),
			    le16_to_cpu(ndp->wLength),
			    le16_to_cpu(ndp->wNextNdpIndex));
		return NULL;
	}

	/* Last DPE is always zero */
	dpe = (struct usb_cdc_ncm_dpe16 *)(buffer + le16_to_cpu(ndp->wLength) -
					   sizeof(struct usb_cdc_ncm_dpe16));
	if (le16_to_cpu(dpe->wDatagramIndex) ||
	    le16_to_cpu(dpe->wDatagramLength))
	    return NULL;

	/* First DPE should not be a Null entry */
	dpe = ndp->dpe16;
	if (!le16_to_cpu(dpe->wDatagramIndex) ||
	    !le16_to_cpu(dpe->wDatagramLength))
		return NULL;

	do {
		/* 3.7 Null NCM Datagram Pointer Entries [USBNCM11] */
		if (!le16_to_cpu(dpe->wDatagramIndex) ||
		    !le16_to_cpu(dpe->wDatagramLength))
		    break;

		if (le16_to_cpu(dpe->wDatagramIndex) < sizeof(struct usb_cdc_ncm_nth16) ||
		    le16_to_cpu(dpe->wDatagramLength) < ETH_HLEN ||
		    le16_to_cpu(dpe->wDatagramIndex) + le16_to_cpu(dpe->wDatagramLength) >
		    le16_to_cpu(nth->wBlockLength)) {
			log_warning("Invalid DPE16: [%u,%u]\n",
				    le16_to_cpu(dpe->wDatagramIndex),
				    le16_to_cpu(dpe->wDatagramLength));
			return NULL;
		}
	} while (++dpe);

	return ndp->dpe16;
}

static int cdc_ncm_recv(struct udevice *dev, int flags, uchar **packetp)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ncm_priv *priv = dev_get_priv(dev);
	int actual_length, ret;

	if (!priv->rx_dpe && (flags & ETH_RECV_CHECK_DEVICE)) {
		ret = cdc_bulk_xfer(udev, priv->bulkinpipe,
				    priv->rx_buffer, priv->rx_size,
				    &actual_length, USB_BULK_RECV_TIMEOUT);
		if (!ret && actual_length)
			priv->rx_dpe = cdc_ncm_validate_nth(priv->rx_buffer,
							    actual_length);
	}

	if (priv->rx_dpe) {
		*packetp = priv->rx_buffer + le16_to_cpu(priv->rx_dpe->wDatagramIndex);
		return le16_to_cpu(priv->rx_dpe->wDatagramLength);
	}

	*packetp = NULL;
	return -EAGAIN;
}

static int cdc_ncm_free_pkt(struct udevice *dev, uchar *packet, int length)
{
	struct cdc_ncm_priv *priv = dev_get_priv(dev);

	if (priv->rx_dpe) {
		/* Advance to the next DPE */
		priv->rx_dpe++;

		/* Stop at first Null entry */
		if (!le16_to_cpu(priv->rx_dpe->wDatagramIndex) ||
		    !le16_to_cpu(priv->rx_dpe->wDatagramLength))
			priv->rx_dpe = NULL;
	}

	return 0;
}

static int cdc_ncm_write_hwaddr(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ncm_priv *priv = dev_get_priv(dev);
	struct eth_pdata *pdata = dev_get_plat(dev);

	if (!(priv->ncm_desc.bmNetworkCapabilities & USB_CDC_NCM_NCAP_NET_ADDRESS))
		return -ENOSYS;

	/* 7.2.3 SetNetAddress [USBNCM11] */
	return cdc_set_net_address(udev, priv->control, pdata->enetaddr);
}

static int cdc_ncm_read_rom_hwaddr(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ncm_priv *priv = dev_get_priv(dev);
	struct eth_pdata *pdata = dev_get_plat(dev);

	if (!(priv->ncm_desc.bmNetworkCapabilities & USB_CDC_NCM_NCAP_NET_ADDRESS))
		return cdc_get_mac_address(udev, priv->ether_desc.iMACAddress,
					   pdata->enetaddr);

	/* 7.2.2 GetNetAddress [USBNCM11] */
	return cdc_get_net_address(udev, priv->control, pdata->enetaddr);
}

static int cdc_ncm_parse_config_descriptor(struct udevice *dev,
					   struct usb_config_descriptor *config)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ncm_priv *priv = dev_get_priv(dev);
	struct usb_interface_descriptor *iface_desc;
	struct usb_endpoint_descriptor *ep_desc;
	struct usb_descriptor_header *head;
	bool comm_if_found = false;
	bool data_if_found = false;
	u8 *buffer = (u8 *)config;
	int ep, index;

	priv->control = U16_MAX;
	priv->data = U16_MAX;
	priv->intpipe = 0;
	priv->intinterval = 0;
	priv->bulkinpipe = 0;
	priv->bulkoutpipe = 0;
	memset(&priv->ether_desc, 0, sizeof(priv->ether_desc));
	memset(&priv->ncm_desc, 0, sizeof(priv->ncm_desc));

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
					 iface_desc->bInterfaceSubClass == USB_CDC_SUBCLASS_NCM &&
					 iface_desc->bInterfaceProtocol == USB_CDC_PROTO_NONE);
			data_if_found = (iface_desc->bInterfaceClass == USB_CLASS_CDC_DATA &&
					 iface_desc->bInterfaceSubClass == 0 &&
					 iface_desc->bInterfaceProtocol == USB_CDC_NCM_PROTO_NTB &&
					 iface_desc->bAlternateSetting == 1 &&
					 iface_desc->bNumEndpoints == 2);
			if (comm_if_found)
				priv->control = iface_desc->bInterfaceNumber;
			else if (data_if_found)
				priv->data = iface_desc->bInterfaceNumber;
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
			/* 6.2.1 NCM Functional Descriptor [USBNCM11] */
			case USB_CDC_NCM_TYPE:
				memcpy(&priv->ncm_desc, head, sizeof(priv->ncm_desc));
				le16_to_cpus(&priv->ncm_desc.bcdNcmVersion);
				break;
			}
			break;
		}
		index += head->bLength;
		head = (struct usb_descriptor_header *)&buffer[index];
	}

	if (priv->control == U16_MAX || priv->data == U16_MAX ||
	    !priv->intpipe || !priv->bulkinpipe || !priv->bulkoutpipe)
		return -ENODEV;

	return 0;
}

static int cdc_ncm_probe(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ncm_priv *priv = dev_get_priv(dev);
	bool configuration_found = false;
	int cfgno, ret;

	/* Find the CDC NCM configuration */
	for (cfgno = 0; cfgno < udev->descriptor.bNumConfigurations; cfgno++) {
		struct usb_config_descriptor *config;

		/* 9.4.3 Get Descriptor [USB20] */
		config = cdc_get_config_descriptor(udev, cfgno);
		if (!config)
			return -ENODEV;

		ret = cdc_ncm_parse_config_descriptor(dev, config);
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

	/* 7.2.1 GetNtbParameters [USBNCM11] */
	ret = cdc_get_ntb_parameters(udev, priv->control,
				     &priv->ncm_ntb_parameters);
	if (ret < 0)
		return -ENODEV;

	priv->tx_ndp_alignment = max_t(u32, USB_CDC_NCM_NDP_ALIGN_MIN_SIZE,
				       priv->ncm_ntb_parameters.wNdpOutAlignment);
	if (priv->tx_ndp_alignment > SZ_512)
		return -EINVAL;

	/*
	 * The NTB-16 format used by this driver can support NTB sizes up to 64
	 * KiB and the host must support NTBs of at least 2 KiB. Support up to
	 * 16 KiB NTBs as a reasonable default and limit it to the maximum
	 * supported by the device and the host controller.
	 */

	priv->rx_size = SZ_16K;
	if (priv->ncm_ntb_parameters.dwNtbInMaxSize)
		priv->rx_size = min(priv->rx_size,
				    priv->ncm_ntb_parameters.dwNtbInMaxSize);

	if (CONFIG_IS_ENABLED(DM_USB)) {
		size_t size = 0;

		ret = usb_get_max_xfer_size(udev, &size);
		if (ret >= 0 && size < priv->rx_size)
			priv->rx_size = size;
	}

	if (priv->rx_size < USB_CDC_NCM_NTB_MIN_IN_SIZE)
		priv->rx_size = USB_CDC_NCM_NTB_MIN_IN_SIZE;

	priv->rx_buffer = malloc_cache_aligned(priv->rx_size);
	if (!priv->rx_buffer)
		return -ENOMEM;

	return 0;
}

static int cdc_ncm_remove(struct udevice *dev)
{
	struct usb_device *udev = dev_get_parent_priv(dev);
	struct cdc_ncm_priv *priv = dev_get_priv(dev);

	if (priv->normal_operation) {
		/* Set initial state (AltSet=0) to disable networking traffic */
		cdc_set_interface(udev, priv->data, 0);
	}

	free(priv->rx_buffer);

	return 0;
}

static const struct eth_ops cdc_ncm_eth_ops = {
	.start = cdc_ncm_start,
	.send = cdc_ncm_send,
	.recv = cdc_ncm_recv,
	.free_pkt = cdc_ncm_free_pkt,
	.stop = cdc_ncm_stop,
	.write_hwaddr = cdc_ncm_write_hwaddr,
	.read_rom_hwaddr = cdc_ncm_read_rom_hwaddr,
};

U_BOOT_DRIVER(usb_cdc_1_ncm) = {
	.name = "cdc_ncm",
	.id = UCLASS_ETH,
	.probe = cdc_ncm_probe,
	.remove = cdc_ncm_remove,
	.ops = &cdc_ncm_eth_ops,
	.priv_auto = sizeof(struct cdc_ncm_priv),
	.plat_auto = sizeof(struct eth_pdata),
};

static const struct usb_device_id cdc_ncm_id_table[] = {
	/* ASIX AX88179A, AX88772D, AX88279 */
	{ USB_DEVICE_VER(0x0b95, 0x1790, 0x0200, 0x0400), },
	/* CDC NCM */
	{ USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_NCM,
			     USB_CDC_PROTO_NONE), },
	{ /* sentinel */ }
};

U_BOOT_USB_DEVICE(usb_cdc_1_ncm, cdc_ncm_id_table);

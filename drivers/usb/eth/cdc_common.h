// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright Contributors to the U-Boot project.

#ifndef _USB_ETH_CDC_COMMON_H
#define _USB_ETH_CDC_COMMON_H

#define USB_BULK_RECV_TIMEOUT		500
#define USB_BULK_SEND_TIMEOUT		5000
#define USB_CTRL_GET_TIMEOUT		5000
#define USB_CTRL_SET_TIMEOUT		5000

int cdc_bulk_xfer(struct usb_device *udev, u32 pipe, void *data, int length,
		  int *actual_length, int timeout);

int cdc_get_mac_address(struct usb_device *udev, int index, u8 *data);

int cdc_get_net_address(struct usb_device *udev, int index, u8 *data);

int cdc_get_ntb_parameters(struct usb_device *udev, u16 index,
			   struct usb_cdc_ncm_ntb_parameters *params);

int cdc_set_ethernet_packet_filter(struct usb_device *udev, u16 index, u16 value);

int cdc_set_interface(struct usb_device *udev, u16 index, u16 value);

int cdc_set_net_address(struct usb_device *udev, u16 index, const u8 *data);

int cdc_set_ntb_input_size(struct usb_device *udev, u16 index, u32 value);

int cdc_wait_on_connection(struct usb_device *udev, u32 pipe, int interval);

struct usb_config_descriptor *cdc_get_config_descriptor(struct usb_device *udev,
							int cfgno);

#endif

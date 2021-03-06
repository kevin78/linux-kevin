/*
 * Copyright (c) 2014-2015 Hisilicon Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __HNS_ENET_H
#define __HNS_ENET_H

#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/of_net.h>
#include <linux/of_mdio.h>
#include <linux/netdevice.h>
#include "hnae.h"

enum hns_nic_state {
	NIC_STATE_TESTING = 0,
	NIC_STATE_RESETTING,
	NIC_STATE_REINITING,
	NIC_STATE_DOWN,
	NIC_STATE_DISABLED,
	NIC_STATE_REMOVING,
	NIC_STATE_SERVICE_INITED,
	NIC_STATE_SERVICE_SCHED,
	NIC_STATE2_RESET_REQUESTED,
	NIC_STATE_MAX
};

struct hns_nic_ring_data {
	struct hnae_ring *ring;
	struct napi_struct napi;
	int queue_index;
	int (*poll_one)(struct hns_nic_ring_data *, int, void *);
	void (*ex_process)(struct hns_nic_ring_data *, struct sk_buff *);
	void (*fini_process)(struct hns_nic_ring_data *);
};

struct hns_nic_priv {
	const char *ae_name;
	const char *ae_opts;
	int phy_mode;
	int phy_led_val;
	struct phy_device *phy;
	struct net_device *netdev;
	struct device *dev;
	struct hnae_handle *ae_handle;

	/* the cb for nic to manage the ring buffer, the first half of the
	 * array is for tx_ring and vice versa for the second half
	 */
	struct hns_nic_ring_data *ring_data;

	/* The most recently read link state */
	int link;
	u64 tx_timeout_count;

	unsigned long state;

	struct timer_list service_timer;

	struct work_struct service_task;

	struct notifier_block notifier_block;
};

#define tx_ring_data(priv, idx) ((priv)->ring_data[idx])
#define rx_ring_data(priv, idx) \
	((priv)->ring_data[(priv)->ae_handle->q_num + (idx)])

void hns_ethtool_set_ops(struct net_device *ndev);
void hns_nic_net_reset(struct net_device *ndev);
void hns_nic_net_reinit(struct net_device *netdev);
int hns_nic_init_phy(struct net_device *ndev, struct hnae_handle *h);
int hns_nic_net_xmit_hw(struct net_device *ndev,
			struct sk_buff *skb,
			struct hns_nic_ring_data *ring_data);

#endif	/**__HNS_ENET_H */

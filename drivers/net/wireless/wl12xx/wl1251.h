/*
 * This file is part of wl1251
 *
 * Copyright (c) 1998-2007 Texas Instruments Incorporated
 * Copyright (C) 2008-2009 Nokia Corporation
 *
 * Contact: Kalle Valo <kalle.valo@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef __WL1251_H__
#define __WL1251_H__

#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/bitops.h>
#include <net/mac80211.h>
#include <mach/board.h>
#include <mach/board-nokia.h>

#define DRIVER_NAME "wl1251"
#define DRIVER_PREFIX DRIVER_NAME ": "

enum {
	DEBUG_NONE	= 0,
	DEBUG_IRQ	= BIT(0),
	DEBUG_SPI	= BIT(1),
	DEBUG_BOOT	= BIT(2),
	DEBUG_MAILBOX	= BIT(3),
	DEBUG_NETLINK	= BIT(4),
	DEBUG_EVENT	= BIT(5),
	DEBUG_TX	= BIT(6),
	DEBUG_RX	= BIT(7),
	DEBUG_SCAN	= BIT(8),
	DEBUG_CRYPT	= BIT(9),
	DEBUG_PSM	= BIT(10),
	DEBUG_MAC80211	= BIT(11),
	DEBUG_CMD	= BIT(12),
	DEBUG_ACX	= BIT(13),
	DEBUG_ALL	= ~0,
};

#define DEBUG_LEVEL (DEBUG_NONE)

#define DEBUG_DUMP_LIMIT 1024

#define wl1251_error(fmt, arg...) \
	printk(KERN_ERR DRIVER_PREFIX "ERROR " fmt "\n", ##arg)

#define wl1251_warning(fmt, arg...) \
	printk(KERN_WARNING DRIVER_PREFIX "WARNING " fmt "\n", ##arg)

#define wl1251_notice(fmt, arg...) \
	printk(KERN_INFO DRIVER_PREFIX fmt "\n", ##arg)

#define wl1251_info(fmt, arg...) \
	printk(KERN_DEBUG DRIVER_PREFIX fmt "\n", ##arg)

#define wl1251_debug(level, fmt, arg...) \
	do { \
		if (level & DEBUG_LEVEL) \
			printk(KERN_DEBUG DRIVER_PREFIX fmt "\n", ##arg); \
	} while (0)

#define wl1251_dump(level, prefix, buf, len)	\
	do { \
		if (level & DEBUG_LEVEL) \
			print_hex_dump(KERN_DEBUG, DRIVER_PREFIX prefix, \
				       DUMP_PREFIX_OFFSET, 16, 1,	\
				       buf,				\
				       min_t(size_t, len, DEBUG_DUMP_LIMIT), \
				       0);				\
	} while (0)

#define wl1251_dump_ascii(level, prefix, buf, len)	\
	do { \
		if (level & DEBUG_LEVEL) \
			print_hex_dump(KERN_DEBUG, DRIVER_PREFIX prefix, \
				       DUMP_PREFIX_OFFSET, 16, 1,	\
				       buf,				\
				       min_t(size_t, len, DEBUG_DUMP_LIMIT), \
				       true);				\
	} while (0)

#define WL1251_DEFAULT_RX_CONFIG (CFG_UNI_FILTER_EN |	\
				  CFG_BSSID_FILTER_EN | \
				  CFG_MC_FILTER_EN)

#define WL1251_DEFAULT_RX_FILTER (CFG_RX_PRSP_EN |  \
				  CFG_RX_MGMT_EN |  \
				  CFG_RX_DATA_EN |  \
				  CFG_RX_CTL_EN |   \
				  CFG_RX_BCN_EN |   \
				  CFG_RX_AUTH_EN |  \
				  CFG_RX_ASSOC_EN)

#define WL1251_BUSY_WORD_LEN 8

struct boot_attr {
	u32 radio_type;
	u8 mac_clock;
	u8 arm_clock;
	int firmware_debug;
	u32 minor;
	u32 major;
	u32 bugfix;
};

enum wl1251_state {
	WL1251_STATE_OFF,
	WL1251_STATE_ON,
	WL1251_STATE_PLT,
};

enum wl1251_partition_type {
	PART_DOWN,
	PART_WORK,
	PART_DRPW,

	PART_TABLE_LEN
};

struct wl1251_partition {
	u32 size;
	u32 start;
};

struct wl1251_partition_set {
	struct wl1251_partition mem;
	struct wl1251_partition reg;
};

struct wl1251_stats {
	struct acx_statistics *fw_stats;
	unsigned long fw_stats_update;

	unsigned int retry_count;
	unsigned int excessive_retries;
};

struct wl1251_debugfs {
	struct dentry *rootdir;
	struct dentry *fw_statistics;

	struct dentry *tx_internal_desc_overflow;

	struct dentry *rx_out_of_mem;
	struct dentry *rx_hdr_overflow;
	struct dentry *rx_hw_stuck;
	struct dentry *rx_dropped;
	struct dentry *rx_fcs_err;
	struct dentry *rx_xfr_hint_trig;
	struct dentry *rx_path_reset;
	struct dentry *rx_reset_counter;

	struct dentry *dma_rx_requested;
	struct dentry *dma_rx_errors;
	struct dentry *dma_tx_requested;
	struct dentry *dma_tx_errors;

	struct dentry *isr_cmd_cmplt;
	struct dentry *isr_fiqs;
	struct dentry *isr_rx_headers;
	struct dentry *isr_rx_mem_overflow;
	struct dentry *isr_rx_rdys;
	struct dentry *isr_irqs;
	struct dentry *isr_tx_procs;
	struct dentry *isr_decrypt_done;
	struct dentry *isr_dma0_done;
	struct dentry *isr_dma1_done;
	struct dentry *isr_tx_exch_complete;
	struct dentry *isr_commands;
	struct dentry *isr_rx_procs;
	struct dentry *isr_hw_pm_mode_changes;
	struct dentry *isr_host_acknowledges;
	struct dentry *isr_pci_pm;
	struct dentry *isr_wakeups;
	struct dentry *isr_low_rssi;

	struct dentry *wep_addr_key_count;
	struct dentry *wep_default_key_count;
	/* skipping wep.reserved */
	struct dentry *wep_key_not_found;
	struct dentry *wep_decrypt_fail;
	struct dentry *wep_packets;
	struct dentry *wep_interrupt;

	struct dentry *pwr_ps_enter;
	struct dentry *pwr_elp_enter;
	struct dentry *pwr_missing_bcns;
	struct dentry *pwr_wake_on_host;
	struct dentry *pwr_wake_on_timer_exp;
	struct dentry *pwr_tx_with_ps;
	struct dentry *pwr_tx_without_ps;
	struct dentry *pwr_rcvd_beacons;
	struct dentry *pwr_power_save_off;
	struct dentry *pwr_enable_ps;
	struct dentry *pwr_disable_ps;
	struct dentry *pwr_fix_tsf_ps;
	/* skipping cont_miss_bcns_spread for now */
	struct dentry *pwr_rcvd_awake_beacons;

	struct dentry *mic_rx_pkts;
	struct dentry *mic_calc_failure;

	struct dentry *aes_encrypt_fail;
	struct dentry *aes_decrypt_fail;
	struct dentry *aes_encrypt_packets;
	struct dentry *aes_decrypt_packets;
	struct dentry *aes_encrypt_interrupt;
	struct dentry *aes_decrypt_interrupt;

	struct dentry *event_heart_beat;
	struct dentry *event_calibration;
	struct dentry *event_rx_mismatch;
	struct dentry *event_rx_mem_empty;
	struct dentry *event_rx_pool;
	struct dentry *event_oom_late;
	struct dentry *event_phy_transmit_error;
	struct dentry *event_tx_stuck;

	struct dentry *ps_pspoll_timeouts;
	struct dentry *ps_upsd_timeouts;
	struct dentry *ps_upsd_max_sptime;
	struct dentry *ps_upsd_max_apturn;
	struct dentry *ps_pspoll_max_apturn;
	struct dentry *ps_pspoll_utilization;
	struct dentry *ps_upsd_utilization;

	struct dentry *rxpipe_rx_prep_beacon_drop;
	struct dentry *rxpipe_descr_host_int_trig_rx_data;
	struct dentry *rxpipe_beacon_buffer_thres_host_int_trig_rx_data;
	struct dentry *rxpipe_missed_beacon_host_int_trig_rx_data;
	struct dentry *rxpipe_tx_xfr_host_int_trig_rx_data;

	struct dentry *tx_queue_len;
	struct dentry *tx_queue_status;

	struct dentry *retry_count;
	struct dentry *excessive_retries;
};

enum wl1251_bt_coex_mode {
	WL1251_BT_COEX_OFF,
	WL1251_BT_COEX_ENABLE,
	WL1251_BT_COEX_MONOAUDIO
};

struct wl1251 {
	struct ieee80211_hw *hw;
	bool mac80211_registered;

	struct spi_device *spi;

	void (*set_power)(bool enable);
	int irq;

	spinlock_t wl_lock;

	enum wl1251_state state;
	struct mutex mutex;

	int physical_mem_addr;
	int physical_reg_addr;
	int virtual_mem_addr;
	int virtual_reg_addr;

	int cmd_box_addr;
	int event_box_addr;
	struct boot_attr boot_attr;

	u8 *fw;
	size_t fw_len;
	u8 *nvs;
	size_t nvs_len;

	u8 bssid[ETH_ALEN];
	u8 mac_addr[ETH_ALEN];
	u8 bss_type;
	u8 ssid[IW_ESSID_MAX_SIZE + 1];
	u8 ssid_len;
	u8 listen_int;
	int channel;

	void *target_mem_map;
	struct acx_data_path_params_resp *data_path;

	/* Accounting for allocated / available TX blocks on HW */
	u32 tx_blocks_available;
	u32 tx_descriptors_available;
	u32 tx_results_count;

	/* Transmitted TX packets counter for chipset interface */
	int tx_packets_count;

	/* Time-offset between host and chipset clocks */
	int time_offset;

	/* Session counter for the chipset */
	int session_counter;

	/* Number of TX packets transferred to the FW, modulo 16 */
	u32 data_in_count;

	/* Frames scheduled for transmission, not handled yet */
	struct sk_buff_head tx_queue;
	bool tx_queue_stopped;

	struct work_struct tx_work;
	struct work_struct filter_work;
	struct wl1251_filter_params *filter_params;

	/* Pending TX frames */
	struct sk_buff *tx_frames[16];

	/*
	 * Index pointing to the next TX complete entry
	 * in the cyclic XT complete array we get from
	 * the FW.
	 */
	u32 next_tx_complete;

	/* FW Rx counter */
	u32 rx_counter;

	/* Rx frames handled */
	u32 rx_handled;

	/* Current double buffer */
	u32 rx_current_buffer;
	u32 rx_last_id;

	/* The target interrupt mask */
	u32 intr_mask;
	struct work_struct irq_work;

	/* The mbox event mask */
	u32 event_mask;

	/* Mailbox pointers */
	u32 mbox_ptr[2];

	/* Are we currently scanning */
	bool scanning;

	unsigned long last_event;

	/* Our association ID */
	u16 aid;

	/* Default key (for WEP) */
	u32 default_key;

	unsigned int tx_mgmt_frm_rate;
	unsigned int tx_mgmt_frm_mod;

	unsigned int rx_config;
	unsigned int rx_filter;

	/* is firmware in elp mode */
	bool elp;

	struct delayed_work elp_work;

	/* we can be in psm, but not in elp, we have to differentiate */
	bool psm;

	/* PSM mode requested */
	bool psm_requested;

	u8 ps_entry_retry;

	u16 beacon_int;
	u8 dtim_period;

	/* in dBm */
	int power_level;

	struct wl1251_stats stats;
	struct wl1251_debugfs debugfs;

	u32 buffer_32;
	u32 buffer_cmd;
	u8 buffer_busyword[WL1251_BUSY_WORD_LEN];
	struct wl1251_rx_descriptor *rx_descriptor;

	struct wl1271_fw_status *fw_status;

	struct ieee80211_vif *vif;

	enum wl1251_bt_coex_mode bt_coex_mode;

	u32 chip_id;
	char fw_ver[21];
};

int wl1251_plt_start(struct wl1251 *wl);
int wl1251_plt_stop(struct wl1251 *wl);

#define DEFAULT_HW_GEN_MODULATION_TYPE    CCK_LONG /* Long Preamble */
#define DEFAULT_HW_GEN_TX_RATE          RATE_2MBPS
#define JOIN_TIMEOUT 5000 /* 5000 milliseconds to join */

#define SESSION_COUNTER_MAX 7 /* maximum value for the session counter */

#define WL1251_DEFAULT_POWER_LEVEL 20

#define WL1251_TX_QUEUE_MAX_LENGTH 20

#define WL1251_DEFAULT_BEACON_INT 100
#define WL1251_DEFAULT_DTIM_PERIOD 1

#define WL1251_DEFAULT_CHANNEL 0

#define WL1251_DEFAULT_BET_CONSECUTIVE 10

#define CHIP_ID_1251_PG10	           (0x7010101)
#define CHIP_ID_1251_PG11	           (0x7020101)
#define CHIP_ID_1251_PG12	           (0x7030101)
#define CHIP_ID_1271_PG10	           (0x4030101)
#define CHIP_ID_1271_PG20	           (0x4030111)

#define WL1251_FW_NAME "wl1251-fw.bin"
#define WL1251_NVS_NAME "wl1251-nvs.bin"

#define WL1251_POWER_ON_SLEEP 10 /* in miliseconds */

#define WL1251_PART_DOWN_MEM_START	0x0
#define WL1251_PART_DOWN_MEM_SIZE	0x16800
#define WL1251_PART_DOWN_REG_START	REGISTERS_BASE
#define WL1251_PART_DOWN_REG_SIZE	REGISTERS_DOWN_SIZE

#define WL1251_PART_WORK_MEM_START	0x28000
#define WL1251_PART_WORK_MEM_SIZE	0x14000
#define WL1251_PART_WORK_REG_START	REGISTERS_BASE
#define WL1251_PART_WORK_REG_SIZE	REGISTERS_WORK_SIZE

#define WL12XX_DEFAULT_LOW_RSSI_THRESHOLD       -75
#define WL12XX_DEFAULT_LOW_RSSI_WEIGHT          10
#define WL12XX_DEFAULT_LOW_RSSI_DEPTH           10

#endif

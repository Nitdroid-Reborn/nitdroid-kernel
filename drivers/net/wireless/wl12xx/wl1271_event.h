/*
 * This file is part of wl1271
 *
 * Copyright (c) 1998-2007 Texas Instruments Incorporated
 * Copyright (C) 2008 Nokia Corporation
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

#ifndef __WL1271_EVENT_H__
#define __WL1271_EVENT_H__

/*
 * Mbox events
 *
 * The event mechanism is based on a pair of event buffers (buffers A and
 * B) at fixed locations in the target's memory. The host processes one
 * buffer while the other buffer continues to collect events. If the host
 * is not processing events, an interrupt is issued to signal that a buffer
 * is ready. Once the host is done with processing events from one buffer,
 * it signals the target (with an ACK interrupt) that the event buffer is
 * free.
 */

enum {
	MEASUREMENT_START_EVENT_ID		 = BIT(8),
	MEASUREMENT_COMPLETE_EVENT_ID		 = BIT(9),
	SCAN_COMPLETE_EVENT_ID			 = BIT(10),
	SCHEDULED_SCAN_COMPLETE_EVENT_ID	 = BIT(11),
	AP_DISCOVERY_COMPLETE_EVENT_ID		 = BIT(12),
	PS_REPORT_EVENT_ID			 = BIT(13),
	PSPOLL_DELIVERY_FAILURE_EVENT_ID	 = BIT(14),
	DISCONNECT_EVENT_COMPLETE_ID		 = BIT(15),
	JOIN_EVENT_COMPLETE_ID			 = BIT(16),
	CHANNEL_SWITCH_COMPLETE_EVENT_ID	 = BIT(17),
	BSS_LOSE_EVENT_ID			 = BIT(18),
	REGAINED_BSS_EVENT_ID			 = BIT(19),
	ROAMING_TRIGGER_MAX_TX_RETRY_EVENT_ID	 = BIT(20),
	SOFT_GEMINI_SENSE_EVENT_ID		 = BIT(22),
	SOFT_GEMINI_PREDICTION_EVENT_ID		 = BIT(23),
	SOFT_GEMINI_AVALANCHE_EVENT_ID		 = BIT(24),
	PLT_RX_CALIBRATION_COMPLETE_EVENT_ID	 = BIT(25),
	DBG_EVENT_ID				 = BIT(26),
	HEALTH_CHECK_REPLY_EVENT_ID		 = BIT(27),
	PERIODIC_SCAN_COMPLETE_EVENT_ID		 = BIT(28),
	PERIODIC_SCAN_REPORT_EVENT_ID		 = BIT(29),
	BA_SESSION_TEAR_DOWN_EVENT_ID		 = BIT(30),
	EVENT_MBOX_ALL_EVENT_ID			 = 0x7fffffff,
};

struct event_debug_report {
	u8 debug_event_id;
	u8 num_params;
	u16 pad;
	u32 report_1;
	u32 report_2;
	u32 report_3;
} __attribute__ ((packed));

#define NUM_OF_RSSI_SNR_TRIGGERS 8

struct event_mailbox {
	u32 events_vector;
	u32 events_mask;
	u32 reserved_1;
	u32 reserved_2;

	u8 dbg_event_id;
	u8 num_relevant_params;
	u16 reserved_3;
	u32 event_report_p1;
	u32 event_report_p2;
	u32 event_report_p3;

	u8 number_of_scan_results;
	u8 scan_tag;
	u8 reserved_4[2];
	u32 compl_scheduled_scan_status;

	u16 scheduled_scan_attended_channels;
	u8 soft_gemini_sense_info;
	u8 soft_gemini_protective_info;
	s8 rssi_snr_trigger_metric[NUM_OF_RSSI_SNR_TRIGGERS];
	u8 channel_switch_status;
	u8 scheduled_scan_status;
	u8 ps_status;

	u8 reserved_5[29];
} __attribute__ ((packed));

int wl1271_event_unmask(struct wl1271 *wl);
void wl1271_event_mbox_config(struct wl1271 *wl);
int wl1271_event_handle(struct wl1271 *wl, u8 mbox);

#endif

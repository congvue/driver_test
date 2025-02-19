/* IEEE 802.11 SoftMAC layer
 * Copyright (c) 2005 Andrea Merello <andreamrl@tiscali.it>
 *
 * Mostly extracted from the rtl8180-sa2400 driver for the 
 * in-kernel generic ieee802.11 stack.
 *
 * Few lines might be stolen from other part of the ieee80211
 * stack. Copyright who own it's copyright
 *
 * WPA code stolen from the ipw2200 driver.
 * Copyright who own it's copyright. 
 *
 * released under the GPL
 */


#include "ieee80211.h"

#include <linux/random.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/list.h>
#include <asm/uaccess.h>

#define RTL8187_MODULE_NAME "ieee80211"
#define DMESG(x,a...) printk(KERN_INFO RTL8187_MODULE_NAME ": " x "\n", ## a)



short ieee80211_is_54g(struct ieee80211_network net)
{
	return ((net.rates_ex_len > 0) || (net.rates_len > 4));
}

short ieee80211_is_shortslot(struct ieee80211_network net)
{
	return (net.capability & WLAN_CAPABILITY_SHORT_SLOT);
}

/* returns the total length needed for pleacing the RATE MFIE
 * tag and the EXTENDED RATE MFIE tag if needed.
 * It encludes two bytes per tag for the tag itself and its len
 */
unsigned int ieee80211_MFIE_rate_len(struct ieee80211_device *ieee)
{
	unsigned int rate_len = 0;
	
	if (ieee->modulation & IEEE80211_CCK_MODULATION)
		rate_len = IEEE80211_CCK_RATE_LEN + 2;
		
	if (ieee->modulation & IEEE80211_OFDM_MODULATION)
		
		rate_len += IEEE80211_OFDM_RATE_LEN + 2;
	
	return rate_len;
}

/* pleace the MFIE rate, tag to the memory (double) poined. 
 * Then it updates the pointer so that
 * it points after the new MFIE tag added.
 */  
void ieee80211_MFIE_Brate(struct ieee80211_device *ieee, u8 **tag_p)
{
	u8 *tag = *tag_p; 
	
	if (ieee->modulation & IEEE80211_CCK_MODULATION){
		*tag++ = MFIE_TYPE_RATES;
		*tag++ = 4;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_CCK_RATE_1MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_CCK_RATE_2MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_CCK_RATE_5MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_CCK_RATE_11MB;
	}
	
	/* We may add an option for custom rates that specific HW might support */
	*tag_p = tag;
}

void ieee80211_MFIE_Grate(struct ieee80211_device *ieee, u8 **tag_p)
{	
	u8 *tag = *tag_p; 
	
		if (ieee->modulation & IEEE80211_OFDM_MODULATION){
		
		*tag++ = MFIE_TYPE_RATES_EX;
		*tag++ = 8;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_6MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_9MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_12MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_18MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_24MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_36MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_48MB;
		*tag++ = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_54MB;
		
	}
	
	/* We may add an option for custom rates that specific HW might support */
	*tag_p = tag;
}

void enqueue_mgmt(struct ieee80211_device *ieee, struct sk_buff *skb)
{
	int nh;
	nh = (ieee->mgmt_queue_head +1) % MGMT_QUEUE_NUM;
			
/*
 * if the queue is full but we have newer frames then
 * just overwrites the oldest.
 *	
 * if (nh == ieee->mgmt_queue_tail)
 *		return -1;
 */	
	if (nh == ieee->mgmt_queue_tail) {
		dev_kfree_skb_any(skb);
	} else {
		ieee->mgmt_queue_head = nh;
		ieee->mgmt_queue_ring[nh] = skb;
	}
	
}

struct sk_buff *dequeue_mgmt(struct ieee80211_device *ieee)
{
	struct sk_buff *ret;
	
	if(ieee->mgmt_queue_tail == ieee->mgmt_queue_head)
		return NULL;
		
	ret = ieee->mgmt_queue_ring[ieee->mgmt_queue_tail];
	
	ieee->mgmt_queue_tail = 
		(ieee->mgmt_queue_tail+1) % MGMT_QUEUE_NUM;
	
	return ret;
}

void init_mgmt_queue(struct ieee80211_device *ieee)
{
	ieee->mgmt_queue_tail = ieee->mgmt_queue_head = 0;
}


void ieee80211_sta_wakeup(struct ieee80211_device *ieee, short nl);

inline void softmac_mgmt_xmit(struct sk_buff *skb, struct ieee80211_device *ieee)
{
	unsigned long flags;
	short single = ieee->softmac_features & IEEE_SOFTMAC_SINGLE_QUEUE;
	struct ieee80211_hdr_3addr  *header=
		(struct ieee80211_hdr_3addr  *) skb->data;
	
	
	spin_lock_irqsave(&ieee->lock, flags);
      
	/* called with 2nd param 0, no mgmt lock required */
	ieee80211_sta_wakeup(ieee,0);
		
	if(single){
		
		if(ieee->queue_stop){
			
			enqueue_mgmt(ieee,skb);
		
		}else{
			header->seq_ctl = cpu_to_le16(ieee->seq_ctrl << 4);

			if (ieee->seq_ctrl == 0xFFF)
				ieee->seq_ctrl = 0;
			else
				ieee->seq_ctrl++;
			
			/* avoid watchdog triggers */
			ieee->dev->trans_start = jiffies;
			ieee->softmac_data_hard_start_xmit(skb,ieee->dev,ieee->basic_rate);
			dev_kfree_skb_any(skb);
		}
		
		spin_unlock_irqrestore(&ieee->lock, flags);
	}else{
		spin_unlock_irqrestore(&ieee->lock, flags);
		spin_lock_irqsave(&ieee->mgmt_tx_lock, flags);
	
		header->seq_ctl = cpu_to_le16(ieee->seq_ctrl << 4);
	
		if (ieee->seq_ctrl == 0xFFF)
			ieee->seq_ctrl = 0;
		else
			ieee->seq_ctrl++;
		
		ieee->softmac_hard_start_xmit(skb,ieee->dev);
		
		spin_unlock_irqrestore(&ieee->mgmt_tx_lock, flags);
	}
}


inline void softmac_ps_mgmt_xmit(struct sk_buff *skb, struct ieee80211_device *ieee)
{
	
	short single = ieee->softmac_features & IEEE_SOFTMAC_SINGLE_QUEUE;
	struct ieee80211_hdr_3addr  *header =
		(struct ieee80211_hdr_3addr  *) skb->data;
	
	
	if(single){
		
		header->seq_ctl = cpu_to_le16(ieee->seq_ctrl << 4);

		if (ieee->seq_ctrl == 0xFFF)
			ieee->seq_ctrl = 0;
		else
			ieee->seq_ctrl++;
			
		/* avoid watchdog triggers */
		ieee->dev->trans_start = jiffies;
		ieee->softmac_data_hard_start_xmit(skb,ieee->dev,ieee->basic_rate);
	
	}else{
		
		header->seq_ctl = cpu_to_le16(ieee->seq_ctrl << 4);
	
		if (ieee->seq_ctrl == 0xFFF)
			ieee->seq_ctrl = 0;
		else
			ieee->seq_ctrl++;

		ieee->softmac_hard_start_xmit(skb,ieee->dev);
		
	}
}

inline struct sk_buff *ieee80211_probe_req(struct ieee80211_device *ieee)
{
	unsigned int len,rate_len;
	u8 *tag;
	struct sk_buff *skb;
	struct ieee80211_probe_request *req;
	
	len = ieee->current_network.ssid_len;
	
	rate_len = ieee80211_MFIE_rate_len(ieee);
	
	skb = dev_alloc_skb(sizeof(struct ieee80211_probe_request) +
			    2 + len + rate_len);
	
	if (!skb) 
		return NULL;
	
	req = (struct ieee80211_probe_request *) skb_put(skb,sizeof(struct ieee80211_probe_request));
	if (ieee->ps == IEEE80211_PS_DISABLED) 
		req->header.frame_ctl = IEEE80211_STYPE_PROBE_REQ;
	else 
		req->header.frame_ctl = cpu_to_le16(IEEE80211_STYPE_PROBE_REQ|IEEE80211_FCTL_PM);
	req->header.duration_id = 0; 
	
	memset(req->header.addr1, 0xff, ETH_ALEN);
	memcpy(req->header.addr2, ieee->dev->dev_addr, ETH_ALEN);
	memset(req->header.addr3, 0xff, ETH_ALEN);
	
	tag = (u8 *) skb_put(skb,len+2+rate_len);
	
	*tag++ = MFIE_TYPE_SSID;
	*tag++ = len;
	memcpy(tag, ieee->current_network.ssid, len);
	tag += len;
	
	ieee80211_MFIE_Brate(ieee,&tag);
	ieee80211_MFIE_Grate(ieee,&tag);
	return skb;
}

struct sk_buff *ieee80211_get_beacon_(struct ieee80211_device *ieee);
void ieee80211_send_beacon(struct ieee80211_device *ieee)
{
	struct sk_buff *skb;
	
	unsigned long flags;	
	
	skb = ieee80211_get_beacon_(ieee);
	if (skb){
		softmac_mgmt_xmit(skb, ieee);
		ieee->softmac_stats.tx_beacons++;
	}
	
	if(ieee->beacon_txing)
		mod_timer(&ieee->beacon_timer,(jiffies + MSECS(ieee->current_network.beacon_interval -5)));
}


void ieee80211_send_beacon_cb(unsigned long _ieee)
{
	struct ieee80211_device *ieee =
		(struct ieee80211_device *) _ieee;
	ieee80211_send_beacon(ieee);
}


void ieee80211_send_probe(struct ieee80211_device *ieee)
{
	struct sk_buff *skb;
	
	skb = ieee80211_probe_req(ieee);
	if (skb){
		softmac_mgmt_xmit(skb, ieee);
		ieee->softmac_stats.tx_probe_rq++;
	}
}

void ieee80211_send_probe_requests(struct ieee80211_device *ieee)
{
	if (ieee->active_scan && (ieee->softmac_features & IEEE_SOFTMAC_PROBERQ)){
		ieee80211_send_probe(ieee);
		ieee80211_send_probe(ieee);
	}
}

/* this performs syncro scan blocking the caller until all channels
 * in the allowed channel map has been checked. 
 */
void ieee80211_softmac_scan_syncro(struct ieee80211_device *ieee)
{
	short ch = 0;
        
	down(&ieee->scan_sem);
	
	while(1)
	{
		
		do{
			ch++;
			if (ch > MAX_CHANNEL_NUMBER) 
				goto out; /* scan completed */
				
		}while(!ieee->channel_map[ch]);
	
		/* this fuction can be called in two situations
		 * 1- We have switched to ad-hoc mode and we are
		 *    performing a complete syncro scan before conclude
		 *    there are no interesting cell and to create a 
		 *    new one. In this case the link state is 
		 *    IEEE80211_NOLINK until we found an interesting cell.
		 *    If so the ieee8021_new_net, called by the RX path
		 *    will set the state to IEEE80211_LINKED, so we stop
		 *    scanning
		 * 2- We are linked and the root uses run iwlist scan.
		 *    So we switch to IEEE80211_LINKED_SCANNING to remember
		 *    that we are still logically linked (not interested in
		 *    new network events, despite for updating the net list,
		 *    but we are temporarly 'unlinked' as the driver shall
		 *    not filter RX frames and the channel is changing.
		 * So the only situation in witch are interested is to check
		 * if the state become LINKED because of the #1 situation
		 */    
		    
		if (ieee->state == IEEE80211_LINKED)
			goto out;
		
		ieee->set_chan(ieee->dev, ch);
		ieee80211_send_probe_requests(ieee);
		
		/* this prevent excessive time wait when we
		 * need to wait for a syncro scan to end..
		 */  		
		if (ieee->sync_scan_hurryup)
			goto out;


		msleep_interruptible_rtl(IEEE80211_SOFTMAC_SCAN_TIME);
		
	}
out:
	ieee->sync_scan_hurryup = 0;
	up(&ieee->scan_sem);
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)	
/* called both by wq with ieee->lock held */
void ieee80211_softmac_scan(struct ieee80211_device *ieee)
{
#if 0
	short watchdog = 0;
	
	do{
		ieee->current_network.channel = 
			(ieee->current_network.channel + 1) % MAX_CHANNEL_NUMBER;
		if (watchdog++ > MAX_CHANNEL_NUMBER) 
				return; /* no good chans */
				
	}while(!ieee->channel_map[ieee->current_network.channel]);
		
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20) 
	queue_delayed_work(ieee->wq,&ieee->softmac_scan_wq,0);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
	queue_work(ieee->wq,&ieee->softmac_scan_wq);	
#else		        
	schedule_task(&ieee->softmac_scan_wq);
#endif

}
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void ieee80211_softmac_scan_wq(struct work_struct *work)
{
	struct delayed_work *dwork = container_of(work, struct delayed_work, work);
	struct ieee80211_device *ieee = container_of(dwork, struct ieee80211_device, softmac_scan_wq);
#else
void ieee80211_softmac_scan_wq(struct ieee80211_device *ieee)
{	
#endif
	down(&ieee->scan_sem);

	if (ieee->scanning == 0 )
		goto out;
	
	do{
		ieee->current_network.channel = 
			(ieee->current_network.channel + 1) % MAX_CHANNEL_NUMBER;
		if (ieee->scan_watchdog++ > MAX_CHANNEL_NUMBER) 
				goto out; /* no good chans */
				
	}while(!ieee->channel_map[ieee->current_network.channel]);
	
	
	
	ieee->set_chan(ieee->dev, ieee->current_network.channel);
	ieee80211_send_probe_requests(ieee);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
	queue_delayed_work(ieee->wq, &ieee->softmac_scan_wq, IEEE80211_SOFTMAC_SCAN_TIME);	
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
	queue_work(ieee->wq,&ieee->softmac_scan_wq);	
#else		
	if (ieee->scanning == 1) 
		mod_timer(&ieee->scan_timer,(jiffies + MSECS(IEEE80211_SOFTMAC_SCAN_TIME)));
#endif

out:
	ieee->actscanning = false;
	ieee->scan_watchdog = 0;
	up(&ieee->scan_sem);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)	
void ieee80211_softmac_scan_cb(unsigned long _dev)
{
	unsigned long flags;
	struct ieee80211_device *ieee = (struct ieee80211_device *)_dev;
	
	spin_lock_irqsave(&ieee->lock, flags);
	ieee80211_softmac_scan(ieee);
	spin_unlock_irqrestore(&ieee->lock, flags);
}
#endif


void ieee80211_beacons_start(struct ieee80211_device *ieee)
{
	unsigned long flags;	

	spin_lock_irqsave(&ieee->beacon_lock,flags);

	ieee->beacon_txing = 1;
	ieee80211_send_beacon(ieee);
	
	spin_unlock_irqrestore(&ieee->beacon_lock,flags);
}

void ieee80211_beacons_stop(struct ieee80211_device *ieee)
{
	unsigned long flags;

	spin_lock_irqsave(&ieee->beacon_lock,flags);

	ieee->beacon_txing = 0;
 	del_timer_sync(&ieee->beacon_timer);

	spin_unlock_irqrestore(&ieee->beacon_lock,flags);

}


void ieee80211_stop_send_beacons(struct ieee80211_device *ieee)
{
	if(ieee->stop_send_beacons)
		ieee->stop_send_beacons(ieee->dev);
	if (ieee->softmac_features & IEEE_SOFTMAC_BEACONS)
		ieee80211_beacons_stop(ieee);
}


void ieee80211_start_send_beacons(struct ieee80211_device *ieee)
{
	if(ieee->start_send_beacons)
		ieee->start_send_beacons(ieee->dev);
	if(ieee->softmac_features & IEEE_SOFTMAC_BEACONS)
		ieee80211_beacons_start(ieee); 
}


void ieee80211_softmac_stop_scan(struct ieee80211_device *ieee)
{
	
	
	down(&ieee->scan_sem);
	
	if (ieee->scanning == 1){
		ieee->scanning = 0;
		ieee->scan_watchdog = 0;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
		cancel_delayed_work(&ieee->softmac_scan_wq);
#else
		del_timer_sync(&ieee->scan_timer);
#endif
	}
	
	up(&ieee->scan_sem);
}

void ieee80211_stop_scan(struct ieee80211_device *ieee)
{
	if (ieee->softmac_features & IEEE_SOFTMAC_SCAN)
		ieee80211_softmac_stop_scan(ieee);
	else
		ieee->stop_scan(ieee->dev);
}

/* called with ieee->lock held */
void ieee80211_start_scan(struct ieee80211_device *ieee)
{

	if (ieee->softmac_features & IEEE_SOFTMAC_SCAN){	
		if (ieee->scanning == 0){
			ieee->scanning = 1;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
			queue_delayed_work(ieee->wq, &ieee->softmac_scan_wq, 0);
#elif  LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	

			queue_work(ieee->wq, &ieee->softmac_scan_wq);
#else
			ieee80211_softmac_scan(ieee);
#endif
		}
	}else
		ieee->start_scan(ieee->dev);
	
}

/* called with wx_sem held */
void ieee80211_start_scan_syncro(struct ieee80211_device *ieee)
{
	ieee->sync_scan_hurryup = 0;
	
	if (ieee->softmac_features & IEEE_SOFTMAC_SCAN)
		ieee80211_softmac_scan_syncro(ieee);
	else
		ieee->scan_syncro(ieee->dev);
		
}

inline struct sk_buff *ieee80211_authentication_req(struct ieee80211_network *beacon, 
	struct ieee80211_device *ieee, int challengelen)
{
	struct sk_buff *skb;	
	struct ieee80211_authentication *auth;
	
	skb = dev_alloc_skb(sizeof(struct ieee80211_authentication) + challengelen); 
	
	if (!skb) return NULL;
	
	auth = (struct ieee80211_authentication *)
		skb_put(skb, sizeof(struct ieee80211_authentication));
	
	auth->header.frame_ctl = IEEE80211_STYPE_AUTH;
	if (challengelen) auth->header.frame_ctl |= IEEE80211_FCTL_WEP;
	if (ieee->ps != IEEE80211_PS_DISABLED) auth->header.frame_ctl |= IEEE80211_FCTL_PM;

	
	auth->header.duration_id = 0x013a; 
	
	memcpy(auth->header.addr1, beacon->bssid, ETH_ALEN);
	memcpy(auth->header.addr2, ieee->dev->dev_addr, ETH_ALEN);
	memcpy(auth->header.addr3, beacon->bssid, ETH_ALEN);
	
	auth->algorithm = ieee->open_wep ? WLAN_AUTH_OPEN : WLAN_AUTH_SHARED_KEY;
	
	auth->transaction = cpu_to_le16(ieee->associate_seq);
	ieee->associate_seq++;
	
	auth->status = cpu_to_le16(WLAN_STATUS_SUCCESS);
	
	return skb;
	
}

static struct sk_buff* ieee80211_probe_resp(struct ieee80211_device *ieee, u8 *dest)
{
	u8 *tag;
	int beacon_size;
	struct ieee80211_probe_response *beacon_buf;
	struct sk_buff *skb;
	int encrypt;
	int atim_len,erp_len;
	struct ieee80211_crypt_data* crypt;
	
	char *ssid = ieee->current_network.ssid;
	int ssid_len = ieee->current_network.ssid_len;
	int rate_len = ieee->current_network.rates_len+2;
	int rate_ex_len = ieee->current_network.rates_ex_len;
	int wpa_ie_len = ieee->wpa_ie_len;

	if(rate_ex_len > 0) rate_ex_len+=2;
	
	if(ieee->current_network.capability & WLAN_CAPABILITY_IBSS)
		atim_len = 4;
	else
		atim_len = 0;
	
	if(ieee80211_is_54g(ieee->current_network)) 
		erp_len = 3;
	else
		erp_len = 0;
	
	crypt = ieee->crypt[ieee->tx_keyidx];


	encrypt = ieee->host_encrypt && crypt && crypt->ops &&
		((0 == strcmp(crypt->ops->name, "WEP") || wpa_ie_len));

	beacon_size = sizeof(struct ieee80211_probe_response)+
		ssid_len
		+3 
		+rate_len
		+rate_ex_len
		+atim_len
		+erp_len
		+wpa_ie_len;
	
	skb = dev_alloc_skb(beacon_size);
	
	if (!skb) 
		return NULL;
	
	beacon_buf = (struct ieee80211_probe_response*) skb_put(skb, beacon_size);
	
	memcpy (beacon_buf->header.addr1, dest,ETH_ALEN);
	memcpy (beacon_buf->header.addr2, ieee->dev->dev_addr, ETH_ALEN);
	memcpy (beacon_buf->header.addr3, ieee->current_network.bssid, ETH_ALEN);

	beacon_buf->header.duration_id = 0; 
	beacon_buf->beacon_interval = 
		cpu_to_le16(ieee->current_network.beacon_interval);
	beacon_buf->capability = 
		cpu_to_le16(ieee->current_network.capability & WLAN_CAPABILITY_IBSS);
	
	if(ieee->short_slot && (ieee->current_network.capability & WLAN_CAPABILITY_SHORT_SLOT))
		cpu_to_le16((beacon_buf->capability |= WLAN_CAPABILITY_SHORT_SLOT));		
	
	crypt = ieee->crypt[ieee->tx_keyidx];


	if (encrypt)	
		beacon_buf->capability |= cpu_to_le16(WLAN_CAPABILITY_PRIVACY);
	
		
	beacon_buf->header.frame_ctl = cpu_to_le16(IEEE80211_STYPE_PROBE_RESP);
	
	beacon_buf->info_element.id = MFIE_TYPE_SSID;	
	beacon_buf->info_element.len = ssid_len;
	
	tag = (u8*) beacon_buf->info_element.data;
	
	memcpy(tag, ssid, ssid_len);
	
	tag += ssid_len;
	
	*(tag++) = MFIE_TYPE_RATES;
	*(tag++) = rate_len-2; 
	memcpy(tag,ieee->current_network.rates,rate_len-2);
	tag+=rate_len-2;
	
	*(tag++) = MFIE_TYPE_DS_SET;
	*(tag++) = 1;
	*(tag++) = ieee->current_network.channel;
	
	if(atim_len){
		*(tag++) = MFIE_TYPE_IBSS_SET;
		*(tag++) = 2;
		*((u16*)(tag)) = cpu_to_le16(ieee->current_network.atim_window);
		tag+=2;
	}
	
	if(erp_len){
		*(tag++) = MFIE_TYPE_ERP;
		*(tag++) = 1;
		*(tag++) = 0; 
	}
	
	if(rate_ex_len){
		*(tag++) = MFIE_TYPE_RATES_EX;
		*(tag++) = rate_ex_len-2; 
		memcpy(tag,ieee->current_network.rates_ex,rate_ex_len-2);
		tag+=rate_ex_len-2;
	}

	if (wpa_ie_len){
		if (ieee->iw_mode == IW_MODE_ADHOC)
		{
			memcpy(&ieee->wpa_ie[14], &ieee->wpa_ie[8], 4);
		}
		memcpy(tag, ieee->wpa_ie, ieee->wpa_ie_len);
		tag += wpa_ie_len;
	}

	skb->dev = ieee->dev;
	return skb;
}


struct sk_buff* ieee80211_assoc_resp(struct ieee80211_device *ieee, u8 *dest)
{
	struct sk_buff *skb;
	u8* tag;
	
	struct ieee80211_crypt_data* crypt;
	struct ieee80211_assoc_response_frame *assoc;
	short encrypt;
	
	unsigned int rate_len = ieee80211_MFIE_rate_len(ieee);
	int len = sizeof(struct ieee80211_assoc_response_frame) + rate_len;
	
	skb = dev_alloc_skb(len); 
	
	if (!skb) 
		return NULL;
	
	assoc = (struct ieee80211_assoc_response_frame *)
		skb_put(skb,sizeof(struct ieee80211_assoc_response_frame));
	
	assoc->header.frame_ctl = cpu_to_le16(IEEE80211_STYPE_ASSOC_RESP);
	memcpy(assoc->header.addr1, dest,ETH_ALEN);
	memcpy(assoc->header.addr3, ieee->dev->dev_addr, ETH_ALEN);
	memcpy(assoc->header.addr2, ieee->dev->dev_addr, ETH_ALEN);
	assoc->capability = cpu_to_le16(ieee->iw_mode == IW_MODE_MASTER ? 
		WLAN_CAPABILITY_BSS : WLAN_CAPABILITY_IBSS);
	
		
	if(ieee->short_slot)
		assoc->capability |= cpu_to_le16(WLAN_CAPABILITY_SHORT_SLOT);
		
	if (ieee->host_encrypt)
		crypt = ieee->crypt[ieee->tx_keyidx];
	else crypt = NULL;
	
	encrypt = ( crypt && crypt->ops);
	   
	if (encrypt)
		assoc->capability |= cpu_to_le16(WLAN_CAPABILITY_PRIVACY);
	
	assoc->status = 0;
	assoc->aid = cpu_to_le16(ieee->assoc_id);
	if (ieee->assoc_id == 0x2007) ieee->assoc_id=0;
	else ieee->assoc_id++;
	
	tag = (u8*) skb_put(skb, rate_len);
	
	ieee80211_MFIE_Brate(ieee, &tag);
	ieee80211_MFIE_Grate(ieee, &tag);
	
	return skb;
}

struct sk_buff* ieee80211_auth_resp(struct ieee80211_device *ieee,int status, u8 *dest)
{
	struct sk_buff *skb;
	struct ieee80211_authentication *auth;
	
	skb = dev_alloc_skb(sizeof(struct ieee80211_authentication)+1); 
	
	if (!skb) 
		return NULL;
	
	skb->len = sizeof(struct ieee80211_authentication);
	
	auth = (struct ieee80211_authentication *)skb->data;
	
	auth->status = cpu_to_le16(status);
	auth->transaction = cpu_to_le16(2);
	auth->algorithm = cpu_to_le16(WLAN_AUTH_OPEN);
	
	memcpy(auth->header.addr3, ieee->dev->dev_addr, ETH_ALEN);
	memcpy(auth->header.addr2, ieee->dev->dev_addr, ETH_ALEN);
	memcpy(auth->header.addr1, dest, ETH_ALEN);
	auth->header.frame_ctl = cpu_to_le16(IEEE80211_STYPE_AUTH); 
	return skb;
	
	
}

struct sk_buff* ieee80211_null_func(struct ieee80211_device *ieee,short pwr)
{
	struct sk_buff *skb;
	struct ieee80211_hdr_3addr* hdr;
	
	skb = dev_alloc_skb(sizeof(struct ieee80211_hdr_3addr)); 
	
	if (!skb) 
		return NULL;
	
	hdr = (struct ieee80211_hdr_3addr*)skb_put(skb,sizeof(struct ieee80211_hdr_3addr));
	
	memcpy(hdr->addr1, ieee->current_network.bssid, ETH_ALEN);
	memcpy(hdr->addr2, ieee->dev->dev_addr, ETH_ALEN);
	memcpy(hdr->addr3, ieee->current_network.bssid, ETH_ALEN);
	
	hdr->frame_ctl = cpu_to_le16(IEEE80211_FTYPE_DATA | 
		IEEE80211_STYPE_NULLFUNC | IEEE80211_FCTL_TODS | 
		(pwr ? IEEE80211_FCTL_PM:0)); 
	
	return skb;
	
	
}


void ieee80211_resp_to_assoc_rq(struct ieee80211_device *ieee, u8* dest)
{
	struct sk_buff *buf = ieee80211_assoc_resp(ieee, dest);
	
	if (buf)
		softmac_mgmt_xmit(buf, ieee);
}


void ieee80211_resp_to_auth(struct ieee80211_device *ieee, int s, u8* dest)
{
	struct sk_buff *buf = ieee80211_auth_resp(ieee, s, dest);
	
	if (buf)
		softmac_mgmt_xmit(buf, ieee);
}


void ieee80211_resp_to_probe(struct ieee80211_device *ieee, u8 *dest)
{
	
	struct sk_buff *buf = ieee80211_probe_resp(ieee, dest);
	
	if (buf) 
		softmac_mgmt_xmit(buf, ieee);
}

#if 0
inline struct sk_buff *ieee80211_association_req(struct ieee80211_network *beacon,struct ieee80211_device *ieee)
{
	struct sk_buff *skb;
	
	struct ieee80211_assoc_request_frame *hdr;
	u8 *tag;
	
	unsigned int wpa_len = beacon->wpa_ie_len;
	unsigned int rsn_len = beacon->rsn_ie_len;
	
	unsigned int rate_len = ieee80211_MFIE_rate_len(ieee);
	
	
	
	int len=sizeof(struct ieee80211_assoc_request_frame)+
				+ beacon->ssid_len
				+ rate_len
				+ rsn_len
				+ wpa_len;
				
	skb = dev_alloc_skb(len);
	
	if (!skb) 
		return NULL;
	
	hdr = (struct ieee80211_assoc_request_frame *)
		skb_put(skb, sizeof(struct ieee80211_assoc_request_frame));
	
	
	hdr->header.frame_ctl = IEEE80211_STYPE_ASSOC_REQ;
	if (ieee->ps != IEEE80211_PS_DISABLED) hdr->header.frame_ctl |= IEEE80211_FCTL_PM; 
	hdr->header.duration_id= 37; 
	memcpy(hdr->header.addr1, beacon->bssid, ETH_ALEN);
	memcpy(hdr->header.addr2, ieee->dev->dev_addr, ETH_ALEN);
	memcpy(hdr->header.addr3, beacon->bssid, ETH_ALEN);
	
	hdr->capability = cpu_to_le16(WLAN_CAPABILITY_BSS);
	if (beacon->capability & WLAN_CAPABILITY_PRIVACY ) 
		hdr->capability |= cpu_to_le16(WLAN_CAPABILITY_PRIVACY);
	if(beacon->capability & WLAN_CAPABILITY_SHORT_PREAMBLE)
		hdr->capability |= cpu_to_le16(WLAN_CAPABILITY_SHORT_PREAMBLE); 
	
	if(ieee->short_slot)
		hdr->capability |= cpu_to_le16(WLAN_CAPABILITY_SHORT_SLOT);
	
	hdr->listen_interval = 0xa; 
	
	hdr->info_element.id = MFIE_TYPE_SSID;

	hdr->info_element.len = beacon->ssid_len;
	tag = skb_put(skb, beacon->ssid_len);
	memcpy(tag, beacon->ssid, beacon->ssid_len);
	
	tag = skb_put(skb, rate_len); 
	
	ieee80211_MFIE_Brate(ieee, &tag);
	ieee80211_MFIE_Grate(ieee, &tag);
	
	tag = skb_put(skb,wpa_len);
	
	if(wpa_len) {
		if(wpa_len > (22 + 2) )
		{
			beacon->wpa_ie[wpa_len - 2] = 0;
		}
	
	}
	memcpy(tag,beacon->wpa_ie,wpa_len);
	
	tag = skb_put(skb,rsn_len);

	if(rsn_len) {
		beacon->rsn_ie[rsn_len - 2] = 0;
	}
	memcpy(tag,beacon->rsn_ie,rsn_len);
	return skb;
}
#else

inline struct sk_buff *ieee80211_association_req(struct ieee80211_network *beacon,struct ieee80211_device *ieee)
{
	struct sk_buff *skb;
	
	struct ieee80211_assoc_request_frame *hdr;
	u8 *tag;
	unsigned int wpa_len = beacon->wpa_ie_len;
#if 0	
	unsigned long flags;
	short info_addr = 0;
	int i;
	u16 suite_count = 0;
	u8 suit_select = 0;
	struct net_device *dev = ieee->dev;
	union iwreq_data wrqu;
	u8 *buff;
	u8 *p;
	u8  pairwise_type = (ieee->wpax_type_notify >> 8) & 0xff;
	u8  authen_type = (ieee->wpax_type_notify >> 16) & 0xff;
#endif
#if 1	
	unsigned int rsn_len = beacon->rsn_ie_len;
#else
	unsigned int rsn_len = beacon->rsn_ie_len - 4;
#endif	
	unsigned int rate_len = ieee80211_MFIE_rate_len(ieee);
	unsigned int wmm_info_len = beacon->QoS_Enable?9:0;
#ifdef THOMAS_TURBO	
	unsigned int turbo_info_len = beacon->Turbo_Enable?9:0;
#endif

	u8  encry_proto = ieee->wpax_type_notify & 0xff;
	
	int len = 0; 
	
	if(ieee->wpax_type_set) {
		if (IEEE_PROTO_WPA == encry_proto) {
			rsn_len = 0;
		} else if (IEEE_PROTO_RSN == encry_proto) {
			wpa_len = 0;
		}
	}
#ifdef THOMAS_TURBO		
	len = sizeof(struct ieee80211_assoc_request_frame)+
		+ beacon->ssid_len
		+ rate_len
		+ wpa_len
		+ rsn_len
		+ wmm_info_len
		+ turbo_info_len;
#else
	len = sizeof(struct ieee80211_assoc_request_frame)+
		+ beacon->ssid_len
		+ rate_len
		+ wpa_len
		+ rsn_len
		+ wmm_info_len;
#endif

#ifdef _RTL8187_EXT_PATCH_
	if(ieee->iw_mode == ieee->iw_ext_mode)
		skb = dev_alloc_skb(len+256); 
	else
#endif
	skb = dev_alloc_skb(len);
	
	if (!skb) 
		return NULL;
	
	hdr = (struct ieee80211_assoc_request_frame *)
		skb_put(skb, sizeof(struct ieee80211_assoc_request_frame));
	
	hdr->header.frame_ctl = IEEE80211_STYPE_ASSOC_REQ;
	hdr->header.duration_id= 37; 
	memcpy(hdr->header.addr1, beacon->bssid, ETH_ALEN);
	memcpy(hdr->header.addr2, ieee->dev->dev_addr, ETH_ALEN);
	memcpy(hdr->header.addr3, beacon->bssid, ETH_ALEN);
	memcpy(ieee->ap_mac_addr, beacon->bssid, ETH_ALEN);
	
	hdr->capability = cpu_to_le16(WLAN_CAPABILITY_BSS);
	if (beacon->capability & WLAN_CAPABILITY_PRIVACY ) 
		hdr->capability |= cpu_to_le16(WLAN_CAPABILITY_PRIVACY);
	
	if(ieee->short_slot)
		hdr->capability |= cpu_to_le16(WLAN_CAPABILITY_SHORT_SLOT);
	
#ifdef _RTL8187_EXT_PATCH_
	if((ieee->iw_mode == ieee->iw_ext_mode) && ieee->ext_patch_ieee80211_association_req_1)
		ieee->ext_patch_ieee80211_association_req_1(hdr);
#endif

	hdr->listen_interval = 0xa; 
	
	hdr->info_element.id = MFIE_TYPE_SSID;

	hdr->info_element.len = beacon->ssid_len;
	tag = skb_put(skb, beacon->ssid_len);
	memcpy(tag, beacon->ssid, beacon->ssid_len);
	
	tag = skb_put(skb, rate_len); 
	
	ieee80211_MFIE_Brate(ieee, &tag);
	ieee80211_MFIE_Grate(ieee, &tag);
	
#if 0
	if(rsn_len == 0){

		tag = skb_put(skb,wpa_len);

		if(wpa_len) {
		

		  if(wpa_len > 24) {
			beacon->wpa_ie[wpa_len - 2] = 0;
		  }
                  if(  beacon->wpa_ie[11]==0x2      ){ 
                  ieee->broadcast_key_type = KEY_TYPE_TKIP;
                }
                  else if(  beacon->wpa_ie[11]==0x4      ){
                  ieee->broadcast_key_type = KEY_TYPE_CCMP;
                }
		  if(	beacon->wpa_ie[14]==0		 
			&& beacon->wpa_ie[15]==0x50
                        && beacon->wpa_ie[16]==0xf2
                        && beacon->wpa_ie[17]==0x2  	){ 
                  ieee->pairwise_key_type = KEY_TYPE_TKIP;
		}

                  else if(   beacon->wpa_ie[14]==0
                        && beacon->wpa_ie[15]==0x50
                        && beacon->wpa_ie[16]==0xf2
                        && beacon->wpa_ie[17]==0x4      ){
                  ieee->pairwise_key_type = KEY_TYPE_CCMP;
		}
		buff = kmalloc(IW_CUSTOM_MAX, GFP_ATOMIC);
		memset(buff, 0, IW_CUSTOM_MAX);
		p=buff;
		p += sprintf(p, "ASSOCINFO(ReqIEs=");
		for(i=0;i<wpa_len;i++){
			p += sprintf(p, "%02x", beacon->wpa_ie[i]);
		}
		p += sprintf(p, ")");
		memset(&wrqu, 0, sizeof(wrqu) );
		wrqu.data.length = p - buff;

		wireless_send_event(dev, IWEVCUSTOM, &wrqu, buff);
		  memcpy(tag,beacon->wpa_ie,wpa_len);
		}

	}	

	if(rsn_len > 22) {

	  					if(     beacon->rsn_ie[4]==0x0 &&
                                beacon->rsn_ie[5]==0xf &&
                                beacon->rsn_ie[6]==0xac){

                                switch(beacon->rsn_ie[7]){
                                        case 0x1:
                                                ieee->broadcast_key_type = KEY_TYPE_WEP40;
                                                break;
                                        case 0x2:
                                                ieee->broadcast_key_type = KEY_TYPE_TKIP;
                                                break;
                                        case 0x4:
                                                ieee->broadcast_key_type = KEY_TYPE_CCMP;
                                                break;
                                        case 0x5:
                                                ieee->broadcast_key_type = KEY_TYPE_WEP104;
                                                break;
                                        default:
                                                printk("fault suite type in RSN broadcast key\n");
                                                break;
                                }
                        }

                        if(     beacon->rsn_ie[10]==0x0 &&
                                beacon->rsn_ie[11]==0xf &&
                                beacon->rsn_ie[12]==0xac){
				if(beacon->rsn_ie[8]==1){
	                                switch(beacon->rsn_ie[13]){
        	                                case 0x2:
                	                                ieee->pairwise_key_type = KEY_TYPE_TKIP;
                        	                        break;
                                	        case 0x4:
                                        	        ieee->pairwise_key_type = KEY_TYPE_CCMP;
                                                	break;
        	                                default:	
	                                                printk("fault suite type in RSN pairwise key\n");
                	                                break;
                                	}
				}
				else if(beacon->rsn_ie[8]==2){
					ieee->pairwise_key_type = KEY_TYPE_CCMP;
				}
                        }


		
		tag = skb_put(skb,22);
		memcpy(tag,(beacon->rsn_ie + info_addr),8);
		tag[1] =  20;
		tag += 8;
		info_addr += 8;

		spin_lock_irqsave(&ieee->wpax_suitlist_lock,flags);
		for (i = 0; i < 2; i++) {
			tag[0] = 1;
			tag[1] = 0;
			tag += 2;
			suite_count = beacon->rsn_ie[info_addr] + \
				      (beacon->rsn_ie[info_addr + 1] << 8);
			info_addr += 2;
			if(1 == suite_count) {
				memcpy(tag,(beacon->rsn_ie + info_addr),4);
				info_addr += 4;
			} else {
				if(ieee->wpax_type_set) {
					suit_select = ((0 == i) ? pairwise_type:authen_type)&0x0f ;
					memcpy(tag,rsn_authen_cipher_suite[suit_select],4);
				} else {
					if(i == 0) {
						memcpy(tag,rsn_authen_cipher_suite[4],4);
					} else {
						memcpy(tag,rsn_authen_cipher_suite[2],4);
					}

				}

				info_addr += (suite_count * 4);
			}
			tag += 4;
		}
		spin_unlock_irqrestore(&ieee->wpax_suitlist_lock,flags);

		tag[0] = 0;
		tag[1] = beacon->rsn_ie[info_addr+1];



	} else {
		tag = skb_put(skb,rsn_len);
		if(rsn_len) {


			if( 	beacon->rsn_ie[4]==0x0 &&
				beacon->rsn_ie[5]==0xf &&
				beacon->rsn_ie[6]==0xac){
				switch(beacon->rsn_ie[7]){
					case 0x1:
						ieee->broadcast_key_type = KEY_TYPE_WEP40;
                                                break;
					case 0x2:
						ieee->broadcast_key_type = KEY_TYPE_TKIP;
						break;
					case 0x4:
   	                                        ieee->broadcast_key_type = KEY_TYPE_CCMP;
                                                break;
                                        case 0x5:
                                                ieee->broadcast_key_type = KEY_TYPE_WEP104;
                                                break;
					default: 
						printk("fault suite type in RSN broadcast key\n");
						break;
				}
			}
                        if(     beacon->rsn_ie[10]==0x0 &&
                                beacon->rsn_ie[11]==0xf &&
                                beacon->rsn_ie[12]==0xac){
                                if(beacon->rsn_ie[8]==1){
                                        switch(beacon->rsn_ie[13]){
                                                case 0x2:
                                                        ieee->pairwise_key_type = KEY_TYPE_TKIP;
                                                        break;
                                                case 0x4:
                                                        ieee->pairwise_key_type = KEY_TYPE_CCMP;
                                                        break;
                                                default:
                                                        printk("fault suite type in RSN pairwise key\n");
                                                        break;
                                	}

				}
                                else if(beacon->rsn_ie[8]==2){
                                        ieee->pairwise_key_type = KEY_TYPE_CCMP;
                                }
                        }

   
			beacon->rsn_ie[rsn_len - 2] = 0;
			memcpy(tag,beacon->rsn_ie,rsn_len);
		}
	}
#else
        if (ieee->wpa_ie){
	tag = skb_put(skb,ieee->wpa_ie_len);
	memcpy(tag,ieee->wpa_ie,ieee->wpa_ie_len);
        }
#endif
#ifdef WMM
	tag = skb_put(skb,wmm_info_len);
	if(wmm_info_len) {
	  ieee80211_WMM_Info(ieee, &tag);
	}
#endif
#ifdef THOMAS_TURBO
	tag = skb_put(skb,turbo_info_len);
        if(turbo_info_len) {
                ieee80211_TURBO_Info(ieee, &tag);
        }
#endif

#ifdef _RTL8187_EXT_PATCH_
	if((ieee->iw_mode == ieee->iw_ext_mode) && ieee->ext_patch_ieee80211_association_req_2)
		ieee->ext_patch_ieee80211_association_req_2(ieee, beacon, skb);
#endif

	return skb;
}
#endif

void ieee80211_associate_abort(struct ieee80211_device *ieee)
{
	
	unsigned long flags;
	spin_lock_irqsave(&ieee->lock, flags);
	
	ieee->associate_seq++;
	
	/* don't scan, and avoid to have the RX path possibily
	 * try again to associate. Even do not react to AUTH or
	 * ASSOC response. Just wait for the retry wq to be scheduled.
	 * Here we will check if there are good nets to associate
	 * with, so we retry or just get back to NO_LINK and scanning
	 */
	if (ieee->state == IEEE80211_ASSOCIATING_AUTHENTICATING){
		IEEE80211_DEBUG_MGMT("Authentication failed\n"); 
		ieee->softmac_stats.no_auth_rs++;
	}else{
		IEEE80211_DEBUG_MGMT("Association failed\n"); 
		ieee->softmac_stats.no_ass_rs++;
	}
		
	ieee->state = IEEE80211_ASSOCIATING_RETRY;

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
	queue_delayed_work(ieee->wq, &ieee->associate_retry_wq, IEEE80211_SOFTMAC_ASSOC_RETRY_TIME);
#else	
	schedule_task(&ieee->associate_retry_wq);
#endif	
		
	
	
	spin_unlock_irqrestore(&ieee->lock, flags);
}

void ieee80211_associate_abort_cb(unsigned long dev)
{
	ieee80211_associate_abort((struct ieee80211_device *) dev);
}


void ieee80211_associate_step1(struct ieee80211_device *ieee)
{
	struct ieee80211_network *beacon = &ieee->current_network;
	struct sk_buff *skb;
	

	
	ieee->softmac_stats.tx_auth_rq++;
	skb=ieee80211_authentication_req(beacon, ieee, 0);
	
	if (!skb) 
		ieee80211_associate_abort(ieee);
	else{ 
		ieee->state = IEEE80211_ASSOCIATING_AUTHENTICATING ;

		softmac_mgmt_xmit(skb, ieee);
		mod_timer(&ieee->associate_timer, jiffies + HZ/2);
	}	
}

void ieee80211_auth_challenge(struct ieee80211_device *ieee, u8 *challenge, int chlen)
{
	u8 *c;	
	struct sk_buff *skb;
	struct ieee80211_network *beacon = &ieee->current_network;
	
	ieee->associate_seq++;
	ieee->softmac_stats.tx_auth_rq++;
	
	skb = ieee80211_authentication_req(beacon, ieee, chlen+2);
	if (!skb) 
		ieee80211_associate_abort(ieee);
	else{
		c = skb_put(skb, chlen+2);
		*(c++) = MFIE_TYPE_CHALLENGE;
		*(c++) = chlen;
		memcpy(c, challenge, chlen);
		
		IEEE80211_DEBUG_MGMT("Sending authentication challenge response\n");
		
		ieee80211_encrypt_fragment(ieee, skb, sizeof(struct ieee80211_hdr_3addr  ));
			
		softmac_mgmt_xmit(skb, ieee);
		mod_timer(&ieee->associate_timer, jiffies + HZ/2);
	}	
	kfree(challenge);
}

void ieee80211_associate_step2(struct ieee80211_device *ieee)
{
	struct sk_buff* skb;
	struct ieee80211_network *beacon = &ieee->current_network;
	
	del_timer_sync(&ieee->associate_timer);
	

	
	ieee->softmac_stats.tx_ass_rq++;
	skb=ieee80211_association_req(beacon, ieee);
	if (!skb) 
		ieee80211_associate_abort(ieee);
	else{
		softmac_mgmt_xmit(skb, ieee);
		mod_timer(&ieee->associate_timer, jiffies + HZ/2);
	}	
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void ieee80211_associate_complete_wq(struct work_struct *work)
{
	struct ieee80211_device *ieee = container_of(work, struct ieee80211_device, associate_complete_wq);
#else
void ieee80211_associate_complete_wq(struct ieee80211_device *ieee)
{
#endif
	printk(KERN_INFO "Associated successfully\n");
	if(ieee80211_is_54g(ieee->current_network) && 
		(ieee->modulation & IEEE80211_OFDM_MODULATION)){
		
		ieee->rate = 540;
		printk(KERN_INFO"Using G rates\n");
	}else{
		ieee->rate = 110;
		printk(KERN_INFO"Using B rates\n");
	}

#ifdef LED_SHIN
	{
		struct net_device *dev = ieee->dev;
		ieee->ieee80211_led_contorl(dev, LED_CTL_LINK);
	}
#endif

	ieee->link_change(ieee->dev);	
	notify_wx_assoc_event(ieee);	

	if (ieee->data_hard_resume)
		ieee->data_hard_resume(ieee->dev);

	netif_carrier_on(ieee->dev);

	udelay(500);

}

void ieee80211_associate_complete(struct ieee80211_device *ieee)
{

	del_timer_sync(&ieee->associate_timer);
	
	ieee->seq_ctrl = 0;
	ieee->state = IEEE80211_LINKED;
	IEEE80211_DEBUG_MGMT("Successfully associated\n");
	
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
	queue_work(ieee->wq, &ieee->associate_complete_wq);
#else
	schedule_task(&ieee->associate_complete_wq);
#endif
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void ieee80211_associate_procedure_wq(struct work_struct *work)
{
	struct ieee80211_device *ieee = container_of(work, struct ieee80211_device, associate_procedure_wq);
#else
void ieee80211_associate_procedure_wq(struct ieee80211_device *ieee)
{
#endif
	ieee->sync_scan_hurryup = 1;
	down(&ieee->wx_sem);
	
	if (ieee->data_hard_stop)
		ieee->data_hard_stop(ieee->dev);
	
	ieee80211_stop_scan(ieee);
	ieee->set_chan(ieee->dev, ieee->current_network.channel);
	
	ieee->associate_seq = 1;
	ieee80211_associate_step1(ieee);
	
	up(&ieee->wx_sem);
}

inline void ieee80211_softmac_new_net(struct ieee80211_device *ieee, struct ieee80211_network *net)
{
	
	u8 tmp_ssid[IW_ESSID_MAX_SIZE+1];
	int tmp_ssid_len = 0;
	
	short apset,ssidset,ssidbroad,apmatch,ssidmatch;
	
	/* we are interested in new new only if we are not associated 
	 * and we are not associating / authenticating
	 */
	if (ieee->state != IEEE80211_NOLINK)
		return; 

	if ((ieee->iw_mode == IW_MODE_INFRA) && !(net->capability & WLAN_CAPABILITY_BSS))
		return;
	
	if ((ieee->iw_mode == IW_MODE_ADHOC) && !(net->capability & WLAN_CAPABILITY_IBSS))
		return;

	
	if (ieee->iw_mode == IW_MODE_INFRA || ieee->iw_mode == IW_MODE_ADHOC){
		/* if the user specified the AP MAC, we need also the essid
		 * This could be obtained by beacons or, if the network does not
		 * broadcast it, it can be put manually.
		 */
		apset = ieee->wap_set;
		ssidset = ieee->ssid_set;
		ssidbroad =  !(net->ssid_len == 0 || net->ssid[0]== '\0');
		apmatch = (memcmp(ieee->current_network.bssid, net->bssid, ETH_ALEN)==0);
		ssidmatch = (0==strncmp(ieee->current_network.ssid, net->ssid, net->ssid_len));
		

		

			
		if (	/* if the user set the AP check if match.
		         * if the network does not broadcast essid we check the user supplyed ANY essid
			 * if the network does broadcast and the user does not set essid it is OK
			 * if the network does broadcast and the user did set essid chech if essid match
			 */
			( apset && apmatch && 
				((ssidset && ssidbroad && ssidmatch) || (ssidbroad && !ssidset) || (!ssidbroad && ssidset)) ) ||  
			/* if the ap is not set, check that the user set the bssid
			 * and the network does bradcast and that those two bssid matches
			 */ 
			(!apset && ssidset && ssidbroad && ssidmatch) 
			){
			
			
				/* if the essid is hidden replace it with the
				* essid provided by the user.
				*/
				if (!ssidbroad){
					strncpy(tmp_ssid, ieee->current_network.ssid, IW_ESSID_MAX_SIZE);
					tmp_ssid_len = ieee->current_network.ssid_len;
				}
				memcpy(&ieee->current_network, net, sizeof(struct ieee80211_network));
				
				if (!ssidbroad){
					strncpy(ieee->current_network.ssid, tmp_ssid, IW_ESSID_MAX_SIZE);
					ieee->current_network.ssid_len = tmp_ssid_len;
				}
				printk(KERN_INFO"Linking with %s\n",ieee->current_network.ssid);
				
				if (ieee->iw_mode == IW_MODE_INFRA){
					ieee->state = IEEE80211_ASSOCIATING;
					ieee->beinretry = false;
					
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)					
					queue_work(ieee->wq, &ieee->associate_procedure_wq);
#else
					schedule_task(&ieee->associate_procedure_wq);
#endif
				}else{
					if(ieee80211_is_54g(ieee->current_network) && 
						(ieee->modulation & IEEE80211_OFDM_MODULATION)){
						ieee->rate = 540;
						printk(KERN_INFO"Using G rates\n");
					}else{
						ieee->rate = 110;
						printk(KERN_INFO"Using B rates\n");
					}
					ieee->state = IEEE80211_LINKED;
					ieee->beinretry = false;
				}
			
		}
	}

}

void ieee80211_softmac_check_all_nets(struct ieee80211_device *ieee)
{
	
	unsigned long flags;
	struct ieee80211_network *target;
	
	spin_lock_irqsave(&ieee->lock, flags);
			
	list_for_each_entry(target, &ieee->network_list, list) {
		
		/* if the state become different that NOLINK means
		 * we had found what we are searching for
		 */
		if (ieee->state != IEEE80211_NOLINK) 
			break;
			
			ieee80211_softmac_new_net(ieee, target);
	}
	
	spin_unlock_irqrestore(&ieee->lock, flags);
	
}


static inline u16 auth_parse(struct sk_buff *skb, u8** challenge, int *chlen)
{
	struct ieee80211_authentication *a;
	u8 *t;
	if (skb->len <  (sizeof(struct ieee80211_authentication)-sizeof(struct ieee80211_info_element))){ 
		IEEE80211_DEBUG_MGMT("invalid len in auth resp: %d\n",skb->len);
		return 0xcafe;
	}
	*challenge = NULL;
	a = (struct ieee80211_authentication*) skb->data;
	if(skb->len > (sizeof(struct ieee80211_authentication) +3)){
		t = skb->data + sizeof(struct ieee80211_authentication);
		
		if(*(t++) == MFIE_TYPE_CHALLENGE){
			*chlen = *(t++);
			*challenge = (u8*)kmalloc(*chlen, GFP_ATOMIC);
			memcpy(*challenge, t, *chlen);
		}
	}
	
	return cpu_to_le16(a->status);
	
}


int auth_rq_parse(struct sk_buff *skb,u8* dest)
{
	struct ieee80211_authentication *a;
	
	if (skb->len <  (sizeof(struct ieee80211_authentication)-sizeof(struct ieee80211_info_element))){ 
		IEEE80211_DEBUG_MGMT("invalid len in auth request: %d\n",skb->len);	
		return -1;
	}
	a = (struct ieee80211_authentication*) skb->data;
	
	memcpy(dest,a->header.addr2, ETH_ALEN);
	
	if (le16_to_cpu(a->algorithm) != WLAN_AUTH_OPEN) 
		return  WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG;
	
	return WLAN_STATUS_SUCCESS;
}

static short probe_rq_parse(struct ieee80211_device *ieee, struct sk_buff *skb, u8 *src)
{
	u8 *tag;
	u8 *skbend;
	u8 *ssid=NULL;
	u8 ssidlen = 0;
	
	struct ieee80211_hdr_3addr   *header =
		(struct ieee80211_hdr_3addr   *) skb->data;
	
	if (skb->len < sizeof (struct ieee80211_hdr_3addr  )) 
		return -1; /* corrupted */
	
	memcpy(src,header->addr2, ETH_ALEN);
	
	skbend = (u8*)skb->data + skb->len;
	
	tag = skb->data + sizeof (struct ieee80211_hdr_3addr  );
	
	while (tag+1 < skbend){
		if (*tag == 0){ 
			ssid = tag+2;
			ssidlen = *(tag+1);
			break;
		}
		tag++; /* point to the len field */
		tag = tag + *(tag); /* point to the last data byte of the tag */
		tag++; /* point to the next tag */
	}
	
	if (ssidlen == 0) return 1;
	
	if (!ssid) return 1; /* ssid not found in tagged param */
	return (!strncmp(ssid, ieee->current_network.ssid, ssidlen));
		
}

int assoc_rq_parse(struct sk_buff *skb,u8* dest)
{
	struct ieee80211_assoc_request_frame *a;
	
	if (skb->len < (sizeof(struct ieee80211_assoc_request_frame) - 
		sizeof(struct ieee80211_info_element))) { 
		
		IEEE80211_DEBUG_MGMT("invalid len in auth request:%d \n", skb->len);
		return -1;
	}
	
	a = (struct ieee80211_assoc_request_frame*) skb->data;
		
	memcpy(dest,a->header.addr2,ETH_ALEN);
	
	return 0;
}

static inline u16 assoc_parse(struct sk_buff *skb, int *aid)
{
	struct ieee80211_assoc_response_frame *a;
	if (skb->len <  sizeof(struct ieee80211_assoc_response_frame)){ 
		IEEE80211_DEBUG_MGMT("invalid len in auth resp: %d\n", skb->len);
		return 0xcafe;
	}
	
	a = (struct ieee80211_assoc_response_frame*) skb->data;
	*aid = le16_to_cpu(a->aid) & 0x3fff;
	return le16_to_cpu(a->status);
}

static inline void
ieee80211_rx_probe_rq(struct ieee80211_device *ieee, struct sk_buff *skb)
{
	u8 dest[ETH_ALEN];
	
	ieee->softmac_stats.rx_probe_rq++;
	if (probe_rq_parse(ieee, skb, dest)){
		ieee->softmac_stats.tx_probe_rs++;
		ieee80211_resp_to_probe(ieee, dest);
	}
}

static inline void
ieee80211_rx_auth_rq(struct ieee80211_device *ieee, struct sk_buff *skb)
{
	u8 dest[ETH_ALEN];
	int status;
	ieee->softmac_stats.rx_auth_rq++;
	
	if ((status = auth_rq_parse(skb, dest))!= -1){
		ieee80211_resp_to_auth(ieee, status, dest);
	}
	
}

static inline void
ieee80211_rx_assoc_rq(struct ieee80211_device *ieee, struct sk_buff *skb)
{
	
	u8 dest[ETH_ALEN];
	
	ieee->softmac_stats.rx_ass_rq++;
	if (assoc_rq_parse(skb,dest) != -1){
		ieee80211_resp_to_assoc_rq(ieee, dest);
	}
	
	printk(KERN_INFO"New client associated: "MAC_FMT"\n", MAC_ARG(dest));
	#if 0
	spin_lock_irqsave(&ieee->lock,flags);
	add_associate(ieee,dest);
	spin_unlock_irqrestore(&ieee->lock,flags);
	#endif
}



void ieee80211_sta_ps_send_null_frame(struct ieee80211_device *ieee, short pwr)
{
	
	struct sk_buff *buf = ieee80211_null_func(ieee, pwr);
	
	if (buf)
		softmac_ps_mgmt_xmit(buf, ieee);

} 


short ieee80211_sta_ps_sleep(struct ieee80211_device *ieee, u32 *time_h, u32 *time_l)
{	
	int timeout = ieee->ps_timeout;
	u8 dtim;
	/*if(ieee->ps == IEEE80211_PS_DISABLED ||
		ieee->iw_mode != IW_MODE_INFRA || 
		ieee->state != IEEE80211_LINKED)
		
		return 0;
	*/
	dtim = ieee->current_network.dtim_data;
	printk(KERN_INFO "DTIM\n");
	if(!(dtim & IEEE80211_DTIM_VALID))
		return 0;
	printk(KERN_INFO "VALID\n");
	ieee->current_network.dtim_data = IEEE80211_DTIM_INVALID;
		
	if(dtim & ((IEEE80211_DTIM_UCAST | IEEE80211_DTIM_MBCAST)& ieee->ps))
		return 2;
	printk(KERN_INFO "no UCAST\n");
	if(!time_after(jiffies, ieee->dev->trans_start + MSECS(timeout)))
		return 0;
	printk(KERN_INFO "aa\n");
	if(!time_after(jiffies, ieee->last_rx_ps_time + MSECS(timeout)))
		return 0;
	printk(KERN_INFO "bb\n");
	if((ieee->softmac_features & IEEE_SOFTMAC_SINGLE_QUEUE ) &&
		(ieee->mgmt_queue_tail != ieee->mgmt_queue_head))
		return 0;
	printk(KERN_INFO "cc\n");
	if(time_l){
		*time_l = ieee->current_network.last_dtim_sta_time[0] 
			+ (ieee->current_network.beacon_interval 
			* ieee->current_network.dtim_period) * 1000;
	}
	
	if(time_h){
		*time_h = ieee->current_network.last_dtim_sta_time[1];
		if(time_l && *time_l < ieee->current_network.last_dtim_sta_time[0])
			*time_h += 1;
	}
	ieee->ps_th=*time_h;
	ieee->ps_tl=*time_l;
	return 1;
	
	
}

inline void ieee80211_sta_ps(struct ieee80211_device *ieee)
{

	u32 th,tl;
	short sleep;
	
	unsigned long flags,flags2;
	spin_lock_irqsave(&ieee->lock, flags);
	
	if((ieee->ps == IEEE80211_PS_DISABLED ||
		ieee->iw_mode != IW_MODE_INFRA || 
		ieee->state != IEEE80211_LINKED)){
		
		spin_lock_irqsave(&ieee->mgmt_tx_lock, flags2);
		
		ieee80211_sta_wakeup(ieee, 1);	
		printk(KERN_WARNING "wakeup 1!\n" );
		spin_unlock_irqrestore(&ieee->mgmt_tx_lock, flags2);
	}
	
	sleep = ieee80211_sta_ps_sleep(ieee,&th, &tl);
	/* 2 wake, 1 sleep, 0 do nothing */
	if(sleep == 0)
		goto out;
	
	if(sleep == 1){
	
		if(ieee->sta_sleep == 1)
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
			queue_work(ieee->wq,&ieee->hw_sleep_wq);
#else
			schedule_task(&ieee->hw_sleep_wq);
#endif
		else if(ieee->sta_sleep == 0){
			spin_lock_irqsave(&ieee->mgmt_tx_lock, flags2);
			
			if(ieee->ps_is_queue_empty(ieee->dev)){
				
			
				ieee->sta_sleep = 2;
				
				ieee80211_sta_ps_send_null_frame(ieee,1);
				ieee80211_sta_ps_send_null_frame(ieee,1);
				
				ieee->ps_th = th;
				ieee->ps_tl = tl;
				ieee80211_ps_tx_ack(ieee,1);
			}	
			spin_unlock_irqrestore(&ieee->mgmt_tx_lock, flags2);
			
		}
		
		
	}else if(sleep == 2){
		spin_lock_irqsave(&ieee->mgmt_tx_lock, flags2);
			
		ieee80211_sta_wakeup(ieee,1);
		printk(KERN_WARNING "wakeup 2!\n" );
		spin_unlock_irqrestore(&ieee->mgmt_tx_lock, flags2);
	}

out:	
	spin_unlock_irqrestore(&ieee->lock, flags);
	
}

void ieee80211_sta_wakeup(struct ieee80211_device *ieee, short nl)
{
	if(ieee->sta_sleep == 0){
		if(nl){
			printk("Warning: driver is probably failing to report TX ps error\n");
			ieee80211_sta_ps_send_null_frame(ieee, 0);
		}
		return;
		
	}
	if(ieee->sta_sleep == 1) 
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
		queue_work(ieee->wq,&ieee->hw_wakeup_wq);
#else
		schedule_task(&ieee->hw_wakeup_wq);
#endif
		
	ieee->sta_sleep = 0;
	
	if(nl){
		ieee80211_sta_ps_send_null_frame(ieee, 0);
	}
}

void ieee80211_ps_tx_ack(struct ieee80211_device *ieee, short success)
{
	unsigned long flags,flags2;
	spin_lock_irqsave(&ieee->lock, flags);
	
	if(ieee->sta_sleep == 2){
		/* Null frame with PS bit set */
		if(success){
			ieee->sta_sleep = 1;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
			queue_work(ieee->wq,&ieee->hw_sleep_wq);
#else
			schedule_task(&ieee->hw_sleep_wq);
#endif
		}
		/* if the card report not success we can't be sure the AP
		 * has not RXed so we can't assume the AP believe us awake
		 */
	}
	/* 21112005 - tx again null without PS bit if lost */
	else {
	
		if((ieee->sta_sleep == 0) && !success){
			spin_lock_irqsave(&ieee->mgmt_tx_lock, flags2);
			ieee80211_sta_ps_send_null_frame(ieee, 0);
			spin_unlock_irqrestore(&ieee->mgmt_tx_lock, flags2);
		}
	}
	spin_unlock_irqrestore(&ieee->lock, flags);
}

inline int
ieee80211_rx_frame_softmac(struct ieee80211_device *ieee, struct sk_buff *skb,
			struct ieee80211_rx_stats *rx_stats, u16 type,
			u16 stype)
{
	struct ieee80211_hdr_3addr *header;
	u16 errcode;
	u8* challenge;
	int chlen = 0;
	int aid;
	header = (struct ieee80211_hdr_3addr *) skb->data;
	if(!ieee->proto_started)
		return 0;
	
	if(ieee->sta_sleep || (ieee->ps != IEEE80211_PS_DISABLED &&
		ieee->iw_mode == IW_MODE_INFRA && 
		ieee->state == IEEE80211_LINKED)) {
		tasklet_schedule(&ieee->ps_task);}
				
	if(WLAN_FC_GET_STYPE(header->frame_ctl) != IEEE80211_STYPE_PROBE_RESP &&
		WLAN_FC_GET_STYPE(header->frame_ctl) != IEEE80211_STYPE_BEACON)
		ieee->last_rx_ps_time = jiffies;
		
	switch (WLAN_FC_GET_STYPE(header->frame_ctl)) {
	
		case IEEE80211_STYPE_ASSOC_RESP:
		case IEEE80211_STYPE_REASSOC_RESP:
		
			IEEE80211_DEBUG_MGMT("received [RE]ASSOCIATION RESPONSE (%d)\n",
					WLAN_FC_GET_STYPE(header->frame_ctl));
			if ((ieee->softmac_features & IEEE_SOFTMAC_ASSOCIATE) &&
				ieee->state == IEEE80211_ASSOCIATING_AUTHENTICATED && 
				ieee->iw_mode == IW_MODE_INFRA){
				if (0 == (errcode=assoc_parse(skb, &aid))){
					
					ieee->state=IEEE80211_LINKED;
					ieee->assoc_id = aid;
					ieee->softmac_stats.rx_ass_ok++;
				    
					ieee80211_associate_complete(ieee);
				}else{
					ieee->softmac_stats.rx_ass_err++;

						
					
					ieee80211_associate_abort(ieee); 
				}
			}
			break;
		
		case IEEE80211_STYPE_ASSOC_REQ:
		case IEEE80211_STYPE_REASSOC_REQ:
		
			if ((ieee->softmac_features & IEEE_SOFTMAC_ASSOCIATE) &&
				ieee->iw_mode == IW_MODE_MASTER)
					
				ieee80211_rx_assoc_rq(ieee, skb);
			break;
			
		case IEEE80211_STYPE_AUTH:
		
			if (ieee->softmac_features & IEEE_SOFTMAC_ASSOCIATE){
				if (ieee->state == IEEE80211_ASSOCIATING_AUTHENTICATING && 
				ieee->iw_mode == IW_MODE_INFRA){
			
						IEEE80211_DEBUG_MGMT("Received authentication response");
						
						if (0 == (errcode=auth_parse(skb, &challenge, &chlen))){
							if(ieee->open_wep || !challenge){
								ieee->state = IEEE80211_ASSOCIATING_AUTHENTICATED;
								ieee->softmac_stats.rx_auth_rs_ok++;
								
								ieee80211_associate_step2(ieee);
							}else{
								ieee80211_auth_challenge(ieee, challenge, chlen);
							}
						}else{
							ieee->softmac_stats.rx_auth_rs_err++;
							IEEE80211_DEBUG_MGMT("Authentication respose status code 0x%x",errcode);
							ieee80211_associate_abort(ieee);
						}
						
					}else if (ieee->iw_mode == IW_MODE_MASTER){
						ieee80211_rx_auth_rq(ieee, skb);
					}
				}
			break;
				
		case IEEE80211_STYPE_PROBE_REQ:
		
			if ((ieee->softmac_features & IEEE_SOFTMAC_PROBERS) && 
				((ieee->iw_mode == IW_MODE_ADHOC || 
				ieee->iw_mode == IW_MODE_MASTER) &&
				ieee->state == IEEE80211_LINKED))
				
				ieee80211_rx_probe_rq(ieee, skb);
			break;
			
		case IEEE80211_STYPE_DISASSOC:
		case IEEE80211_STYPE_DEAUTH:
			/* FIXME for now repeat all the association procedure 
			* both for disassociation and deauthentication
			*/
			if ((ieee->softmac_features & IEEE_SOFTMAC_ASSOCIATE) &&
				ieee->state == IEEE80211_LINKED && 
				ieee->iw_mode == IW_MODE_INFRA){
				
				ieee->state = IEEE80211_ASSOCIATING;
				ieee->softmac_stats.reassoc++;
				
				notify_wx_assoc_event(ieee);
				
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)								
				queue_work(ieee->wq, &ieee->associate_procedure_wq);
#else
				schedule_task(&ieee->associate_procedure_wq);
#endif
			}
			
			break;
		
		default: 
			return -1;
			break;
	}
	
	return 0;
}



/* following are for a simplier TX queue management.
 * Instead of using netif_[stop/wake]_queue the driver
 * will uses these two function (plus a reset one), that
 * will internally uses the kernel netif_* and takes
 * care of the ieee802.11 fragmentation.
 * So the driver receives a fragment per time and might
 * call the stop function when it want without take care
 * to have enought room to TX an entire packet.
 * This might be useful if each fragment need it's own
 * descriptor, thus just keep a total free memory > than
 * the max fragmentation treshold is not enought.. If the
 * ieee802.11 stack passed a TXB struct then you needed  
 * to keep N free descriptors where 
 * N = MAX_PACKET_SIZE / MIN_FRAG_TRESHOLD
 * In this way you need just one and the 802.11 stack
 * will take care of buffering fragments and pass them to 
 * to the driver later, when it wakes the queue.
 */ 
 
void ieee80211_softmac_xmit(struct ieee80211_txb *txb, struct ieee80211_device *ieee)
{
	
	
	unsigned long flags;
	int  i;
	
	spin_lock_irqsave(&ieee->lock,flags);
	#if 0
	if(ieee->queue_stop){
		IEEE80211DMESG("EE: IEEE hard_start_xmit invoked when kernel queue should be stopped");
		netif_stop_queue(ieee->dev);
		ieee->ieee_stats.swtxstop++;
		err = 1;
		goto exit;
	}
	
	ieee->stats.tx_bytes+=skb->len;
	
	
	txb=ieee80211_skb_to_txb(ieee,skb);
	
	
	if(txb==NULL){
		IEEE80211DMESG("WW: IEEE stack failed to provide txb");
		err = 1;
		goto exit;
	}
	#endif
	if ((ieee->queue_stop)&&(ieee->tx_pending.txb != NULL)) {
		goto lost;
	}
	/* called with 2nd parm 0, no tx mgmt lock required */
	ieee80211_sta_wakeup(ieee,0);
	
	for(i = 0; i < txb->nr_frags; i++) {
	
		if (ieee->queue_stop){
			if(ieee->tx_pending.txb != NULL) {
			}
			ieee->tx_pending.txb = txb;
			ieee->tx_pending.frag = i;
			goto exit;
		}else{
			ieee->softmac_data_hard_start_xmit(
				txb->fragments[i],
				ieee->dev,ieee->rate);
			ieee->stats.tx_packets++;
			ieee->stats.tx_bytes += txb->fragments[i]->len;
			ieee->dev->trans_start = jiffies; 
		}
	}	
	lost:
	
	exit:
	atomic_dec(&ieee->tx_pending_txb);
	spin_unlock_irqrestore(&ieee->lock,flags);
	
}

/* called with ieee->lock acquired */
void ieee80211_resume_tx(struct ieee80211_device *ieee)
{
	int i;
	for(i = ieee->tx_pending.frag; i < ieee->tx_pending.txb->nr_frags; i++) {
		
		if (ieee->queue_stop){
			ieee->tx_pending.frag = i;
			return;
		}else{
		
			ieee->softmac_data_hard_start_xmit( 
				ieee->tx_pending.txb->fragments[i],
				ieee->dev,ieee->rate);
			ieee->stats.tx_packets++;
			ieee->dev->trans_start = jiffies;
		}
	}
	
	
	ieee->tx_pending.txb = NULL;
}


void ieee80211_reset_queue(struct ieee80211_device *ieee)
{
	unsigned long flags;
	struct sk_buff *ret;
	
	spin_lock_irqsave(&ieee->lock,flags);
	while((ret = dequeue_mgmt(ieee)) != NULL) {
		dev_kfree_skb_any(ret);
	}
	init_mgmt_queue(ieee);
	if (ieee->tx_pending.txb){
		ieee->tx_pending.txb = NULL;
	}
	ieee->queue_stop = 0;
	spin_unlock_irqrestore(&ieee->lock,flags);

}

void ieee80211_wake_queue(struct ieee80211_device *ieee)
{

	unsigned long flags;
	struct sk_buff *skb;
	struct ieee80211_hdr_3addr  *header;
	
	spin_lock_irqsave(&ieee->lock,flags);
	if (! ieee->queue_stop) goto exit;
	
	ieee->queue_stop = 0;
	
	if(ieee->softmac_features & IEEE_SOFTMAC_SINGLE_QUEUE){
		while (!ieee->queue_stop && (skb = dequeue_mgmt(ieee))){
			
			header = (struct ieee80211_hdr_3addr  *) skb->data;
			
			header->seq_ctl = cpu_to_le16(ieee->seq_ctrl << 4);

			if (ieee->seq_ctrl == 0xFFF)
				ieee->seq_ctrl = 0;
			else
				ieee->seq_ctrl++;

			ieee->softmac_data_hard_start_xmit(skb,ieee->dev,ieee->basic_rate);

			dev_kfree_skb_any(skb);
		}
	}
	if (!ieee->queue_stop && ieee->tx_pending.txb)
		ieee80211_resume_tx(ieee);
	
	if (!ieee->queue_stop && netif_queue_stopped(ieee->dev)){
		ieee->softmac_stats.swtxawake++;
		netif_wake_queue(ieee->dev);
	}
	
exit :
	spin_unlock_irqrestore(&ieee->lock,flags);
}


void ieee80211_stop_queue(struct ieee80211_device *ieee)
{

	if (! netif_queue_stopped(ieee->dev)){
		netif_stop_queue(ieee->dev);
		ieee->softmac_stats.swtxstop++;
	}
	ieee->queue_stop = 1;
	
}


inline void ieee80211_randomize_cell(struct ieee80211_device *ieee)
{
	
	get_random_bytes(ieee->current_network.bssid, ETH_ALEN);
	
	/* an IBSS cell address must have the two less significant
	 * bits of the first byte = 2 
	 */
	ieee->current_network.bssid[0] &= ~0x01;
	ieee->current_network.bssid[0] |= 0x02;
}

/* called in user context only */
void ieee80211_start_master_bss(struct ieee80211_device *ieee)
{
	ieee->assoc_id = 1;
	
	if (ieee->current_network.ssid_len == 0){
		strncpy(ieee->current_network.ssid, 
			IEEE80211_DEFAULT_TX_ESSID,
			IW_ESSID_MAX_SIZE);
			
		ieee->current_network.ssid_len = strlen(IEEE80211_DEFAULT_TX_ESSID);
		ieee->ssid_set = 1;
	}
	
	memcpy(ieee->current_network.bssid, ieee->dev->dev_addr, ETH_ALEN);
	 
	ieee->set_chan(ieee->dev, ieee->current_network.channel);
	ieee->state = IEEE80211_LINKED;

#ifdef LED_SHIN
	ieee->ieee80211_led_contorl(ieee->dev, LED_CTL_LINK);
#endif
	ieee->link_change(ieee->dev);
	notify_wx_assoc_event(ieee);
	
	if (ieee->data_hard_resume)
		ieee->data_hard_resume(ieee->dev);
	
	netif_carrier_on(ieee->dev);
}

void ieee80211_start_monitor_mode(struct ieee80211_device *ieee)
{
	if(ieee->raw_tx){
		
		if (ieee->data_hard_resume)
			ieee->data_hard_resume(ieee->dev);
	
		netif_carrier_on(ieee->dev);
	}
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void ieee80211_start_ibss_wq(struct work_struct *work)
{
	struct delayed_work *dwork = container_of(work, struct delayed_work, work);
	struct ieee80211_device *ieee = container_of(dwork, struct ieee80211_device, start_ibss_wq);
#else
void ieee80211_start_ibss_wq(struct ieee80211_device *ieee)
{
#endif
	
	/* iwconfig mode ad-hoc will schedule this and return
	 * on the other hand this will block further iwconfig SET
	 * operations because of the wx_sem hold.
	 * Anyway some most set operations set a flag to speed-up
	 * (abort) this wq (when syncro scanning) before sleeping 
	 * on the semaphore
	 */
	
	down(&ieee->wx_sem);
	

	if (ieee->current_network.ssid_len == 0){
		strcpy(ieee->current_network.ssid,IEEE80211_DEFAULT_TX_ESSID);
		ieee->current_network.ssid_len = strlen(IEEE80211_DEFAULT_TX_ESSID);
		ieee->ssid_set = 1;
	} 
	
#ifdef LED_SHIN
	ieee->ieee80211_led_contorl(ieee->dev, LED_CTL_SITE_SURVEY);
#endif
	
	/* check if we have this cell in our network list */
	ieee80211_softmac_check_all_nets(ieee);
	
	/* if not then the state is not linked. Maybe the user swithced to
	 * ad-hoc mode just after being in monitor mode, or just after
	 * being very few time in managed mode (so the card have had no
	 * time to scan all the chans..) or we have just run up the iface
	 * after setting ad-hoc mode. So we have to give another try..
	 * Here, in ibss mode, should be safe to do this without extra care
	 * (in bss mode we had to make sure no-one tryed to associate when
	 * we had just checked the ieee->state and we was going to start the
	 * scan) beacause in ibss mode the ieee80211_new_net function, when
	 * finds a good net, just set the ieee->state to IEEE80211_LINKED,
	 * so, at worst, we waste a bit of time to initiate an unneeded syncro
	 * scan, that will stop at the first round because it sees the state
	 * associated.
	 */
	if (ieee->state == IEEE80211_NOLINK)
		ieee80211_start_scan_syncro(ieee);

	/* the network definitively is not here.. create a new cell */
	if (ieee->state == IEEE80211_NOLINK){
		printk("creating new IBSS cell\n"); 
		if(!ieee->wap_set)
			ieee80211_randomize_cell(ieee);
		
		if(ieee->modulation & IEEE80211_CCK_MODULATION){
		
			ieee->current_network.rates_len = 4;
			
			ieee->current_network.rates[0] = IEEE80211_BASIC_RATE_MASK | IEEE80211_CCK_RATE_1MB;
			ieee->current_network.rates[1] = IEEE80211_BASIC_RATE_MASK | IEEE80211_CCK_RATE_2MB;
			ieee->current_network.rates[2] = IEEE80211_BASIC_RATE_MASK | IEEE80211_CCK_RATE_5MB;
			ieee->current_network.rates[3] = IEEE80211_BASIC_RATE_MASK | IEEE80211_CCK_RATE_11MB;
				
		}else
			ieee->current_network.rates_len = 0;
		
		if(ieee->modulation & IEEE80211_OFDM_MODULATION){
			ieee->current_network.rates_ex_len = 8;
			
			ieee->current_network.rates_ex[0] = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_6MB;
			ieee->current_network.rates_ex[1] = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_9MB;
			ieee->current_network.rates_ex[2] = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_12MB;
			ieee->current_network.rates_ex[3] = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_18MB;
			ieee->current_network.rates_ex[4] = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_24MB;
			ieee->current_network.rates_ex[5] = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_36MB;
			ieee->current_network.rates_ex[6] = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_48MB;
			ieee->current_network.rates_ex[7] = IEEE80211_BASIC_RATE_MASK | IEEE80211_OFDM_RATE_54MB;
			
			ieee->rate = 540;
		}else{
			ieee->current_network.rates_ex_len = 0;
			ieee->rate = 110;
		}
#ifdef WMM	
		ieee->current_network.QoS_Enable = 0;
#endif
	
		ieee->current_network.atim_window = 0;
		ieee->current_network.capability = WLAN_CAPABILITY_IBSS;
		if(ieee->short_slot)
			ieee->current_network.capability |= WLAN_CAPABILITY_SHORT_SLOT;
				 
	}
	
	ieee->state = IEEE80211_LINKED;
		
#ifdef LED_SHIN
	ieee->ieee80211_led_contorl(ieee->dev, LED_CTL_LINK);
#endif
		
	ieee->set_chan(ieee->dev, ieee->current_network.channel);
	ieee->link_change(ieee->dev);
	
	notify_wx_assoc_event(ieee);
	
	ieee80211_start_send_beacons(ieee);
	printk(KERN_WARNING "after sending beacon packet!\n");
	
	if (ieee->data_hard_resume)
		ieee->data_hard_resume(ieee->dev);
	
	netif_carrier_on(ieee->dev);
	
	up(&ieee->wx_sem);
}

inline void ieee80211_start_ibss(struct ieee80211_device *ieee)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
	queue_delayed_work(ieee->wq, &ieee->start_ibss_wq, 150); 
#else
	schedule_task(&ieee->start_ibss_wq);
#endif
}

/* this is called only in user context, with wx_sem held */
void ieee80211_start_bss(struct ieee80211_device *ieee)
{
	unsigned long flags;
	/* check if we have already found the net we
	 * are interested in (if any).
	 * if not (we are disassociated and we are not
	 * in associating / authenticating phase) start the background scanning.
	 */

#ifdef LED_SHIN
	struct net_device *dev = ieee->dev;
	ieee->ieee80211_led_contorl(dev, LED_CTL_SITE_SURVEY);
#endif
	ieee80211_softmac_check_all_nets(ieee);
	
	/* ensure no-one start an associating process (thus setting
	 * the ieee->state to ieee80211_ASSOCIATING) while we
	 * have just cheked it and we are going to enable scan.
	 * The ieee80211_new_net function is always called with
	 * lock held (from both ieee80211_softmac_check_all_nets and
	 * the rx path), so we cannot be in the middle of such function
	 */
	spin_lock_irqsave(&ieee->lock, flags);
	
	if (ieee->state == IEEE80211_NOLINK)
		ieee80211_start_scan(ieee);
	
	spin_unlock_irqrestore(&ieee->lock, flags);
}

/* called only in userspace context */
void ieee80211_disassociate(struct ieee80211_device *ieee)
{
	netif_carrier_off(ieee->dev);
	
	if (ieee->softmac_features & IEEE_SOFTMAC_TX_QUEUE)
			ieee80211_reset_queue(ieee);
	
	if (ieee->data_hard_stop)
			ieee->data_hard_stop(ieee->dev);
	
	ieee->state = IEEE80211_NOLINK;
        ieee->wap_set = 0;
        ieee->ssid_set = 0;

	notify_wx_assoc_event(ieee);
	
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void ieee80211_associate_retry_wq(struct work_struct *work)
{
	struct delayed_work *dwork = container_of(work, struct delayed_work, work);
	struct ieee80211_device *ieee = container_of(dwork, struct ieee80211_device, associate_retry_wq);
#else
void ieee80211_associate_retry_wq(struct ieee80211_device *ieee)
{
#endif
	unsigned long flags;
	
	down(&ieee->wx_sem);
	if(!ieee->proto_started)
		goto exit;
		
	if(ieee->state != IEEE80211_ASSOCIATING_RETRY)
		goto exit;
		
	/* until we do not set the state to IEEE80211_NOLINK 
	* there are no possibility to have someone else trying
	* to start an association procdure (we get here with
	* ieee->state = IEEE80211_ASSOCIATING).
	* When we set the state to IEEE80211_NOLINK it is possible
	* that the RX path run an attempt to associate, but
	* both ieee80211_softmac_check_all_nets and the
	* RX path works with ieee->lock held so there are no
	* problems. If we are still disassociated then start a scan.
	* the lock here is necessary to ensure no one try to start
	* an association procedure when we have just checked the 
	* state and we are going to start the scan.
	*/
	ieee->state = IEEE80211_NOLINK;
	ieee->beinretry = true;
	ieee80211_softmac_check_all_nets(ieee);
	
	spin_lock_irqsave(&ieee->lock, flags);
	
	if(ieee->state == IEEE80211_NOLINK){
		ieee->beinretry = false;
		ieee->actscanning = true;
		ieee80211_start_scan(ieee);
	}
	
	spin_unlock_irqrestore(&ieee->lock, flags);

exit:
	up(&ieee->wx_sem);
}

struct sk_buff *ieee80211_get_beacon_(struct ieee80211_device *ieee)
{
	u8 broadcast_addr[] = {0xff,0xff,0xff,0xff,0xff,0xff};
	
	struct sk_buff *skb;
	struct ieee80211_probe_response *b;
	
	skb = ieee80211_probe_resp(ieee, broadcast_addr);
	
	if (!skb) 
		return NULL;
	
	b = (struct ieee80211_probe_response *) skb->data;
	b->header.frame_ctl = cpu_to_le16(IEEE80211_STYPE_BEACON);
		
	return skb;
	
}

struct sk_buff *ieee80211_get_beacon(struct ieee80211_device *ieee)
{
	struct sk_buff *skb;
	struct ieee80211_probe_response *b;
	
	skb = ieee80211_get_beacon_(ieee);
	if(!skb) 
		return NULL;
		
	b = (struct ieee80211_probe_response *) skb->data;	
	b->header.seq_ctrl = cpu_to_le16(ieee->seq_ctrl << 4);
	
	if (ieee->seq_ctrl == 0xFFF)
		ieee->seq_ctrl = 0;
	else
		ieee->seq_ctrl++;
	
	return skb;
}

void ieee80211_softmac_stop_protocol(struct ieee80211_device *ieee)
{
	ieee->sync_scan_hurryup = 1;
	down(&ieee->wx_sem);
	ieee80211_stop_protocol(ieee);
	up(&ieee->wx_sem);
}


void ieee80211_stop_protocol(struct ieee80211_device *ieee)
{
	u8 i = 0;
	
	if (!ieee->proto_started)
		return;
	ieee->proto_started = 0;
	
	ieee80211_stop_send_beacons(ieee);
	
	del_timer_sync(&ieee->associate_timer);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
	cancel_delayed_work(&ieee->associate_retry_wq);	
    	cancel_delayed_work(&ieee->start_ibss_wq); 
#endif	
	ieee80211_stop_scan(ieee);

	ieee80211_disassociate(ieee);

	for(i = 0; i < ETH_ALEN; i++)
		ieee->current_network.bssid[i] = 0x22;
}

void ieee80211_softmac_start_protocol(struct ieee80211_device *ieee)
{
	ieee->sync_scan_hurryup = 0;
	down(&ieee->wx_sem);
	ieee80211_start_protocol(ieee);
	up(&ieee->wx_sem);
}

void ieee80211_start_protocol(struct ieee80211_device *ieee)
{
	short ch = 0;

	if (ieee->proto_started)
		return;
		
	ieee->proto_started = 1;
	
	if (ieee->current_network.channel == 0){
		do{
			ch++;
			if (ch > MAX_CHANNEL_NUMBER) 
				return; /* no channel found */
				
		}while(!ieee->channel_map[ch]);
		
		ieee->current_network.channel = ch;
	}
	
	if (ieee->current_network.beacon_interval == 0)
		ieee->current_network.beacon_interval = 100;

	ieee->set_chan(ieee->dev,ieee->current_network.channel);
	
	ieee->last_seq_num = -1;
	ieee->last_frag_num = -1;
	ieee->last_packet_time = 0;


	/* if the user set the MAC of the ad-hoc cell and then
	 * switch to managed mode, shall we  make sure that association
	 * attempts does not fail just because the user provide the essid
	 * and the nic is still checking for the AP MAC ??
	 */

	if (ieee->iw_mode == IW_MODE_INFRA)
		ieee80211_start_bss(ieee);
		
	else if (ieee->iw_mode == IW_MODE_ADHOC)
		ieee80211_start_ibss(ieee);
		
	else if (ieee->iw_mode == IW_MODE_MASTER)
		ieee80211_start_master_bss(ieee);
		
	else if(ieee->iw_mode == IW_MODE_MONITOR)
		ieee80211_start_monitor_mode(ieee);	
}


#define DRV_NAME  "Ieee80211"
void ieee80211_softmac_init(struct ieee80211_device *ieee)
{
	memset(&ieee->current_network, 0, sizeof(struct ieee80211_network));
	
	ieee->state = IEEE80211_NOLINK;
	ieee->sync_scan_hurryup = 0;
	ieee->seq_ctrl = 0;
	
	ieee->assoc_id = 0;
	ieee->queue_stop = 0;
	ieee->scanning = 0;
	ieee->scan_watchdog = 0;
	ieee->softmac_features = 0; 
	ieee->wap_set = 0;
	ieee->ssid_set = 0;
	ieee->proto_started = 0;
	ieee->basic_rate = IEEE80211_DEFAULT_BASIC_RATE;
	ieee->rate = 3;
	ieee->ps = IEEE80211_PS_DISABLED;
	ieee->sta_sleep = 0;
	ieee->bHwRadioOff = false;
	ieee->bInactivePs = false;
	ieee->actscanning = false;
	ieee->beinretry = false;
	ieee->ListenInterval = 2;
	ieee->NumRxData = 0;
	init_mgmt_queue(ieee);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)	
	init_timer(&ieee->scan_timer);
	ieee->scan_timer.data = (unsigned long)ieee;
	ieee->scan_timer.function = ieee80211_softmac_scan_cb;
#endif
	ieee->tx_pending.txb = NULL;
	
	init_timer(&ieee->associate_timer);
	ieee->associate_timer.data = (unsigned long)ieee;
	ieee->associate_timer.function = ieee80211_associate_abort_cb;

	init_timer(&ieee->beacon_timer);
	ieee->beacon_timer.data = (unsigned long) ieee;
	ieee->beacon_timer.function = ieee80211_send_beacon_cb;
	
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
#ifdef PF_SYNCTHREAD
	ieee->wq = create_workqueue(DRV_NAME,0);
#else	
	ieee->wq = create_workqueue(DRV_NAME);
#endif
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	INIT_WORK(&ieee->softmac_scan_wq,(void*) ieee80211_softmac_scan_wq,ieee);
	INIT_WORK(&ieee->start_ibss_wq,(void*) ieee80211_start_ibss_wq,ieee);	
	INIT_WORK(&ieee->associate_retry_wq,(void*) ieee80211_associate_retry_wq,ieee);	
	INIT_WORK(&ieee->associate_complete_wq,(void*) ieee80211_associate_complete_wq,ieee);	
	INIT_WORK(&ieee->associate_procedure_wq,(void*) ieee80211_associate_procedure_wq,ieee);
	INIT_WORK(&ieee->wx_sync_scan_wq,(void*) ieee80211_wx_sync_scan_wq,ieee);
#else
	INIT_DELAYED_WORK(&ieee->softmac_scan_wq,(void*) ieee80211_softmac_scan_wq);
	INIT_DELAYED_WORK(&ieee->start_ibss_wq,(void*)ieee80211_start_ibss_wq);
	INIT_DELAYED_WORK(&ieee->associate_retry_wq,(void*) ieee80211_associate_retry_wq);		
	INIT_WORK(&ieee->associate_complete_wq,(void*) ieee80211_associate_complete_wq);
	INIT_WORK(&ieee->associate_procedure_wq,(void*) ieee80211_associate_procedure_wq);	
	INIT_WORK(&ieee->wx_sync_scan_wq,(void*) ieee80211_wx_sync_scan_wq);

#endif	
#else
	tq_init(&ieee->softmac_scan_wq,(void*) ieee80211_softmac_scan_wq,ieee);
	tq_init(&ieee->start_ibss_wq,(void*) ieee80211_start_ibss_wq,ieee);	
	tq_init(&ieee->associate_retry_wq,(void*) ieee80211_associate_retry_wq,ieee);	
	tq_init(&ieee->associate_complete_wq,(void*) ieee80211_associate_complete_wq,ieee);	
	tq_init(&ieee->associate_procedure_wq,(void*) ieee80211_associate_procedure_wq,ieee);	
	tq_init(&ieee->wx_sync_scan_wq,(void*) ieee80211_wx_sync_scan_wq,ieee);
#endif

	sema_init(&ieee->wx_sem, 1);
	sema_init(&ieee->scan_sem, 1);
	
	spin_lock_init(&ieee->mgmt_tx_lock);
	spin_lock_init(&ieee->beacon_lock);
	
	tasklet_init(&ieee->ps_task,
	     (void(*)(unsigned long)) ieee80211_sta_ps,
	     (unsigned long)ieee);

}

void ieee80211_softmac_free(struct ieee80211_device *ieee)
{
	down(&ieee->wx_sem);
	
	del_timer_sync(&ieee->associate_timer);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
	cancel_delayed_work(&ieee->associate_retry_wq);
	destroy_workqueue(ieee->wq);
#endif	
	up(&ieee->wx_sem);
}

/******************************************************** 
 * Start of WPA code.                                   *
 * this is stolen from the ipw2200 driver               *
 ********************************************************/

 
static int ieee80211_wpa_enable(struct ieee80211_device *ieee, int value)
{
	/* This is called when wpa_supplicant loads and closes the driver
	 * interface. */

	ieee->wpa_enabled = value;
	return 0;
}

 
void ieee80211_wpa_assoc_frame(struct ieee80211_device *ieee, char *wpa_ie, int wpa_ie_len)
{
	/* make sure WPA is enabled */
	ieee80211_wpa_enable(ieee, 1);

	ieee80211_disassociate(ieee);
}


static int ieee80211_wpa_mlme(struct ieee80211_device *ieee, int command, int reason)
{
	
	int ret = 0;

	switch (command) {
	case IEEE_MLME_STA_DEAUTH:
		break;

	case IEEE_MLME_STA_DISASSOC:
		ieee80211_disassociate(ieee);
		break;

	default:

		ret = -EOPNOTSUPP;
	}

	return ret;
}


static int ieee80211_wpa_set_wpa_ie(struct ieee80211_device *ieee,
			      struct ieee_param *param, int plen)
{
	u8 *buf;

	if (param->u.wpa_ie.len > MAX_WPA_IE_LEN ||
	    (param->u.wpa_ie.len && param->u.wpa_ie.data == NULL))
		return -EINVAL;

	if (param->u.wpa_ie.len) {
		buf = kmalloc(param->u.wpa_ie.len, GFP_KERNEL);
		if (buf == NULL)
			return -ENOMEM;

		memcpy(buf, param->u.wpa_ie.data, param->u.wpa_ie.len);
		kfree(ieee->wpa_ie);
		ieee->wpa_ie = buf;
		ieee->wpa_ie_len = param->u.wpa_ie.len;
	} else {
		kfree(ieee->wpa_ie);
		ieee->wpa_ie = NULL;
		ieee->wpa_ie_len = 0;
	}

	ieee80211_wpa_assoc_frame(ieee, ieee->wpa_ie, ieee->wpa_ie_len);
	return 0;
}

#define AUTH_ALG_OPEN_SYSTEM			0x1
#define AUTH_ALG_SHARED_KEY			0x2

static int ieee80211_wpa_set_auth_algs(struct ieee80211_device *ieee, int value)
{
	
	struct ieee80211_security sec = {
		.flags = SEC_AUTH_MODE,
	};
	int ret = 0;

	if (value & AUTH_ALG_SHARED_KEY) {
		sec.auth_mode = WLAN_AUTH_SHARED_KEY;
		ieee->open_wep = 0;
	} else {
		sec.auth_mode = WLAN_AUTH_OPEN;
		ieee->open_wep = 1;
	}

	if (ieee->set_security)
		ieee->set_security(ieee->dev, &sec);
	else
		ret = -EOPNOTSUPP;

	return ret;
}



static int ieee80211_wpa_set_param(struct ieee80211_device *ieee, u8 name, u32 value)
{
	int ret=0;

	switch (name) {
	case IEEE_PARAM_WPA_ENABLED:
		ret = ieee80211_wpa_enable(ieee, value);
		break;

	case IEEE_PARAM_TKIP_COUNTERMEASURES:
		ieee->tkip_countermeasures=value;
		break;

	case IEEE_PARAM_DROP_UNENCRYPTED: {
		/* HACK:
		 *
		 * wpa_supplicant calls set_wpa_enabled when the driver
		 * is loaded and unloaded, regardless of if WPA is being
		 * used.  No other calls are made which can be used to
		 * determine if encryption will be used or not prior to
		 * association being expected.  If encryption is not being
		 * used, drop_unencrypted is set to false, else true -- we
		 * can use this to determine if the CAP_PRIVACY_ON bit should
		 * be set.
		 */
		struct ieee80211_security sec = {
			.flags = SEC_ENABLED,
			.enabled = value,
		};
 		ieee->drop_unencrypted = value;
		/* We only change SEC_LEVEL for open mode. Others
		 * are set by ipw_wpa_set_encryption.
		 */
		if (!value) {
			sec.flags |= SEC_LEVEL;
			sec.level = SEC_LEVEL_0;
		}
		else {
			sec.flags |= SEC_LEVEL;
			sec.level = SEC_LEVEL_1;
		}
		if (ieee->set_security)
			ieee->set_security(ieee->dev, &sec);
		break;
	}

	case IEEE_PARAM_PRIVACY_INVOKED:
		ieee->privacy_invoked=value;
		break;

	case IEEE_PARAM_AUTH_ALGS:
		ret = ieee80211_wpa_set_auth_algs(ieee, value);
		break;

	case IEEE_PARAM_IEEE_802_1X:
		ieee->ieee802_1x=value;
		break;

	default:

		ret = -EOPNOTSUPP;
	}

	return ret;
}

/* implementation borrowed from hostap driver */

static int ieee80211_wpa_set_encryption(struct ieee80211_device *ieee,
				  struct ieee_param *param, int param_len)
{
#if 0 
	int ret = 0;
	int group_key = 0;
	struct ieee80211_crypto_ops *ops;
	struct ieee80211_crypt_data **crypt;

	struct ieee80211_security sec = {
		.flags = 0,
	};

	param->u.crypt.err = 0;
	param->u.crypt.alg[IEEE_CRYPT_ALG_NAME_LEN - 1] = '\0';

	if (param_len !=
	    (int) ((char *) param->u.crypt.key - (char *) param) +
	    param->u.crypt.key_len) {
		printk("Len mismatch %d, %d\n", param_len,
			       param->u.crypt.key_len);
		return -EINVAL;
	}
	if (param->sta_addr[0] == 0xff && param->sta_addr[1] == 0xff &&
	    param->sta_addr[2] == 0xff && param->sta_addr[3] == 0xff &&
	    param->sta_addr[4] == 0xff && param->sta_addr[5] == 0xff) {
		if (param->u.crypt.idx >= WEP_KEYS)
			return -EINVAL;
		crypt = &ieee->crypt[param->u.crypt.idx];
	} else {
		return -EINVAL;
	}

	if (param->u.crypt.idx != 0)
		group_key = 1;

	sec.flags |= SEC_ENABLED | SEC_ENCRYPT;
	if (strcmp(param->u.crypt.alg, "none") == 0) {
		if (crypt) {
			sec.enabled = 0;
			sec.level = SEC_LEVEL_0;
			sec.flags |= SEC_LEVEL;
			ieee80211_crypt_delayed_deinit(ieee, crypt);
		}
		goto done;
	}
	sec.enabled = 1;
	sec.encrypt = 1;

	/* IPW HW cannot build TKIP MIC, host decryption still needed. */
	if (strcmp(param->u.crypt.alg, "TKIP") == 0) {
		if (group_key)
			ieee->host_mc_decrypt = 1;
		else
			ieee->host_encrypt_msdu = 1;
	}

	/*if (!(ieee->host_encrypt || ieee->host_encrypt_msdu ||
	   ieee->host_decrypt))
	   goto skip_host_crypt; */
	if (group_key ? !ieee->host_mc_decrypt :
	    !(ieee->host_encrypt || ieee->host_decrypt ||
	      ieee->host_encrypt_msdu))
		goto skip_host_crypt;

	ops = ieee80211_get_crypto_ops(param->u.crypt.alg);
	if (ops == NULL && strcmp(param->u.crypt.alg, "WEP") == 0) {
		request_module("ieee80211_crypt_wep");
		ops = ieee80211_get_crypto_ops(param->u.crypt.alg);
	} else if (ops == NULL && strcmp(param->u.crypt.alg, "TKIP") == 0) {
		request_module("ieee80211_crypt_tkip");
		ops = ieee80211_get_crypto_ops(param->u.crypt.alg);
	} else if (ops == NULL && strcmp(param->u.crypt.alg, "CCMP") == 0) {
		request_module("ieee80211_crypt_ccmp");
		ops = ieee80211_get_crypto_ops(param->u.crypt.alg);
	}
	if (ops == NULL) {
		printk("unknown crypto alg '%s'\n", param->u.crypt.alg);
		param->u.crypt.err = IEEE_CRYPT_ERR_UNKNOWN_ALG;
		ret = -EINVAL;
		goto done;
	}

	if (*crypt == NULL || (*crypt)->ops != ops) {
		struct ieee80211_crypt_data *new_crypt;

		ieee80211_crypt_delayed_deinit(ieee, crypt);

		new_crypt = (struct ieee80211_crypt_data *)
			kmalloc(sizeof(*new_crypt), GFP_KERNEL);
		if (new_crypt == NULL) {
			ret = -ENOMEM;
			goto done;
		}
		memset(new_crypt, 0, sizeof(struct ieee80211_crypt_data));
		new_crypt->ops = ops;
		if (new_crypt->ops && try_module_get(new_crypt->ops->owner))
			new_crypt->priv =
				new_crypt->ops->init(param->u.crypt.idx);

		if (new_crypt->priv == NULL) {
			kfree(new_crypt);
			param->u.crypt.err = IEEE_CRYPT_ERR_CRYPT_INIT_FAILED;
			ret = -EINVAL;
			goto done;
		}

		*crypt = new_crypt;
	}

	if (param->u.crypt.key_len > 0 && (*crypt)->ops->set_key &&
	    (*crypt)->ops->set_key(param->u.crypt.key,
				   param->u.crypt.key_len, param->u.crypt.seq,
				   (*crypt)->priv) < 0) {
		printk("key setting failed\n");
		param->u.crypt.err = IEEE_CRYPT_ERR_KEY_SET_FAILED;
		ret = -EINVAL;
		goto done;
	}

 skip_host_crypt:
	if (param->u.crypt.set_tx) {
		ieee->tx_keyidx = param->u.crypt.idx;
		sec.active_key = param->u.crypt.idx;
		sec.flags |= SEC_ACTIVE_KEY;
	} else
		sec.flags &= ~SEC_ACTIVE_KEY;

	if (param->u.crypt.alg != NULL) {
		memcpy(sec.keys[param->u.crypt.idx],
		       param->u.crypt.key,
		       param->u.crypt.key_len);
		sec.key_sizes[param->u.crypt.idx] = param->u.crypt.key_len;
		sec.flags |= (1 << param->u.crypt.idx);

		if (strcmp(param->u.crypt.alg, "WEP") == 0) {
			sec.flags |= SEC_LEVEL;
			sec.level = SEC_LEVEL_1;
		} else if (strcmp(param->u.crypt.alg, "TKIP") == 0) {
			sec.flags |= SEC_LEVEL;
			sec.level = SEC_LEVEL_2;
		} else if (strcmp(param->u.crypt.alg, "CCMP") == 0) {
			sec.flags |= SEC_LEVEL;
			sec.level = SEC_LEVEL_3;
		}
		/* Don't set sec level for group keys. */
		if (group_key)
			sec.flags &= ~SEC_LEVEL;
	}
 done:
	if (ieee->set_security)
		ieee->set_security(ieee->dev, &sec);

	/* Do not reset port if card is in Managed mode since resetting will
	 * generate new IEEE 802.11 authentication which may end up in looping
	 * with IEEE 802.1X.  If your hardware requires a reset after WEP
	 * configuration (for example... Prism2), implement the reset_port in
	 * the callbacks structures used to initialize the 802.11 stack. */
	if (ieee->reset_on_keychange &&
	    ieee->iw_mode != IW_MODE_INFRA &&
	    ieee->reset_port &&
	    ieee->reset_port(ieee->dev)) {
		printk("reset_port failed\n");
		param->u.crypt.err = IEEE_CRYPT_ERR_CARD_CONF_FAILED;
		return -EINVAL;
	}

	return ret;
}
#else	
	int ret = 0;
	int tmp = 0;
	struct ieee80211_crypto_ops *ops;
	struct ieee80211_crypt_data **crypt;

	struct ieee80211_security sec = {
		.flags = 0,
	};

	param->u.crypt.err = 0;
	param->u.crypt.alg[IEEE_CRYPT_ALG_NAME_LEN - 1] = '\0';

	if (param_len !=
	    (int) ((char *) param->u.crypt.key - (char *) param) +
	    param->u.crypt.key_len) {


		return -EINVAL;
	}
	if (param->sta_addr[0] == 0xff && param->sta_addr[1] == 0xff &&
	    param->sta_addr[2] == 0xff && param->sta_addr[3] == 0xff &&
	    param->sta_addr[4] == 0xff && param->sta_addr[5] == 0xff) {
		if (param->u.crypt.idx >= WEP_KEYS)
			return -EINVAL;
		crypt = &ieee->crypt[param->u.crypt.idx];
	} else {
		return -EINVAL;
	}

	if (strcmp(param->u.crypt.alg, "none") == 0) {
		if (crypt) {
			sec.enabled = 0;
			sec.level = SEC_LEVEL_0;
			sec.flags |= SEC_ENABLED | SEC_LEVEL;
			ieee80211_crypt_delayed_deinit(ieee, crypt);
		

		}
		goto done;
	
	}

	sec.enabled = 1;
	sec.flags |= SEC_ENABLED;


	/* IPW HW cannot build TKIP MIC, host decryption still needed. */
	if (!(ieee->host_encrypt || ieee->host_decrypt) &&
	    strcmp(param->u.crypt.alg, "TKIP")){
	
		goto skip_host_crypt;}

	ops = ieee80211_get_crypto_ops(param->u.crypt.alg);
	if (ops == NULL && strcmp(param->u.crypt.alg, "WEP") == 0) {
		request_module("ieee80211_crypt_wep");
		ops = ieee80211_get_crypto_ops(param->u.crypt.alg);
	} else if (ops == NULL && strcmp(param->u.crypt.alg, "TKIP") == 0) {
		request_module("ieee80211_crypt_tkip");
		ops = ieee80211_get_crypto_ops(param->u.crypt.alg);
	} else if (ops == NULL && strcmp(param->u.crypt.alg, "CCMP") == 0) {
		request_module("ieee80211_crypt_ccmp");
		ops = ieee80211_get_crypto_ops(param->u.crypt.alg);
	}

	if (ops == NULL) {

		param->u.crypt.err = IEEE_CRYPT_ERR_UNKNOWN_ALG;
		ret = -EINVAL;
		goto done;
	}

	if (*crypt == NULL || (*crypt)->ops != ops) {
		struct ieee80211_crypt_data *new_crypt;

		ieee80211_crypt_delayed_deinit(ieee, crypt);

		new_crypt = (struct ieee80211_crypt_data *)
			kmalloc(sizeof(*new_crypt), GFP_KERNEL);
		if (new_crypt == NULL) {
			ret = -ENOMEM;
			goto done;
		}
		memset(new_crypt, 0, sizeof(struct ieee80211_crypt_data));
		new_crypt->ops = ops;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
		if (new_crypt->ops && try_module_get(new_crypt->ops->owner))
#else
		if (new_crypt->ops && try_inc_mod_count(new_crypt->ops->owner))
#endif
			new_crypt->priv =
				new_crypt->ops->init(param->u.crypt.idx);

		if (new_crypt->priv == NULL) {
			kfree(new_crypt);
			param->u.crypt.err = IEEE_CRYPT_ERR_CRYPT_INIT_FAILED;
			ret = -EINVAL;
			goto done;
		}

		*crypt = new_crypt;
	}




	if (param->u.crypt.key_len > 0 && (*crypt)->ops->set_key){

	tmp = (*crypt)->ops->set_key(param->u.crypt.key,
				   param->u.crypt.key_len, param->u.crypt.seq,
				   (*crypt)->priv);

	if ( tmp < 0) {

		param->u.crypt.err = IEEE_CRYPT_ERR_KEY_SET_FAILED;
		ret = -EINVAL;
		goto done;
	}}


 skip_host_crypt:
	if (param->u.crypt.set_tx) {
		ieee->tx_keyidx = param->u.crypt.idx;
		sec.active_key = param->u.crypt.idx;
		sec.flags |= SEC_ACTIVE_KEY;
	} else
		sec.flags &= ~SEC_ACTIVE_KEY;

	if (param->u.crypt.alg != NULL) {
		memcpy(sec.keys[param->u.crypt.idx],
		       param->u.crypt.key,
		       param->u.crypt.key_len);
		sec.key_sizes[param->u.crypt.idx] = param->u.crypt.key_len;
		sec.flags |= (1 << param->u.crypt.idx);

		if (strcmp(param->u.crypt.alg, "WEP") == 0) {
			sec.flags |= SEC_LEVEL;
			sec.level = SEC_LEVEL_1;
		} else if (strcmp(param->u.crypt.alg, "TKIP") == 0) {
			sec.flags |= SEC_LEVEL;
			sec.level = SEC_LEVEL_2;
		} else if (strcmp(param->u.crypt.alg, "CCMP") == 0) {
			sec.flags |= SEC_LEVEL;
			sec.level = SEC_LEVEL_3;
		}
	}

 done:
	if (ieee->set_security)
		ieee->set_security(ieee->dev, &sec);

	/* Do not reset port if card is in Managed mode since resetting will
	 * generate new IEEE 802.11 authentication which may end up in looping
	 * with IEEE 802.1X.  If your hardware requires a reset after WEP
	 * configuration (for example... Prism2), implement the reset_port in
	 * the callbacks structures used to initialize the 802.11 stack. */
	if (ieee->reset_on_keychange &&
	    ieee->iw_mode != IW_MODE_INFRA &&
	    ieee->reset_port &&
	    ieee->reset_port(ieee->dev)) {

		param->u.crypt.err = IEEE_CRYPT_ERR_CARD_CONF_FAILED;
		return -EINVAL;
	}

	return ret;
}


int tmpflag = 0;
int ieee80211_wpa_supplicant_ioctl(struct ieee80211_device *ieee, struct iw_point *p)
{
	struct ieee_param *param;
	int ret=0;

	down(&ieee->wx_sem);

	if (p->length < sizeof(struct ieee_param) || !p->pointer){
		ret = -EINVAL;
		goto out;
	}
	
	param = (struct ieee_param *)kmalloc(p->length, GFP_KERNEL);
	if (param == NULL){
		ret = -ENOMEM;
		goto out;
	}
	if (copy_from_user(param, p->pointer, p->length)) {
		kfree(param);
		ret = -EFAULT;
		goto out;
	}
#if 0	
	if(ieee->state == IEEE80211_LINKED && tmpflag)
	{
		DMESG("cmd is %x,but linked,out.",param->cmd);
		tmpflag = 0;
		goto out;	
	}
#endif	
	udelay(5);	
	switch (param->cmd) {

	case IEEE_CMD_SET_WPA_PARAM:
		ret = ieee80211_wpa_set_param(ieee, param->u.wpa_param.name,
					param->u.wpa_param.value);
		break;

	case IEEE_CMD_SET_WPA_IE:
		tmpflag = 1;
		ret = ieee80211_wpa_set_wpa_ie(ieee, param, p->length);
		break;

	case IEEE_CMD_SET_ENCRYPTION:
		if (param->u.crypt.set_tx)
		{
			if (strcmp(param->u.crypt.alg, "CCMP") == 0)
				ieee->pairwise_key_type = KEY_TYPE_CCMP;
			else if (strcmp(param->u.crypt.alg, "TKIP") == 0)
				ieee->pairwise_key_type = KEY_TYPE_TKIP;
			else if (strcmp(param->u.crypt.alg, "WEP") == 0)
			{
				if (param->u.crypt.key_len == 13)
					ieee->pairwise_key_type = KEY_TYPE_WEP104;
				else if (param->u.crypt.key_len == 5)
					ieee->pairwise_key_type = KEY_TYPE_WEP40;
			} else {
				ieee->pairwise_key_type = KEY_TYPE_NA;
			}

			if (ieee->pairwise_key_type)
			{
				u8 zero_addr[6] = {0};
				if (memcmp(ieee->ap_mac_addr, zero_addr, 6) == 0)
					ieee->iw_mode = IW_MODE_ADHOC;
			}
		}
		
		ret = ieee80211_wpa_set_encryption(ieee, param, p->length);
		break;

	case IEEE_CMD_MLME:
		ret = ieee80211_wpa_mlme(ieee, param->u.mlme.command,
				   param->u.mlme.reason_code);
		break;

	default:
		ret = -EOPNOTSUPP;
		break;
	}
	udelay(100);	
	if (ret == 0 && copy_to_user(p->pointer, param, p->length))
		ret = -EFAULT;
	kfree(param);
out:
	up(&ieee->wx_sem);	
	
	return ret;
#endif
}

void notify_wx_assoc_event(struct ieee80211_device *ieee)
{
	union iwreq_data wrqu;
	wrqu.ap_addr.sa_family = ARPHRD_ETHER;
	if (ieee->state == IEEE80211_LINKED)
		memcpy(wrqu.ap_addr.sa_data, ieee->current_network.bssid, ETH_ALEN);
	else
		memset(wrqu.ap_addr.sa_data, 0, ETH_ALEN);

	wireless_send_event(ieee->dev, SIOCGIWAP, &wrqu, NULL);
}


#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
EXPORT_SYMBOL(ieee80211_get_beacon);
EXPORT_SYMBOL(ieee80211_wake_queue);
EXPORT_SYMBOL(ieee80211_stop_queue);
EXPORT_SYMBOL(ieee80211_reset_queue);
EXPORT_SYMBOL(ieee80211_softmac_stop_protocol);
EXPORT_SYMBOL(ieee80211_softmac_start_protocol);
EXPORT_SYMBOL(ieee80211_is_shortslot);
EXPORT_SYMBOL(ieee80211_is_54g);
EXPORT_SYMBOL(ieee80211_wpa_supplicant_ioctl);
EXPORT_SYMBOL(ieee80211_ps_tx_ack);
EXPORT_SYMBOL(ieee80211_start_protocol);
EXPORT_SYMBOL(ieee80211_stop_protocol);
EXPORT_SYMBOL(ieee80211_disassociate);
EXPORT_SYMBOL(ieee80211_start_scan_syncro);
#else
EXPORT_SYMBOL_NOVERS(ieee80211_get_beacon);
EXPORT_SYMBOL_NOVERS(ieee80211_wake_queue);
EXPORT_SYMBOL_NOVERS(ieee80211_stop_queue);
EXPORT_SYMBOL_NOVERS(ieee80211_reset_queue);
EXPORT_SYMBOL_NOVERS(ieee80211_softmac_stop_protocol);
EXPORT_SYMBOL_NOVERS(ieee80211_softmac_start_protocol);
EXPORT_SYMBOL_NOVERS(ieee80211_is_shortslot);
EXPORT_SYMBOL_NOVERS(ieee80211_is_54g);
EXPORT_SYMBOL_NOVERS(ieee80211_wpa_supplicant_ioctl);
EXPORT_SYMBOL_NOVERS(ieee80211_ps_tx_ack);
EXPORT_SYMBOL_NOVERS(ieee80211_start_protocol);
EXPORT_SYMBOL_NOVERS(ieee80211_stop_protocol);
EXPORT_SYMBOL_NOVERS(ieee80211_disassociate);
EXPORT_SYMBOL_NOVERS(ieee80211_start_scan_syncro);
#endif

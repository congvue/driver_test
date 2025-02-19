/******************************************************************************

  Copyright(c) 2003 - 2004 Intel Corporation. All rights reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  The full GNU General Public License is included in this distribution in the
  file called LICENSE.

  Contact Information:
  James P. Ketrenos <ipw2100-admin@linux.intel.com>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

******************************************************************************

  Few modifications for Realtek's Wi-Fi drivers by 
  Andrea Merello <andreamrl@tiscali.it>
  
  A special thanks goes to Realtek for their support ! 

******************************************************************************/

#include <linux/compiler.h>
#include <linux/errno.h>
#include <linux/if_arp.h>
#include <linux/in6.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/pci.h>
#include <linux/proc_fs.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/tcp.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/wireless.h>
#include <linux/etherdevice.h>
#include <asm/uaccess.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18))
#include <linux/config.h>
#endif

#include "ieee80211.h"

/*


802.11 Data Frame


802.11 frame_contorl for data frames - 2 bytes
     ,-----------------------------------------------------------------------------------------.
bits | 0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  a  |  b  |  c  |  d  |  e   |
     |----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|------|
val  | 0  |  0  |  0  |  1  |  x  |  0  |  0  |  0  |  1  |  0  |  x  |  x  |  x  |  x  |  x   |
     |----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|------|
desc | ^-ver-^  |  ^type-^  |  ^-----subtype-----^  | to  |from |more |retry| pwr |more |wep   |
     |          |           | x=0 data,x=1 data+ack | DS  | DS  |frag |     | mgm |data |      |
     '-----------------------------------------------------------------------------------------'
		                                    /\
                                                    |
802.11 Data Frame                                   |
           ,--------- 'ctrl' expands to >-----------'
          |
      ,--'---,-------------------------------------------------------------.
Bytes |  2   |  2   |    6    |    6    |    6    |  2   | 0..2312 |   4  |
      |------|------|---------|---------|---------|------|---------|------|
Desc. | ctrl | dura |  DA/RA  |   TA    |    SA   | Sequ |  Frame  |  fcs |
      |      | tion | (BSSID) |         |         | ence |  data   |      |
      `--------------------------------------------------|         |------'
Total: 28 non-data bytes                                 `----.----'
                                                              |
       .- 'Frame data' expands to <---------------------------'
       |
       V
      ,---------------------------------------------------.
Bytes |  1   |  1   |    1    |    3     |  2   |  0-2304 |
      |------|------|---------|----------|------|---------|
Desc. | SNAP | SNAP | Control |Eth Tunnel| Type | IP      |
      | DSAP | SSAP |         |          |      | Packet  |
      | 0xAA | 0xAA |0x03 (UI)|0x00-00-F8|      |         |
      `-----------------------------------------|         |
Total: 8 non-data bytes                         `----.----'
                                                     |
       .- 'IP Packet' expands, if WEP enabled, to <--'
       |
       V
      ,-----------------------.
Bytes |  4  |   0-2296  |  4  |
      |-----|-----------|-----|
Desc. | IV  | Encrypted | ICV |
      |     | IP Packet |     |
      `-----------------------'
Total: 8 non-data bytes


802.3 Ethernet Data Frame

      ,-----------------------------------------.
Bytes |   6   |   6   |  2   |  Variable |   4  |
      |-------|-------|------|-----------|------|
Desc. | Dest. | Source| Type | IP Packet |  fcs |
      |  MAC  |  MAC  |      |           |      |
      `-----------------------------------------'
Total: 18 non-data bytes

In the event that fragmentation is required, the incoming payload is split into
N parts of size ieee->fts.  The first fragment contains the SNAP header and the
remaining packets are just data.

If encryption is enabled, each fragment payload size is reduced by enough space
to add the prefix and postfix (IV and ICV totalling 8 bytes in the case of WEP)
So if you have 1500 bytes of payload with ieee->fts set to 500 without
encryption it will take 3 frames.  With WEP it will take 4 frames as the
payload of each frame is reduced to 492 bytes.

* SKB visualization
*
*  ,- skb->data
* |
* |    ETHERNET HEADER        ,-<-- PAYLOAD
* |                           |     14 bytes from skb->data
* |  2 bytes for Type --> ,T. |     (sizeof ethhdr)
* |                       | | |
* |,-Dest.--. ,--Src.---. | | |
* |  6 bytes| | 6 bytes | | | |
* v         | |         | | | |
* 0         | v       1 | v | v           2
* 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
*     ^     | ^         | ^ |
*     |     | |         | | |
*     |     | |         | `T' <---- 2 bytes for Type
*     |     | |         |
*     |     | '---SNAP--' <-------- 6 bytes for SNAP
*     |     |
*     `-IV--' <-------------------- 4 bytes for IV (WEP)
*
*      SNAP HEADER
*
*/

static u8 P802_1H_OUI[P80211_OUI_LEN] = { 0x00, 0x00, 0xf8 };
static u8 RFC1042_OUI[P80211_OUI_LEN] = { 0x00, 0x00, 0x00 };

static inline int ieee80211_put_snap(u8 *data, u16 h_proto)
{
	struct ieee80211_snap_hdr *snap;
	u8 *oui;

	snap = (struct ieee80211_snap_hdr *)data;
	snap->dsap = 0xaa;
	snap->ssap = 0xaa;
	snap->ctrl = 0x03;

	if (h_proto == 0x8137 || h_proto == 0x80f3)
		oui = P802_1H_OUI;
	else
		oui = RFC1042_OUI;
	snap->oui[0] = oui[0];
	snap->oui[1] = oui[1];
	snap->oui[2] = oui[2];

	*(u16 *)(data + SNAP_SIZE) = htons(h_proto);

	return SNAP_SIZE + sizeof(u16);
}

int ieee80211_encrypt_fragment(
	struct ieee80211_device *ieee,
	struct sk_buff *frag,
	int hdr_len)
{
	struct ieee80211_crypt_data* crypt = ieee->crypt[ieee->tx_keyidx];
	int res;

#ifdef CONFIG_IEEE80211_CRYPT_TKIP
	struct ieee80211_hdr *header;

	if (ieee->tkip_countermeasures &&
	    crypt && crypt->ops && strcmp(crypt->ops->name, "TKIP") == 0) {
		header = (struct ieee80211_hdr *) frag->data;
		if (net_ratelimit()) {
			printk(KERN_DEBUG "%s: TKIP countermeasures: dropped "
			       "TX packet to " MAC_FMT "\n",
			       ieee->dev->name, MAC_ARG(header->addr1));
		}
		return -1;
	}
#endif
	/* To encrypt, frame format is:
	 * IV (4 bytes), clear payload (including SNAP), ICV (4 bytes) */

	/* Host-based IEEE 802.11 fragmentation for TX is not yet supported, so
	 * call both MSDU and MPDU encryption functions from here. */
	atomic_inc(&crypt->refcnt);
	res = 0;
	if (crypt->ops->encrypt_msdu)
		res = crypt->ops->encrypt_msdu(frag, hdr_len, crypt->priv);
	if (res == 0 && crypt->ops->encrypt_mpdu)
		res = crypt->ops->encrypt_mpdu(frag, hdr_len, crypt->priv);

	atomic_dec(&crypt->refcnt);
	if (res < 0) {
		printk(KERN_INFO "%s: Encryption failed: len=%d.\n",
		       ieee->dev->name, frag->len);
		ieee->ieee_stats.tx_discards++;
		return -1;
	}

	return 0;
}


void ieee80211_txb_free(struct ieee80211_txb *txb, unsigned int nr_frags) {
	int i;
	if (unlikely(!txb))
		return;
	for (i = 0; i < nr_frags; i++)
		if (txb->fragments[i])
			dev_kfree_skb_any(txb->fragments[i]);
	kfree(txb);
}

struct ieee80211_txb *ieee80211_alloc_txb(int nr_frags, int txb_size,
					  int gfp_mask)
{
	struct ieee80211_txb *txb;
	int i;
	txb = kmalloc(
		sizeof(struct ieee80211_txb) + (sizeof(u8*) * nr_frags),
		gfp_mask);
	if (!txb)
		return NULL;

	memset(txb, 0, sizeof(struct ieee80211_txb));

	for (i = 0; i < nr_frags; i++) {
		txb->fragments[i] = dev_alloc_skb(txb_size);
		if (unlikely(!txb->fragments[i])) {
			i--;
			break;
		}
	}
	if (unlikely(i != nr_frags)) {
		while (i >= 0)
			dev_kfree_skb_any(txb->fragments[i--]);
		kfree(txb);
		return NULL;
	}
	return txb;
}

/* SKBs are added to the ieee->tx_queue. */
int ieee80211_xmit(struct sk_buff *skb,
		   struct net_device *dev)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0) 
	struct ieee80211_device *ieee = netdev_priv(dev);
#else
	struct ieee80211_device *ieee = (struct ieee80211_device *)dev->priv;
#endif
	struct ieee80211_txb *txb = NULL;
	struct ieee80211_hdr *frag_hdr;
	int i, bytes_per_frag, nr_frags, bytes_last_frag, frag_size;
	unsigned long flags;
	struct net_device_stats *stats = &ieee->stats;
	int ether_type, encrypt;
	int bytes, fc, hdr_len;
	struct sk_buff *skb_frag;
	struct ieee80211_hdr header = { /* Ensure zero initialized */
		.duration_id = 0,
		.seq_ctl = 0
	};
	u8 dest[ETH_ALEN], src[ETH_ALEN];
	int pend;
            

	struct ieee80211_crypt_data* crypt;
       
	spin_lock_irqsave(&ieee->lock, flags);
        
	/* If there is no driver handler to take the TXB, dont' bother
	 * creating it... */
	if ((!ieee->hard_start_xmit && !(ieee->softmac_features & IEEE_SOFTMAC_TX_QUEUE))||
	   ((!ieee->softmac_data_hard_start_xmit && (ieee->softmac_features & IEEE_SOFTMAC_TX_QUEUE)))) {
		printk(KERN_WARNING "%s: No xmit handler.\n",
		       ieee->dev->name);
		goto success;
	}

	if(likely(ieee->raw_tx == 0)){
	
		if (unlikely(skb->len < SNAP_SIZE + sizeof(u16))) {
			printk(KERN_WARNING "%s: skb too small (%d).\n",
			ieee->dev->name, skb->len);
			goto success;
		}
	
		
		ether_type = ntohs(((struct ethhdr *)skb->data)->h_proto);
                
               	crypt = ieee->crypt[ieee->tx_keyidx];
	
		encrypt = !(ether_type == ETH_P_PAE && ieee->ieee802_1x) &&
			ieee->host_encrypt && crypt && crypt->ops;
	
		if (!encrypt && ieee->ieee802_1x &&
		ieee->drop_unencrypted && ether_type != ETH_P_PAE) {
			stats->tx_dropped++;
			goto success;
		}
	
	#ifdef CONFIG_IEEE80211_DEBUG
		if (crypt && !encrypt && ether_type == ETH_P_PAE) {
			struct eapol *eap = (struct eapol *)(skb->data +
				sizeof(struct ethhdr) - SNAP_SIZE - sizeof(u16));
			IEEE80211_DEBUG_EAP("TX: IEEE 802.11 EAPOL frame: %s\n",
				eap_get_type(eap->type));
		}
	#endif
	
		/* Save source and destination addresses */
		memcpy(&dest, skb->data, ETH_ALEN);
		memcpy(&src, skb->data+ETH_ALEN, ETH_ALEN);
              
                      	
                           
              	/* Advance the SKB to the start of the payload */
		skb_pull(skb, sizeof(struct ethhdr));
	
		/* Determine total amount of storage required for TXB packets */
		bytes = skb->len + SNAP_SIZE + sizeof(u16);
                	
		if (encrypt)
			fc = IEEE80211_FTYPE_DATA | IEEE80211_STYPE_DATA |
				IEEE80211_FCTL_WEP;
		else
			fc = IEEE80211_FTYPE_DATA | IEEE80211_STYPE_DATA;
	
		if (ieee->iw_mode == IW_MODE_INFRA) {
			fc |= IEEE80211_FCTL_TODS;
			/* To DS: Addr1 = BSSID, Addr2 = SA,
			Addr3 = DA */
			memcpy(&header.addr1, ieee->current_network.bssid, ETH_ALEN);
			memcpy(&header.addr2, &src, ETH_ALEN);
			memcpy(&header.addr3, &dest, ETH_ALEN);
		} else if (ieee->iw_mode == IW_MODE_ADHOC) {
			/* not From/To DS: Addr1 = DA, Addr2 = SA,
			Addr3 = BSSID */
			memcpy(&header.addr1, dest, ETH_ALEN);
			memcpy(&header.addr2, src, ETH_ALEN);
			memcpy(&header.addr3, ieee->current_network.bssid, ETH_ALEN);
		}
		header.frame_ctl = cpu_to_le16(fc);
		
		hdr_len = IEEE80211_3ADDR_LEN;
               
	/* Determine fragmentation size based on destination (multicast
		* and broadcast are not fragmented) */
		if (is_multicast_ether_addr(dest) ||
		is_broadcast_ether_addr(dest))
			frag_size = MAX_FRAG_THRESHOLD;
		else
			frag_size = ieee->fts;
	
		/* Determine amount of payload per fragment.  Regardless of if
		* this stack is providing the full 802.11 header, one will
		* eventually be affixed to this fragment -- so we must account for
		* it when determining the amount of payload space. */
		bytes_per_frag = frag_size - IEEE80211_3ADDR_LEN;
		if (ieee->config &
		(CFG_IEEE80211_COMPUTE_FCS | CFG_IEEE80211_RESERVE_FCS))
			bytes_per_frag -= IEEE80211_FCS_LEN;
	
		/* Each fragment may need to have room for encryptiong pre/postfix */
		if (encrypt)
			bytes_per_frag -= crypt->ops->extra_prefix_len +
					crypt->ops->extra_postfix_len;
	
		/* Number of fragments is the total bytes_per_frag /
		* payload_per_fragment */
		nr_frags = bytes / bytes_per_frag;
		bytes_last_frag = bytes % bytes_per_frag;
		if (bytes_last_frag)
			nr_frags++;
		else
			bytes_last_frag = bytes_per_frag;
	
		/* When we allocate the TXB we allocate enough space for the reserve
		* and full fragment bytes (bytes_per_frag doesn't include prefix,
		* postfix, header, FCS, etc.) */
		
		pend = atomic_read(&ieee->tx_pending_txb);
		if(pend > (MAX_TX_SKB - 1)) {
			goto failed;
		} else {
			atomic_inc(&ieee->tx_pending_txb);
		}
		txb = ieee->alloc_txb[ieee->tx_skb_index];
		ieee->tx_skb_index = (ieee->tx_skb_index + 1) % MAX_TX_SKB;
		
		if (unlikely(!txb)) {
			printk(KERN_WARNING "%s: Could not allocate TXB\n",
			ieee->dev->name);
			goto failed;
		}

		for(i = 0; i < txb->nr_frags; i++) {
			txb->fragments[i]->len = 0;
			txb->fragments[i]->data = txb->fragments[i]->head + 16;
			txb->fragments[i]->tail = txb->fragments[i]->data;
		}
			
#if 1		
		txb->nr_frags = nr_frags;
		txb->frag_size = frag_size;
#endif		
		txb->encrypted = encrypt;
		txb->payload_size = bytes;
	
		for (i = 0; i < nr_frags; i++) {
			skb_frag = txb->fragments[i];
	
			if (encrypt)
				skb_reserve(skb_frag, crypt->ops->extra_prefix_len);
	
			frag_hdr = (struct ieee80211_hdr *)skb_put(skb_frag, hdr_len);
			memcpy(frag_hdr, &header, hdr_len);
	
			/* If this is not the last fragment, then add the MOREFRAGS
			* bit to the frame control */
			if (i != nr_frags - 1) {
				frag_hdr->frame_ctl = cpu_to_le16(
					fc | IEEE80211_FCTL_MOREFRAGS);
				bytes = bytes_per_frag;
		
			} else {
				/* The last fragment takes the remaining length */
				bytes = bytes_last_frag;
			}
			
			frag_hdr->seq_ctl = cpu_to_le16(ieee->seq_ctrl<<4 | i);
			
			
			/* Put a SNAP header on the first fragment */
			if (i == 0) {
				ieee80211_put_snap(
					skb_put(skb_frag, SNAP_SIZE + sizeof(u16)),
					ether_type);
				bytes -= SNAP_SIZE + sizeof(u16);
			}
	
			memcpy(skb_put(skb_frag, bytes), skb->data, bytes);
	
			/* Advance the SKB... */
			skb_pull(skb, bytes);
	
			/* Encryption routine will move the header forward in order
			* to insert the IV between the header and the payload */
			if (encrypt)
				ieee80211_encrypt_fragment(ieee, skb_frag, hdr_len);
			if (ieee->config &
			(CFG_IEEE80211_COMPUTE_FCS | CFG_IEEE80211_RESERVE_FCS))
				skb_put(skb_frag, 4);
		}
		if (ieee->seq_ctrl == 0xFFF)
			ieee->seq_ctrl = 0;
		else
			ieee->seq_ctrl++;
	}else{
		if (unlikely(skb->len < sizeof(struct ieee80211_hdr_3addr))) {
			printk(KERN_WARNING "%s: skb too small (%d).\n",
			ieee->dev->name, skb->len);
			goto success;
		}
	
		txb = ieee80211_alloc_txb(1, skb->len, GFP_ATOMIC);
		if(!txb){
			printk(KERN_WARNING "%s: Could not allocate TXB\n",
			ieee->dev->name);
			goto failed;
		}
		
		txb->encrypted = 0;
		txb->payload_size = skb->len;
		memcpy(skb_put(txb->fragments[0],skb->len), skb->data, skb->len);
	}	

 success:
	spin_unlock_irqrestore(&ieee->lock, flags);
	dev_kfree_skb_any(skb);
	if (txb) {
		if (ieee->softmac_features & IEEE_SOFTMAC_TX_QUEUE){
			ieee80211_softmac_xmit(txb, ieee);
		}else{
			if ((*ieee->hard_start_xmit)(txb, dev) == 0) {
				stats->tx_packets++;
				stats->tx_bytes += txb->payload_size;
				return 0;
			}
			ieee80211_txb_free(txb, txb->nr_frags);
		}
	}
	return 0;

 failed:
	spin_unlock_irqrestore(&ieee->lock, flags);
	netif_stop_queue(dev);
	stats->tx_errors++;
	return 1;

}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
EXPORT_SYMBOL(ieee80211_txb_free);
EXPORT_SYMBOL(ieee80211_alloc_txb);
#else
EXPORT_SYMBOL_NOVERS(ieee80211_txb_free);
EXPORT_SYMBOL_NOVERS(ieee80211_alloc_txb);
#endif

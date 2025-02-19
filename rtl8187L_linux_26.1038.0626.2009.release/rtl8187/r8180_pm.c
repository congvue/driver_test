/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Based on the r8180 driver, which is:
 * Copyright 2004-2005 Andrea Merello <andreamrl@tiscali.it>, et al.
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * Jerry chuang <wlanfae@realtek.com>
******************************************************************************/

#ifdef CONFIG_PM

#include "r8180_hw.h"
#include "r8180_pm.h"
#include "r8187.h"
int rtl8180_save_state (struct pci_dev *dev, u32 state)
{
        printk(KERN_NOTICE "r8180 save state call (state %u).\n", state);
	return(-EAGAIN);
}


int rtl8187_suspend (struct usb_interface *intf,pm_message_t state)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
	struct net_device *dev = usb_get_intfdata(intf);
#else
#endif

	if(dev)
	{
		if (!netif_running(dev))
			return 0;
	
		dev->stop(dev);
		netif_device_detach(dev);
		return 0;
	}
	return 0;
}


int rtl8187_resume (struct usb_interface *intf)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
	struct net_device *dev = usb_get_intfdata(intf);
#else
#endif

	if(dev)
	{
		if (!netif_running(dev)){
			printk("! netif_running(dev), go out suspend function\n");
			return 0;
			}
		
		netif_device_attach(dev);
		dev->open(dev);
		return 0;
	}	

	return 0;
}


int rtl8180_enable_wake (struct pci_dev *dev, u32 state, int enable)
{
		
	printk(KERN_NOTICE "r8180 enable wake call (state %u, enable %d).\n", 
	       state, enable);
	return 0;
}

#else

#define rtl8187_suspend NULL
#define rtl8187_suspend NULL

#endif 

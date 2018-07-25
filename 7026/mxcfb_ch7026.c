/*
 * Copyright 2009-2011 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @defgroup Framebuffer Framebuffer Driver for SDC and ADC.
 */

/*!
 * @file mxcfb_epson_vga.c
 *
 * @brief MXC Frame buffer driver for SDC
 *
 * @ingroup Framebuffer
 */

/*!
 * Include files
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/i2c.h>
#include <linux/mxcfb.h>
#include <linux/ipu.h>
#include <linux/fsl_devices.h>
#include <mach/hardware.h>

static struct i2c_client *ch7026_client;

static int lcd_init(void);
static void lcd_poweron(struct fb_info *info);
static void lcd_poweroff(void);

static void (*lcd_reset) (void);
static struct regulator *io_reg;
static struct regulator *core_reg;
static struct regulator *analog_reg;

	/* 8 800x600-60 VESA */
static struct fb_videomode mode = {
	NULL, 60, 800, 600, 25000, 88, 40, 23, 1, 128, 4,
	FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA
};

static void lcd_init_fb(struct fb_info *info)
{
	struct fb_var_screeninfo var;

	memset(&var, 0, sizeof(var));

	fb_videomode_to_var(&var, &mode);

	var.activate = FB_ACTIVATE_ALL;

	console_lock();
	info->flags |= FBINFO_MISC_USEREVENT;
	fb_set_var(info, &var);
	fb_blank(info, FB_BLANK_UNBLANK);
	info->flags &= ~FBINFO_MISC_USEREVENT;
	console_unlock();
}

static int lcd_fb_event(struct notifier_block *nb, unsigned long val, void *v)
{
	struct fb_event *event = v;

	if (strcmp(event->info->fix.id, "DISP3 BG - DI1"))
		return 0;

	switch (val) {
	case FB_EVENT_FB_REGISTERED:
		lcd_init_fb(event->info);
		lcd_poweron(event->info);
		break;
	case FB_EVENT_BLANK:
		if (*((int *)event->data) == FB_BLANK_UNBLANK)
			lcd_poweron(event->info);
		else
			lcd_poweroff();
		break;
	}
	return 0;
}

static struct notifier_block nb = {
	.notifier_call = lcd_fb_event,
};

/*!
 * This function is called whenever the SPI slave device is detected.
 *
 * @param	spi	the SPI slave device
 *
 * @return 	Returns 0 on SUCCESS and error on FAILURE.
 */
static int __devinit lcd_probe(struct device *dev)
{
	int ret = 0;
	int i;

	printk("num = %d\n",num_registered_fb);
	for (i = 0; i < num_registered_fb; i++) {
			printk("fd = %s\n",registered_fb[i]->fix.id);
		if (strcmp(registered_fb[i]->fix.id, "DISP3 BG - DI1") == 0) {
			ret = lcd_init();
			if (ret < 0)
				return  ret;

			lcd_init_fb(registered_fb[i]);
			fb_show_logo(registered_fb[i], 0);
			lcd_poweron(registered_fb[i]);
		}
	}

	fb_register_client(&nb);
	return 0;


	return ret;
}

static int __devinit ch7026_probe(struct i2c_client *client,
				  const struct i2c_device_id *id)
{
	
	
	ch7026_client = client;
	
	printk("\n############### 7026 probe\n");

	return lcd_probe(&client->dev);
}

static int __devexit ch7026_remove(struct i2c_client *client)
{
	fb_unregister_client(&nb);
	lcd_poweroff();
	regulator_put(io_reg);
	regulator_put(core_reg);
	regulator_put(analog_reg);

	return 0;
}

static int ch7026_suspend(struct i2c_client *client, pm_message_t message)
{
	return 0;
}

static int ch7026_resume(struct i2c_client *client)
{
	return 0;
}

u8 reg_init[][2] = {
	{ 0x02, 0x01 },
	{ 0x02, 0x03 },
	{ 0x03, 0x00 },
	{ 0x04, 0x39 },
	{ 0x0A, 0x10 },
	{ 0x0D, 0x83 },
	{ 0x0F, 0x1B },
	{ 0x10, 0x20 },
	{ 0x11, 0xB6 },
	{ 0x12, 0x40 },
	{ 0x13, 0x0A },
	{ 0x14, 0x0A },
	{ 0x15, 0x11 },
	{ 0x16, 0xE0 },
	{ 0x17, 0x08 },
	{ 0x19, 0x0A },
	{ 0x1A, 0x0A },
	{ 0x1D, 0xC0 },
	{ 0x21, 0x12 },
	{ 0x22, 0x40 },
	{ 0x23, 0x71 },
	{ 0x4D, 0x03 },
	{ 0x4E, 0xC5 },
	{ 0x4F, 0x7F },
	{ 0x50, 0x7B },
	{ 0x51, 0x59 },
	{ 0x52, 0x12 },
	{ 0x53, 0x1B },
	{ 0x55, 0xE5 },
	{ 0x5E, 0x80 },
	{ 0x69, 0x64 },
	{ 0x7D, 0x62 },
	{ 0x04, 0x38 },
	{ 0x06, 0x71 },
	
	/*
	NOTE: The following five repeated sentences are used here to wait memory initial complete, please don't remove...(you could refer to Appendix A of programming guide document (CH7025(26)B Programming Guide Rev2.03.pdf or later version) for detailed information about memory initialization! 
	*/
	{ 0x03, 0x00 },
	{ 0x03, 0x00 },
	{ 0x03, 0x00 },
	{ 0x03, 0x00 },
	{ 0x03, 0x00 },
	
	{ 0x06, 0x70 },
	{ 0x02, 0x02 },
	{ 0x02, 0x03 },
	{ 0x04, 0x30 }
};

#define REGMAP_LENGTH (sizeof(reg_init) / (2*sizeof(u8)))

/*
 * Send init commands to L4F00242T03
 *
 */
static int lcd_init(void)
{
	int i;
	int dat;
		printk("\n############### 702dddddddddddddddddd\n");

	dev_dbg(&ch7026_client->dev, "initializing CH7026\n");

	/* read device ID */
	msleep(100);
	dat = i2c_smbus_read_byte_data(ch7026_client, 0x00);
	dev_dbg(&ch7026_client->dev, "read id = 0x%02X\n", dat);
	if (dat != 0x54)
		return -ENODEV;

	for (i = 0; i < REGMAP_LENGTH; ++i) {
		if (i2c_smbus_write_byte_data
		    (ch7026_client, reg_init[i][0], reg_init[i][1]) < 0)
			return -EIO;
	}

	return 0;
}

static int lcd_on;
/*
 * Send Power On commands to L4F00242T03
 *
 */
static void lcd_poweron(struct fb_info *info)
{
	u16 data[4];
	u32 refresh;

	if (lcd_on)
		return;

	dev_dbg(&ch7026_client->dev, "turning on LCD\n");

	data[0] = PICOS2KHZ(info->var.pixclock) / 10;
	data[2] = info->var.hsync_len + info->var.left_margin +
	    info->var.xres + info->var.right_margin;
	data[3] = info->var.vsync_len + info->var.upper_margin +
	    info->var.yres + info->var.lower_margin;

	refresh = data[2] * data[3];
	refresh = (PICOS2KHZ(info->var.pixclock) * 1000) / refresh;
	data[1] = refresh * 100;

	lcd_on = 1;
}

/*
 * Send Power Off commands to L4F00242T03
 *
 */
static void lcd_poweroff(void)
{
	if (!lcd_on)
		return;

	dev_dbg(&ch7026_client->dev, "turning off LCD\n");

	lcd_on = 0;
}

static const struct i2c_device_id ch7026_id[] = {
	{"ch7026", 0},
	{},
};

MODULE_DEVICE_TABLE(i2c, ch7026_id);

static struct i2c_driver ch7026_driver = {
	.driver = {
		   .name = "ch7026",
		   },
	.probe = ch7026_probe,
	.remove = ch7026_remove,
	.suspend = ch7026_suspend,
	.resume = ch7026_resume,
	.id_table = ch7026_id,
};

static int __init ch7026_init(void)
{
	return i2c_add_driver(&ch7026_driver);
}

static void __exit ch7026_exit(void)
{
	i2c_del_driver(&ch7026_driver);
}

module_init(ch7026_init);
module_exit(ch7026_exit);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("CH7026 VGA driver");
MODULE_LICENSE("GPL");

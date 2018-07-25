/* 
 * drivers/input/touchscreen/ft5x0x_ts.c
 *
 * FocalTech ft5x0x TouchScreen driver. 
 *
 * Copyright (c) 2010  Focal tech Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * VERSION      	DATE			AUTHOR
 *    1.0		  2010-01-05			WenFS
 *
 * note: only support mulititouch	Wenfs 2010-10-01
 */

#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include "ft5x06_ts.h"
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/slab.h>
//#include <asm/jzsoc.h>

#include <linux/pm.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/spinlock_types.h>
#include <linux/semaphore.h>
#include <linux/string.h>


#define CONFIG_PM	1
#define DEV_NAME	"ft_device"
struct cdev cdev;
dev_t devno;
struct class *cls;


//static int ft_major = -1;

static struct i2c_client *this_client;
static int g_point_num = 0;
//static struct ft5x0x_ts_platform_data *pdata;

#define	CFG_MAX_TOUCH_POINTS			1
#define CONFIG_FT5X0X_MULTITOUCH 		0
#define	DEBUG_PRINT_NUM				0

//static int g_int_num = 0;

struct ts_event {
	u16	x1;
	u16	y1;
	u16	x2;
	u16	y2;
	u16	x3;
	u16	y3;
	u16	x4;
	u16	y4;
	u16	x5;
	u16	y5;
	u16	pressure;
    u8  touch_point;
};

static struct ts_event old_event = {0,0,0,0,0,0,0,0,0,0,0,0};
static struct ts_event new_event = {0,0,0,0,0,0,0,0,0,0,0,0};

struct ft5x0x_ts_data {
	struct input_dev	*input_dev;
	struct ts_event		event;
	struct work_struct 	pen_event_work;
	struct workqueue_struct *ts_workqueue;

    spinlock_t irq_lock;
    struct semaphore sem;
    int use_irq;
    int irq;
    int rst;
    int irq_is_disable;
    int screen_max_x;
    int screen_max_y;
    int revert_x_flag;
    int revert_y_flag;
    int point_num;
//	struct early_suspend	early_suspend;
};


/***********************************************************************************************
Name	:	ft5x0x_i2c_rxdata 

Input	:	*rxdata
                     *length

Output	:	ret

function	:	

***********************************************************************************************/
static int ft5x0x_i2c_rxdata(char *rxdata, int length)
{
	int ret;

	struct i2c_msg msgs[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= rxdata,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= length,
			.buf	= rxdata,
		},
	};

    //msleep(1);
	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);
	
	return ret;
}
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int ft5x0x_i2c_txdata(char *txdata, int length)
{
	int ret;

	struct i2c_msg msg[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= length,
			.buf	= txdata,
		},
	};

   	//msleep(1);
	ret = i2c_transfer(this_client->adapter, msg, 1);
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}
/***********************************************************************************************
Name	:	 ft5x0x_write_reg

Input	:	addr -- address
                     para -- parameter

Output	:	

function	:	write register of ft5x0x

***********************************************************************************************/
static int ft5x0x_write_reg(u8 addr, u8 para)
{
    u8 buf[3];
    int ret = -1;

    buf[0] = addr;
    buf[1] = para;
    ret = ft5x0x_i2c_txdata(buf, 2);
    if (ret < 0) {
        pr_err("write reg failed! %#x ret: %d", buf[0], ret);
        return -1;
    }
    
    return 0;
}


/***********************************************************************************************
Name	:	ft5x0x_read_reg 

Input	:	addr
                     pdata

Output	:	

function	:	read register of ft5x0x

***********************************************************************************************/
static int ft5x0x_read_reg(u8 addr, u8 *pdata)
{
	int ret;
	u8 buf[2] = {0};

	buf[0] = addr;
	struct i2c_msg msgs[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= buf,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= 1,
			.buf	= buf,
		},
	};

    //msleep(1);
	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);

	*pdata = buf[0];
	return ret;
  
}



void ft5x0x_ts_irq_disable()
{
    unsigned long irqflags;
    struct ft5x0x_ts_data *ft5x0x_ts = i2c_get_clientdata(this_client);

    spin_lock_irqsave(&ft5x0x_ts->irq_lock, irqflags);
    if (!ft5x0x_ts->irq_is_disable)
    {
        ft5x0x_ts->irq_is_disable = 1;
        disable_irq_nosync(ft5x0x_ts->irq);
    }
    spin_unlock_irqrestore(&ft5x0x_ts->irq_lock, irqflags);
 }

void ft5x0x_irq_enable()
{
    unsigned long irqflags = 0;
     struct ft5x0x_ts_data *ft5x0x_ts = i2c_get_clientdata(this_client);

     spin_lock_irqsave(&ft5x0x_ts->irq_lock, irqflags);
     if (ft5x0x_ts->irq_is_disable)
    {
        enable_irq(ft5x0x_ts->irq);
        ft5x0x_ts->irq_is_disable = 0;
    }
    spin_unlock_irqrestore(&ft5x0x_ts->irq_lock, irqflags);
}




#ifdef CONFIG_PM
static int ft5x0x_i2c_suspend(struct device *dev)
{
	printk("==ft5x0x_i2c_suspend=\n");
	struct i2c_client *client = to_i2c_client(dev);
	struct ft5x0x_ts_data *ts = i2c_get_clientdata(client);

	disable_irq_nosync(client->irq);

	cancel_work_sync(&ts->pen_event_work);
	flush_workqueue(ts->ts_workqueue);
	
	ft5x0x_write_reg(FT5X0X_REG_PMODE, PMODE_HIBERNATE);

	return 0;
}

static int ft5x0x_i2c_resume(struct device *dev)
{
	printk("==ft5x0x_ts_resume=\n");
	struct i2c_client *client = to_i2c_client(dev);
	struct ft5x0x_ts_data *ts = i2c_get_clientdata(client);
	struct ft5x0x_ts_platform_data *pdata =
	    (struct ft5x0x_ts_platform_data *)client->dev.platform_data;

	gpio_direction_output(pdata->reset, 0);

	msleep(300);

	gpio_direction_output(pdata->reset, 1);
	msleep(300);
	//ft5x0x_write_reg(GPIO_FT5X0X_WAKE,0);
	//msleep(100);
	//ft5x0x_write_reg(GPIO_FT5X0X_WAKE,1);
	//msleep(100);
	ft5x0x_write_reg(FT5X0X_REG_PMODE,PMODE_ACTIVE);
	
	enable_irq(client->irq);

	return 0;
}
static SIMPLE_DEV_PM_OPS(ft5x0x_i2c_pm, ft5x0x_i2c_suspend, ft5x0x_i2c_resume);
#endif



/***********************************************************************************************
Name	:	 ft5x0x_read_fw_ver

Input	:	 void
                     

Output	:	 firmware version 	

function	:	 read TP firmware version

***********************************************************************************************/
static unsigned char ft5x0x_read_fw_ver(void)
{
	unsigned char ver;
	ft5x0x_read_reg(FT5X0X_REG_FIRMID, &ver);
	return(ver);
}


/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static void ft5x0x_ts_release(void)
{
    //printk("###############%s\n", __func__);
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
#if CONFIG_FT5X0X_MULTITOUCH	
	input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 0); 
#else
	input_report_abs(data->input_dev, ABS_PRESSURE, 0);
	input_report_key(data->input_dev, BTN_TOUCH, 0);
#endif
	input_sync(data->input_dev);
}

static int ft5x0x_read_data(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;
//	u8 buf[14] = {0};
	u8 buf[32] = {0};
	u8 ret = 1;

#if CONFIG_FT5X0X_MULTITOUCH
//	ret = ft5x0x_i2c_rxdata(buf, 13);
	ret = ft5x0x_i2c_rxdata(buf, 31);
#else
    ret = ft5x0x_i2c_rxdata(buf, 7);
#endif
    if (ret < 0)
	{
		printk("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
		return ret;
	}

	/*get the number of the touch_point*/
	memset(event, 0, sizeof(struct ts_event));
//	event->touch_point = buf[2] & 0x03;// 0000 0011
	event->touch_point = buf[2] & 0x07;// 000 0111

//	printk("touch_point=%d, FUNC[%s],LINE[%d]\n",event->touch_point,__func__,__LINE__);

	if (event->touch_point > CFG_MAX_TOUCH_POINTS)
		event->touch_point = CFG_MAX_TOUCH_POINTS;
	
    if (event->touch_point == 0) 
	{
		memset(&old_event, 0, sizeof(struct ts_event));
		memset(&new_event, 0, sizeof(struct ts_event));
        ft5x0x_ts_release();
        return 1; 
    }
  /*ryl for sigle point*/  
    if (event->touch_point > 1)
              event->touch_point = 1;
        printk("event->touch_point=%d\n", event->touch_point);
       if (0 == event->touch_point)
        {
            ft5x0x_ts_release();
            return -1;
          }

#if CONFIG_FT5X0X_MULTITOUCH
    switch (event->touch_point) {
		case 5:
			event->x5 = (s16)(buf[0x1b] & 0x0F)<<8 | (s16)buf[0x1c];
			event->y5 = (s16)(buf[0x1d] & 0x0F)<<8 | (s16)buf[0x1e];
			new_event.x5 = event->x5;
			new_event.y5 = event->y5;
		case 4:
			event->x4 = (s16)(buf[0x15] & 0x0F)<<8 | (s16)buf[0x16];
			event->y4 = (s16)(buf[0x17] & 0x0F)<<8 | (s16)buf[0x18];
			new_event.x4 = event->x4;
			new_event.y4 = event->y4;
		case 3:
			event->x3 = (s16)(buf[0x0f] & 0x0F)<<8 | (s16)buf[0x10];
			event->y3 = (s16)(buf[0x11] & 0x0F)<<8 | (s16)buf[0x12];
			new_event.x3 = event->x3;
			new_event.y3 = event->y3;
		case 2:
			event->x2 = (s16)(buf[9] & 0x0F)<<8 | (s16)buf[10];
			event->y2 = (s16)(buf[11] & 0x0F)<<8 | (s16)buf[12];
			new_event.x2 = event->x2;
			new_event.y2 = event->y2;
		case 1:
			event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
			event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
			new_event.x1 = event->x1;
			new_event.y1 = event->y1;
            break;
		default:
		    return -1;
	}
#else

    if (event->touch_point == 1) 
	{
    	event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
		event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
    }
    
#endif
    event->pressure = 255;

#if CONFIG_FT5X0X_MULTITOUCH
	switch (event->touch_point)
	{
		case 1:
			new_event.x2 = 0;
			new_event.y2 = 0;
		case 2:
			new_event.x3 = 0;
			new_event.y3 = 0;
		case 3:
			new_event.x4 = 0;
			new_event.y4 = 0;
		case 4:
			new_event.x5 = 0;
			new_event.y5 = 0;
		default :
			break;
	}
	new_event.pressure = event->pressure;
	new_event.touch_point = event->touch_point;

	dev_dbg(&this_client->dev, "%s: 1:%d %d 2:%d %d \n", __func__,
		event->x1, event->y1, event->x2, event->y2);
	//printk("%d (%d, %d), (%d, %d)\n", event->touch_point, event->x1, event->y1, event->x2, event->y2);
#endif

    return 0;
}
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static void ft5x0x_report_value(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;
    u8 buf[32] = {0};
	u8 ret = -1;
    u8 uVersion;

//#if CONFIG_FT5X0X_MULTITOUCH  //ryl
//	ret = ft5x0x_i2c_rxdata(buf, 13);
	ret = ft5x0x_i2c_rxdata(buf, 31);

//#else
 //   ret = ft5x0x_i2c_rxdata(buf, 7);
//#endif
    if (ret < 0)
	{
		printk("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
		return ret;
	}

	/*get the number of the touch_point*/
	memset(event, 0, sizeof(struct ts_event));
//	event->touch_point = buf[2] & 0x03;// 0000 0011
	event->touch_point = buf[2] & 0x07;// 000 0111

if (event->touch_point > CFG_MAX_TOUCH_POINTS)
		event->touch_point = CFG_MAX_TOUCH_POINTS;
/*	
    if (event->touch_point == 0) 
	{
		memset(&old_event, 0, sizeof(struct ts_event));
		memset(&new_event, 0, sizeof(struct ts_event));
        ft5x0x_ts_release();
        return 0; 
    }
*/
  /*ryl for sigle point*/  
    if (event->touch_point > 1)
              event->touch_point = 1;
    if (0 == event->touch_point && g_point_num != 0)
    {
		g_point_num = event->touch_point;
        ft5x0x_ts_release();
        return 0;
      }


     

//		printk("==ft5x0x_report_value =\n");
#if CONFIG_FT5X0X_MULTITOUCH
    switch (event->touch_point) {
            case 5:
                event->x5 = (s16)(buf[0x1b] & 0x0F)<<8 | (s16)buf[0x1c];
                event->y5 = (s16)(buf[0x1d] & 0x0F)<<8 | (s16)buf[0x1e];
                new_event.x5 = event->x5;
                new_event.y5 = event->y5;
            case 4:
                event->x4 = (s16)(buf[0x15] & 0x0F)<<8 | (s16)buf[0x16];
                event->y4 = (s16)(buf[0x17] & 0x0F)<<8 | (s16)buf[0x18];
                new_event.x4 = event->x4;
                new_event.y4 = event->y4;
            case 3:
                event->x3 = (s16)(buf[0x0f] & 0x0F)<<8 | (s16)buf[0x10];
                event->y3 = (s16)(buf[0x11] & 0x0F)<<8 | (s16)buf[0x12];
                new_event.x3 = event->x3;
                new_event.y3 = event->y3;
            case 2:
                event->x2 = (s16)(buf[9] & 0x0F)<<8 | (s16)buf[10];
                event->y2 = (s16)(buf[11] & 0x0F)<<8 | (s16)buf[12];
                new_event.x2 = event->x2;
                new_event.y2 = event->y2;
            case 1:
                event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
                event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
                new_event.x1 = event->x1;
                new_event.y1 = event->y1;
                break;
            default:
                new_event.x1 = 0;
			new_event.y1 = 0;
                new_event.x2 = 0;
			new_event.y2 = 0;
                new_event.x3 = 0;
			new_event.y3 = 0;
                new_event.x4 = 0;
			new_event.y4 = 0;
               new_event.x5 = 0;
			new_event.y5 = 0;
        }
         event->pressure = 255;

	switch(event->touch_point) {
		case 5: 
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x5);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y5);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_report_key(data->input_dev, BTN_TOUCH, 1);
			input_mt_sync(data->input_dev);
			printk("===x5 = %d,y5 = %d ====\n",event->x5,event->y5);
		case 4:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x4);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y4);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_report_key(data->input_dev, BTN_TOUCH, 1);
			input_mt_sync(data->input_dev);
			printk("===x4 = %d,y4 = %d ====\n",event->x4,event->y4);
		case 3:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x3);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y3);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_report_key(data->input_dev, BTN_TOUCH, 1);
			input_mt_sync(data->input_dev);
			printk("===x3 = %d,y3 = %d ====\n",event->x3,event->y3);
		case 2:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x2);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y2);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_report_key(data->input_dev, BTN_TOUCH, 1);
			input_mt_sync(data->input_dev);
			printk("===x2 = %d,y2 = %d ====\n",event->x2,event->y2);
		case 1:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x1);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y1);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_report_key(data->input_dev, BTN_TOUCH, 1);
			input_mt_sync(data->input_dev);
			printk("===x1 = %d,y1 = %d ====\n",event->x1,event->y1);
		default:
//			printk("==touch_point default =\n");
			break;
	}
#else	/* CONFIG_FT5X0X_MULTITOUCH*/

    if (event->touch_point == 1) 
	{
    	event->x1 = (s16)(buf[3] & 0x0F)<<8 | (s16)buf[4];
		event->y1 = (s16)(buf[5] & 0x0F)<<8 | (s16)buf[6];
    }
    event->pressure = 255;

#if	XY_REVERSE
	event->x1 = SCREEN_MAX_X - event->x1 -1;
	event->y1 = SCREEN_MAX_Y - event->y1 -1;
#endif
    
	if (event->touch_point == 1)
	{
		input_report_abs(data->input_dev, ABS_X, event->x1);
		input_report_abs(data->input_dev, ABS_Y, event->y1);
		input_report_abs(data->input_dev, ABS_PRESSURE, event->pressure);
		input_report_key(data->input_dev, BTN_TOUCH, 1);
	}
	//printk("report TOUCH_KEY FUN[%s] LINE[%d]\n", __func__, __LINE__);
	
   // printk("===x = %d,y = %d ====\n",event->x1,event->y1);
	g_point_num = event->touch_point;
	
#endif	/* CONFIG_FT5X0X_MULTITOUCH*/
 
#if CONFIG_FT5X0X_MULTITOUCH			
	old_event.x1 = event->x1;
	old_event.y1 = event->y1;
	old_event.x2 = event->x2;
	old_event.y2 = event->y2;	
	old_event.x3 = event->x3;
	old_event.y3 = event->y3;	
	old_event.x4 = event->x4;
	old_event.y4 = event->y4;
	old_event.x5 = event->x5;	
	old_event.y5 = event->y5;	
	old_event.pressure = event->pressure;
	old_event.touch_point = event->touch_point;

    dev_dbg(&this_client->dev, "%s: 1:%d %d 2:%d %d \n", __func__,
		event->x1, event->y1, event->x2, event->y2);
#endif	
	input_sync(data->input_dev);

	
}	/*end ft5x0x_report_value*/
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/

static int ts_compare()
{
	return ((new_event.x1==old_event.x1)&&(new_event.y1==old_event.y1)
		&&(new_event.x2==old_event.x2)&&(new_event.y2==old_event.y2)
		&&(new_event.x3==old_event.x3)&&(new_event.y3==old_event.y3)
		&&(new_event.x4==old_event.x4)&&(new_event.y4==old_event.y4)
		&&(new_event.x5==old_event.x5)&&(new_event.y5==old_event.y5)
		&&(new_event.touch_point==old_event.touch_point)
		&&(new_event.pressure==old_event.pressure)) ? 0 : 1;
}

static void ft5x0x_ts_pen_irq_work(struct work_struct *work)
{
    struct ft5x0x_ts_data *ft5x0x_ts = i2c_get_clientdata(this_client);
	if (DEBUG_PRINT_NUM)
		printk("FUN[%s]  LINE[%d]  irq=%d\n", __func__, __LINE__,this_client->irq); 
	int ret = -1;
	//ret = ft5x0x_read_data();		//read data
	if (DEBUG_PRINT_NUM)
		printk("ret = %d, FUN[%s]====LINE[%d]\n",ret, __func__, __LINE__);
	//if (0==ret) 
	//{	
		ft5x0x_report_value();		//read success then report
	//}
	//else ;//printk("data package read error\n");

    if (ft5x0x_ts->use_irq)
        ft5x0x_irq_enable();
}
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static irqreturn_t ft5x0x_ts_interrupt(int irq, void *dev_id)
{

   
	if (DEBUG_PRINT_NUM)
		printk("ft5x0x_ts interrupt FUNC[%s] LINE[%d] \n", __func__, __LINE__);
	struct ft5x0x_ts_data *ft5x0x_ts = (struct ft5x0x_ts_data *)dev_id;

    if (ft5x0x_ts->use_irq)
           ft5x0x_ts_irq_disable();

	if (!work_pending(&ft5x0x_ts->pen_event_work)) {
		queue_work(ft5x0x_ts->ts_workqueue, &ft5x0x_ts->pen_event_work);
		if (DEBUG_PRINT_NUM)
			printk("work_pending success\n");
	}
	if (DEBUG_PRINT_NUM)
		printk("will leave FUNC[%s] right now\n", __func__);

	return IRQ_HANDLED;
}

void resume(void)
{
	struct ft5x0x_ts_data *ts = i2c_get_clientdata(this_client);
	struct ft5x0x_ts_platform_data *pdata =
	    (struct ft5x0x_ts_platform_data *)this_client->dev.platform_data;

	gpio_direction_output(pdata->reset, 0);

	msleep(300);

	gpio_direction_output(pdata->reset, 1);
	msleep(300);
}

static ssize_t ft_write(struct file *fl, const char __user *buf, size_t st, loff_t *lt)
{
	int n_A5h = simple_strtol(buf, NULL, 0);
	printk("========n_A5h=%d =========\n", n_A5h);
	switch(n_A5h)
	{
		case 0:
			printk("==PMODE_ACTIVE =\n");
			resume();
			ft5x0x_write_reg(FT5X0X_REG_PMODE, PMODE_ACTIVE);
			
			enable_irq(this_client->irq);
			break;
		case 1:
			printk("==PMODE_MONITOR =\n");
			resume();
			ft5x0x_write_reg(FT5X0X_REG_PMODE, PMODE_MONITOR);
			enable_irq(this_client->irq);
			break;
		case 3:
			printk("==PMODE_HIBERNATE =\n");
			struct ft5x0x_ts_data *ts = i2c_get_clientdata(this_client);

			disable_irq_nosync(this_client->irq);

			cancel_work_sync(&ts->pen_event_work);
			flush_workqueue(ts->ts_workqueue);
			ft5x0x_write_reg(FT5X0X_REG_PMODE, PMODE_HIBERNATE);
			break;
		default :
			break;
	}
	
	return 0;
		
}

static const struct file_operations ft_fops = {
	.owner		= THIS_MODULE,
	.write		= ft_write,
};


/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int  ft5x0x_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("################  -------   ################\n");
	printk("################ |   @   |  ################\n");
	printk("################ --------   ################\n");
	struct ft5x0x_ts_platform_data *pdata =
	    (struct ft5x0x_ts_platform_data *)client->dev.platform_data;
	struct ft5x0x_ts_data *ft5x0x_ts;
	struct input_dev *input_dev;
	int err = 0;
	unsigned char uc_reg_value; 
	
	//printk("==ft5x0x_ts_probe=\n");
	
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	//printk("==kzalloc=\n");
	ft5x0x_ts = kzalloc(sizeof(*ft5x0x_ts), GFP_KERNEL);
	if (!ft5x0x_ts)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}
    memset(ft5x0x_ts, 0, sizeof(*ft5x0x_ts));

	this_client = client;
    this_client->irq = pdata->irq;
	i2c_set_clientdata(client, ft5x0x_ts);
	spin_lock_init(&ft5x0x_ts->irq_lock);
    ft5x0x_ts->screen_max_x = SCREEN_MAX_X;
    ft5x0x_ts->screen_max_y= SCREEN_MAX_Y;
    ft5x0x_ts->irq = pdata->irq;
    ft5x0x_ts->rst = pdata->reset;

	ft5x0x_ts->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!ft5x0x_ts->ts_workqueue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}
	

	

	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}
	
	ft5x0x_ts->input_dev = input_dev;
   

ft5x0x_ts->input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
 ft5x0x_ts->input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) |
      BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
 
#if CONFIG_FT5X0X_MULTITOUCH
	set_bit(ABS_MT_TOUCH_MAJOR, ft5x0x_ts->input_dev->absbit); 
	set_bit(ABS_MT_POSITION_X, ft5x0x_ts->input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, ft5x0x_ts->input_dev->absbit);
	set_bit(ABS_MT_WIDTH_MAJOR, ft5x0x_ts->input_dev->absbit);

	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_TOUCH_MAJOR, 0, PRESS_MAX, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_TRACKING_ID, 0, CFG_MAX_TOUCH_POINTS, 0, 0);
#else

	set_bit(ABS_X, ft5x0x_ts->input_dev->absbit);
	set_bit(ABS_Y, ft5x0x_ts->input_dev->absbit);
	set_bit(ABS_PRESSURE, ft5x0x_ts->input_dev->absbit);
	set_bit(BTN_TOUCH, ft5x0x_ts->input_dev->keybit); 

	input_set_abs_params( ft5x0x_ts->input_dev, ABS_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params( ft5x0x_ts->input_dev, ABS_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params( ft5x0x_ts->input_dev,
			     ABS_PRESSURE, 0, 255, 0 , 0);

#endif

	//set_bit(EV_ABS, ft5x0x_ts->input_dev->evbit);
	//set_bit(EV_KEY, ft5x0x_ts->input_dev->evbit);
	set_bit(EV_ABS, ft5x0x_ts->input_dev->absbit);
	set_bit(EV_KEY, ft5x0x_ts->input_dev->absbit);

	ft5x0x_ts->input_dev->name		= FT5X0X_NAME;		//dev_name(&client->dev)
	err = input_register_device(ft5x0x_ts->input_dev);
	if (err) {
		dev_err(&client->dev,
		"ft5x0x_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}

	//printk("irq_Num = %d \n", pdata->irq);  
	err = request_irq(pdata->irq, ft5x0x_ts_interrupt,  IRQF_TRIGGER_FALLING, "ft5x0x_ts", ft5x0x_ts);
	if (err < 0) {
        ft5x0x_ts->use_irq = 0;
		dev_err(&client->dev, "ft5x0x_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}
    else
    {
        ft5x0x_ts_irq_disable();
        ft5x0x_ts->use_irq = 1;
       }
	
	
	 INIT_WORK(&ft5x0x_ts->pen_event_work, ft5x0x_ts_pen_irq_work);

    msleep(50);
  
    if (ft5x0x_ts->use_irq)
        ft5x0x_irq_enable();

	int ret = 0;
	ret = alloc_chrdev_region(&devno, 0, 1, DEV_NAME);
	if (ret)
		return ret;

	cdev_init(&cdev, &ft_fops);
	cdev.owner = THIS_MODULE;

	ret = cdev_add(&cdev, devno, 1);
	if(ret) 
	{
		unregister_chrdev_region(devno, 1);
		return ret;
	}

	cls =class_create(THIS_MODULE, "ft_class");
	if (IS_ERR(cls))	
	{
		cdev_del(&cdev);
		unregister_chrdev_region(devno, 1);
		return PTR_ERR(cls);	
	}
	struct device *dev;
	dev = device_create(cls, NULL, devno, NULL, "ft_device%d", 0);

	if (IS_ERR(dev))	
	{
		class_destroy(cls);
		cdev_del(&cdev);
		unregister_chrdev_region(devno, 1);
		return PTR_ERR(dev);	
	}

//	printk("==probe over =\n");
    return 0;
exit_irq_request_failed:
exit_input_register_device_failed:
	input_free_device(input_dev);
exit_input_dev_alloc_failed:
//	free_irq(client->irq, ft5x0x_ts);
	free_irq(client->irq, ft5x0x_ts);
exit_platform_data_null:
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
	destroy_workqueue(ft5x0x_ts->ts_workqueue);
exit_create_singlethread:
	printk("==singlethread error =\n");
	i2c_set_clientdata(client, NULL);
	kfree(ft5x0x_ts);
exit_alloc_data_failed:
exit_check_functionality_failed:
	return err;
}




/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int __devexit ft5x0x_ts_remove(struct i2c_client *client)
{
	//printk("==ft5x0x_ts_remove=\n");

	//if (ft_major >= 0) {
	//	unregister_chrdev(ft_major, "ft_device");
	//	ft_major = -1;
	//}
	device_destroy(cls, devno);
	class_destroy(cls);	
	cdev_del(&cdev);
	unregister_chrdev_region(devno, 1);
	
	
	struct ft5x0x_ts_data *ft5x0x_ts = i2c_get_clientdata(client);
//	free_irq(client->irq, ft5x0x_ts);
	free_irq(client->irq, ft5x0x_ts);
	input_unregister_device(ft5x0x_ts->input_dev);
	kfree(ft5x0x_ts);
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
	destroy_workqueue(ft5x0x_ts->ts_workqueue);
	i2c_set_clientdata(client, NULL);
	return 0;
}

static const struct i2c_device_id ft5x0x_ts_id[] = {
	{ FT5X0X_NAME, 0 },{ }
};


MODULE_DEVICE_TABLE(i2c, ft5x0x_ts_id);

static struct i2c_driver ft5x0x_ts_driver = {
	.probe		= ft5x0x_ts_probe,
	.remove		= __devexit_p(ft5x0x_ts_remove),
	.id_table	= ft5x0x_ts_id,
	.driver	= {
		.name	= FT5X0X_NAME,
		.owner	= THIS_MODULE,
#ifdef CONFIG_PM
		.pm = &ft5x0x_i2c_pm,
#endif

	},
};


/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int __init ft5x0x_ts_init(void)
{
	int ret;
	//printk("==ft5x0x_ts_init==\n");
	ret = i2c_add_driver(&ft5x0x_ts_driver);
	printk("ret=%d\n",ret);
	if (ret) {
		printk(KERN_WARNING "Adding ft5x0x driver failed "
		       "(errno = %d)\n", ret);
	} else {
		pr_info("Successfully added driver %s\n",
			ft5x0x_ts_driver.driver.name);
	}
	//i2c_register_char_device();
	return ret;
//	return i2c_add_driver(&ft5x0x_ts_driver);
}

/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static void __exit ft5x0x_ts_exit(void)
{
	printk("==ft5x0x_ts_exit==\n");
	i2c_del_driver(&ft5x0x_ts_driver);
}

module_init(ft5x0x_ts_init);
module_exit(ft5x0x_ts_exit);

MODULE_AUTHOR("<wenfs@Focaltech-systems.com>");
MODULE_DESCRIPTION("FocalTech ft5x0x TouchScreen driver");
MODULE_LICENSE("GPL");


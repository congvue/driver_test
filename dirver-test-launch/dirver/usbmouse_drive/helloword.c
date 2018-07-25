#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>


static struct input_dev *uk_dev;
static char *usb_buf;
static dma_addr_t usb_buf_phys;
static int len;
static struct urb *uk_urb;

void print_hex(void *data, int len)
{
	int i;
	for(i=0; i<len;i++){
		printk("0x%2x",*(unsigned char*)data);
		data++;
	}
	printk("\n");
}

static struct usb_device_id usbmouse_as_key_id_table[] = {
		{USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,USB_INTERFACE_PROTOCOL_MOUSE)},
			{ }
};

static void usbmouse_as_key_irq(struct urb *urb)
{
	static unsigned char pre_val;

	print_hex(usb_buf, len);

	if ((pre_val & (1<<0)) != (usb_buf[0] & (1<<0)))
	{
		/* 左键发生了变化 */
		input_event(uk_dev, EV_KEY, BTN_LEFT, (usb_buf[0] & (1<<0)) ? 1 : 0);
		printk("kaaaaaaaaaaaaaaaaaaaaaaaaa\n");
		input_sync(uk_dev);
	}

	if((pre_val & (1 << 1)) != (usb_buf[0] & (1 << 1))){
		input_event(uk_dev, EV_KEY, BTN_RIGHT, (usb_buf[0] & (1 << 1)) ? 1 : 0);
		printk("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
		input_sync(uk_dev);
	}

	if((pre_val & (1 << 2)) != (usb_buf[0] & (1 << 2))){
		input_event(uk_dev, EV_KEY, BTN_MIDDLE, (usb_buf[0] & (1 << 2)) ? 1 : 0);
		printk("ccccccccccccccccccccccccc\n");			
		input_sync(uk_dev);
	}
	pre_val = usb_buf[0];

		/* 重新提交urb */

	usb_submit_urb(uk_urb, GFP_KERNEL);
		


}

static int usbmouse_as_key_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	struct usb_host_interface *interface;
	struct usb_endpoint_descriptor *endpoint;

	int pipe;

	interface = intf->cur_altsetting;
	endpoint  = & interface->endpoint[0].desc;

		/* a. 分配一个input_dev */

	uk_dev = input_allocate_device();
		/* b. 设置 */
		/* b.1 能产生哪类事件 */

	set_bit(EV_KEY, uk_dev->evbit);
	set_bit(EV_REP, uk_dev->evbit);

	/* b.2 能产生哪些事件 */

	set_bit(BTN_LEFT, uk_dev->keybit);
	set_bit(BTN_RIGHT, uk_dev->keybit);
	set_bit(BTN_MIDDLE, uk_dev->keybit);

	uk_dev->name = "usb_mouse_test_input";
	uk_dev->phys = "mouse_button";
	uk_dev->id.bustype = BUS_HOST;
	uk_dev->id.vendor = 0x0001;
	uk_dev->id.product = 0x0001;
	uk_dev->id.version = 0x0001;
	

/* c. 注册 */
	input_register_device(uk_dev);

/* d. 硬件相关操作 */
/* 数据传输3要素: 源,目的,长度 */
/* 源: USB设备的某个端点 */

	pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
	printk("0x%2x, 0x%2x\n",dev->devnum, endpoint->bEndpointAddress);//0x03  ; 0x81
	printk("0x%2x\n",usb_pipeendpoint(pipe));//0x01
	printk("0x%2x\n",usb_pipedevice(pipe));	//0x11
	printk("0x%2x\n",usb_pipetype(pipe));	//0x01
	printk("0x%2x\n",usb_pipein(pipe));		//0x80
	printk("0x%2x\n",usb_pipeout(pipe));	//0x00
	

/* 长度: */
	len = endpoint->wMaxPacketSize;

		/* 目的: */

	usb_buf = usb_alloc_coherent(dev, len, GFP_ATOMIC, &usb_buf_phys);

	memset(usb_buf, 0, len);
	printk("len = %d\n",len);

/* 使用"3要素" */
/* 分配usb request block */

	uk_urb = usb_alloc_urb(0, GFP_KERNEL);

/* 使用"3要素设置urb" */

	usb_fill_int_urb(uk_urb, dev, pipe, usb_buf, len, usbmouse_as_key_irq, NULL, endpoint->bInterval);

	uk_urb->transfer_dma = usb_buf_phys;
	uk_urb->transfer_flags = URB_NO_TRANSFER_DMA_MAP;

/* 使用URB */
	usb_submit_urb(uk_urb, GFP_KERNEL);

	return 0;
}


static void usbmouse_as_key_disconnect(struct usb_interface *intf)
{
	struct usb_device *dev = interface_to_usbdev(intf);

	usb_kill_urb(uk_urb);
	usb_free_urb(uk_urb);

	usb_free_coherent(dev, len, usb_buf, usb_buf_phys);

	input_unregister_device(uk_dev);
	input_free_device(uk_dev);

}

static struct usb_driver usbmouse_as_key_driver = {
	.name = "usbmouse_as_key",
	.probe  = usbmouse_as_key_probe,
	.disconnect = usbmouse_as_key_disconnect,

	.id_table = usbmouse_as_key_id_table,
};

static int usbmouse_as_key_init(void)
{
	usb_register(&usbmouse_as_key_driver);
	return 0;

}

static void usbmouse_as_key_exit(void)
{
	usb_deregister(&usbmouse_as_key_driver);

}


module_init(usbmouse_as_key_init);
module_exit(usbmouse_as_key_exit);

MODULE_LICENSE("GPL");

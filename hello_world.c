#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>

MODULE_LICENSE("Dual BSD/GPL");

#define STATUS_SUCCESS (0)
#define PS5_CTRL_PRODUCT_ID ()
#define PS5_CTRL_VENDOR_ID ()

/*
struct usb_driver {
  const char * name;
  int (* probe) (struct usb_interface *intf,const struct usb_device_id *id);
  void (* disconnect) (struct usb_interface *intf);
  int (* ioctl) (struct usb_interface *intf, unsigned int code,void *buf);
  int (* suspend) (struct usb_interface *intf, pm_message_t message);
  int (* resume) (struct usb_interface *intf);
  int (* reset_resume) (struct usb_interface *intf);
  int (* pre_reset) (struct usb_interface *intf);
  int (* post_reset) (struct usb_interface *intf);
  const struct usb_device_id * id_table;
  struct usb_dynids dynids;
  struct usbdrv_wrap drvwrap;
  unsigned int no_dynamic_id:1;
  unsigned int supports_autosuspend:1;
  unsigned int soft_unbind:1;
}; */

static int ps5_ctrl_probe(struct usb_interface* intf, unsigned int code, void* buf);

static struct usb_driver ps5_ctrl_driver = {
    .name = "PS5 controller driver",
    .probe = ps5_ctrl_probe
};

/* table of devices that work with this driver */
static struct usb_device_id ps5_ctrl_table [] = {
        { USB_DEVICE(PS5_CTRL_VENDOR_ID, PS5_CTRL_PRODUCT_ID) },
        { }                      /* Terminating entry */
};


static int __init hello_init(void)
{
    int res = 0;
    printk(KERN_ALERT "Module init\n");
    res = usb_register(&ps5_ctrl_driver);
    if (res != STATUS_SUCCESS) {
        printk("usb_register() failed with code %d\n", res);
        return res;
    }
    //and don't forget MODULE_DEVICE_TABLE(usb, ...)
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_ALERT "Module exit\n");
    usb_deregister(&ps5_ctrl_driver);
}

static int ps5_ctrl_probe(struct usb_interface* intf, unsigned int code, void* buf) {
    printk("probe activated!\n");
}

module_init(hello_init);
module_exit(hello_exit);
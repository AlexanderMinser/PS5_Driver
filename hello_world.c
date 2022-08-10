//includes
#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/list.h>
#include <linux/slab.h>

//defines
#define PS5_CTRL_PRODUCT_ID (0x054C) //Sony ID
#define PS5_CTRL_VENDOR_ID (0x0CE6) //PS5 controller


//declarations
static int ps5_ctrl_probe(struct usb_interface* intf,const struct usb_device_id *id);


//globals
static struct usb_driver ps5_ctrl_driver = {
    .name = "PS5 controller driver",
    .probe = ps5_ctrl_probe
};

/* table of devices that work with this driver */
static struct usb_device_id ps5_ctrl_table [] = {
        { USB_DEVICE(PS5_CTRL_VENDOR_ID, PS5_CTRL_PRODUCT_ID) },
        { }                      /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, ps5_ctrl_table);

static LIST_HEAD(controllers);
//TODO: protect w/ mutex

//procedures
static int __init ps5_ctrl_init(void)
{
    int res = 0;

    INIT_LIST_HEAD(&controllers);
    printk(KERN_ALERT "Module init\n");
    res = usb_register(&ps5_ctrl_driver);
    if (res) {
        printk("usb_register() failed with code %d\n", res);
        return res;
    }
    return 0;
}

static void __exit ps5_ctrl_exit(void)
{
    dev_t curr_dev;

    printk(KERN_ALERT "Module exit\n");
    usb_deregister(&ps5_ctrl_driver);
    list_for_each(&curr_dev, controllers) {
        //TODO: feel like this isn't the right way to do this
        unregister_chrdev_region(curr_dev, 1);
    }
}

static int ps5_ctrl_probe(struct usb_interface* intf,const struct usb_device_id *id) {
    dev_t new_dev;
    int ret = 0;
    struct list_head* new_entry = NULL;
    
    printk(KERN_ALERT "probe activated!\n");
    ret = alloc_chrdev_region(&new_dev, 0, 1, "ps5_ctrl");
    if (ret) {
        printk(KENR_ALERT "Failed to alloc ps5 ctrl!");
        return ret;
    }
    new_entry = kmalloc(sizeof(struct list_head), GFP_KERNEL);
    list_add(new_entry, controllers);
    return 0;
}

//run
module_init(ps5_ctrl_init);
module_exit(ps5_ctrl_exit);

MODULE_AUTHOR("Alexander Minser <aminser1996@gmail.com>")
MODULE_LICENSE("Dual BSD/GPL");
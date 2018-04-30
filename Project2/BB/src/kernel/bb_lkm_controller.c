/*
 * bb_lkm_controller.c
 * Device driver for representing SNES controller value (from TIVA) for gamepad
 * in a Linux application.
 * 
 * Author: Connor Shapiro
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define SUCCESS         (0)

MODULE_LICENSE("GPL");

static uint16_t controller_state;

static int dev_open (struct inode * p_inode, struct file * p_file);
static int dev_release (struct inode * p_inode, struct file * p_file);
static ssize_t dev_read (struct file *p_file, char *p_buffer, size_t length, loff_t *offset);
static ssize_t dev_write (struct file *p_file, const char *p_buffer, size_t length, loff_t *offset);

static struct file_operations driver_fops =
{
    .open = dev_open,
    .write = dev_write,
    .release = dev_release,
    .read = dev_read,
};

static int major_number;
static struct class* snescontroller_class;
static struct device* snescontroller_device;

static int dev_open(struct inode *p_inode, struct file *p_file)
{
    controller_state = 0;
    printk(KERN_INFO "Successfully opened SNES controller driver.\n");
    return 0;
}

static int dev_release(struct inode *p_inode, struct file *p_file)
{
    printk(KERN_INFO "Successfully released SNES controller driver.\n");
    return 0;
}

static ssize_t dev_read(struct file *p_file, char *p_buffer, size_t length, loff_t *offset)
{
    uint8_t buf[2];
    buf[0] = (uint8_t)(controller_state >> 8);
    buf[1] = (uint8_t)(controller_state & 0xFF);
    if (SUCCESS == copy_to_user(p_buffer, buf, sizeof(buf)))
    {
        printk(KERN_INFO "Successfully sent SNES controller value to the requesting application.\n");
        return SUCCESS;
    }
    else
    {
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *p_file, const char *p_buffer, size_t length, loff_t *offset)
{
    uint8_t buf[2];
    if (SUCCESS == copy_from_user(buf, p_buffer, 2))
    {
        controller_state = *(uint16_t *)buf;
        printk(KERN_INFO "Successfully updated SNES controller value in the driver.\n");
        return sizeof(buf);
    }
    else 
    {
        return -EFAULT;
    }
}

static int __init snescontroller_init(void)
{
    /* Register driver major number */;
    major_number = register_chrdev(0, "controller", &driver_fops);

    /* Register driver class */
    snescontroller_class = class_create(THIS_MODULE, "snescontroller");
    
    /* Register driver device */
    snescontroller_device = device_create(snescontroller_class, NULL, MKDEV(major_number, 0), NULL, "controller");

    printk(KERN_INFO "Successful init of SNES controller driver.\n");
    return SUCCESS;
}

static void __exit snescontroller_exit(void)
{
    device_destroy(snescontroller_class, MKDEV(major_number, 0));
    class_unregister(snescontroller_class);
    class_destroy(snescontroller_class);
    unregister_chrdev(major_number, "controller");
    printk(KERN_INFO "Succsesful exit of SNES controller driver.\n");
}

module_init(snescontroller_init);
module_exit(snescontroller_exit);
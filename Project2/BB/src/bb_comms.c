/*
 * bb_comms.c
 * Beaglebone UART/SPI Communications kernel module
 *      for interfacing with the TIVA TM4C1294
 * 
 */

#include <linux/init.h>
#include <linux/module.h>
// #include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "../../inc/API.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Connor Shapiro");
MODULE_DESCRIPTION("Communications Module for BeagleBone to TIVA interface.");
MODULE_VERSION("WP1.0");

static int __init bb_comms_init(void)
{
    struct file * f;
    if ((f = filp_open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY, 0)) < 0)
    {
        printk("UART: Failed to open the file.\n");
        return -1;
    }
    printk("UART: Opened file.\n");
    return 0;
}

static void __exit bb_comms_exit(void)
{

}

module_init(bb_comms_init);
module_exit(bb_comms_exit);
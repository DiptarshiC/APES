/**
*@name:my_module_input.c 
*
*@description: this fuction allows the user to pass parameters  
*		via the command line
*
*
*
*
*/




#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>




MODULE_LICENSE("GPL");

static 	int count_time = 500;

static char *name = "Diptarshi";



/*Now we are actually making the mechanism up -- making the variables command
* line arguments rather than just a bunch of global variables
*/


module_param( count_time , int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(count_time, "This variable stores the count time");



module_param(name , charp, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
MODULE_PARM_DESC(name, "This string stores the name");





static int  __init hello_2_init(void)
{

	printk(KERN_ALERT "The time you have entered is: %i\n", count_time);

	printk(KERN_ALERT "myname is a string: %s\n", name);

	return 0;

}


static void __exit hello_2_exit(void)
{

	printk(KERN_ALERT ,"Goodbye world!!2\n");

}


module_init(hello_2_init);
module_exit(hello_2_exit);





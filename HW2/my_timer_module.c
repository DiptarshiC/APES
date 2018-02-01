
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>


MODULE_LICENSE("GPL");

static struct timer_list my_timer;


static  int count_time = 500;

static long t2=0;

static long t1=0;

static  long diff2=0;

static long  ms=0;

static char *name = "Diptarshi";



/*Now we are actually making the mechanism up -- making the variables command
* line arguments rather than just a bunch of global variables
*/


module_param( count_time , int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(count_time, "This variable stores the count time");



module_param(name , charp, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
MODULE_PARM_DESC(name, "This string stores the name");



void my_timer_callback(unsigned long data)
{

	t2=jiffies;
	diff2=t2-t1;

	ms= diff2*1000/HZ;
	printk( "My name is (%s) and the time elapsed is: (%d) in msecs \n",name, (ms));

}


int init_module( void )
{

	int ret;

	printk("Timer module installing\n");

	//my_timer_function, my_timer_data

	setup_timer(&my_timer,my_timer_callback,0);

	printk( "Starting timer to fire in 500ms (%ld)\n"/*,msecs_to_jiffies*/);

	ret = mod_timer(&my_timer, jiffies+ msecs_to_jiffies(count_time) );
	t1=jiffies;

	if (ret) printk("Error in mod timer\n");

	return 0;

}

void cleanup_module(void)
{

	int ret;

	ret = del_timer(&my_timer);

	return;

}

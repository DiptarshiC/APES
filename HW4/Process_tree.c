/**
*@file: Process_tree.c
*
*@Description: The kernel module takes a process ID
*to investigate the process tree lineage. This module
*should print information about the
*parent processes as it traverses backwards up
*the process tree until it cannot go any further. For each
*process it goes through,it prints the following metrics about the
*process.
*
*@Date: 2nd February 2017
*
*@Author: Diptarshi Chakraborty
*
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/pid.h>


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("DIPTARSHI CHAKRABORTY");

static int __init hello_init(void)
{ 
	
	struct task_struct *task;
	task=current;
	do
	{

	printk(KERN_ALERT "\n\n\n"); 
	printk(KERN_ALERT "The process %s has pid : %d\n",task->comm,
	
					task->pid);
       	printk(KERN_ALERT "The status of the current task is %d\n",
				task->state);

	printk(KERN_ALERT "The prio of the current task is %d\n",
                                task->prio);



	task=task->parent;
	}while((task->pid)>0);
	return 0;
}
static void __exit hello_exit(void)
{

	printk(KERN_ALERT "Goodbye, cruel world\n");

}

module_init(hello_init);
module_exit(hello_exit);







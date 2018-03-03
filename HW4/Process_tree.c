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
	int count=0;
	struct task_struct *task;
	struct task_struct *task1;
	struct list_head *list;
	/**
	Since the question mentions that we can take the
	current task as the starting task for the third
	problem, task is being assigned to current
	*/
	task=current;
	while((task->pid)>0)
	{
	count=0;
	printk(KERN_INFO "\n\n\n"); 
	printk(KERN_INFO "The process %s has pid : %d\n",task->comm,
	
					task->pid);
       	printk(KERN_INFO "The status of the current task is %d\n",
				task->state);

	printk(KERN_INFO "The priority of the current task is %d\n",
                                task->prio);

	 printk(KERN_INFO "The nice value of the current task is %d\n",
          
		                      task_nice(task));
	
	/**
	This macro wraps around a for loop
	Incrementing the value of 'count' inside
	the for loop, i count the number of children
	*/
	list_for_each(list, &task->children) 
	{
	
		count=count+1;
	
	}

	 printk(KERN_INFO "The no. children of the current task is %d\n",
						count);
		


	task=task->parent;
	}
	return 0;
}
static void __exit hello_exit(void)
{

	printk(KERN_INFO "Goodbye, cruel world\n");

}

module_init(hello_init);
module_exit(hello_exit);







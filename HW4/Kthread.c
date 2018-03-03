/**
*@file: Kthread.c
*
*@Description: use the kthread API to create
*a kernel module with a second thread that
*allows the two to communicate via queues (kfifo).
*The first thread should send information to the
*second thread on a timed interval through the
*fifo. The second thread should take data from the kfifo
*and print it to the kernel logger.
*The info you should pass should relate to the currently scheduled
*processes in the rbtree. What was
*the ID and vruntime of the previous, current, and next PID
*
*@Author : Diptarshi Chakraborty
*
*@Date : February 3rd 2018
*
*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/kthread.h>

static struct task_struct *thread_st;
static struct task_struct *thread_st1;

// Function executed by kernel thread
static int thread_fn(void *unused)
{
    
    
        printk(KERN_INFO "This is baby thread\n");
        
    
    printk(KERN_INFO "Thread Stopping\n");
    do_exit(0);
    return 0;
}
// Module Initialization
static int __init init_thread(void)
{
    printk(KERN_INFO "Creating Thread1\n");
    //Create the kernel thread with name 'mythread'
    thread_st = kthread_run(thread_fn, NULL, "mythread");
    if (thread_st)
	{
        printk("Thread1 Created successfully\n");
	}
    else
	{
        printk(KERN_INFO "Thread creation failed\n");
	}

	 printk(KERN_INFO "Creating Thread2\n");
    //Create the kernel thread2 with name 'mythread'
    thread_st1 = kthread_run(thread_fn, NULL, "mythread2");
    if (thread_st1)
        {
        printk("Thread2 Created successfully\n");
        }
    else
	{
        printk(KERN_INFO "Thread creation failed\n");
        }

	
    return 0;
}
// Module Exit
static void __exit cleanup_thread(void)
{
    printk(KERN_INFO "Cleaning Up\n");
}

module_init(init_thread);
module_exit(cleanup_thread);


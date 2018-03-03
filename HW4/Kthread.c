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

#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/current.h>
#include <linux/pid.h>
#include <linux/time.h>   // for using jiffies 
#include <linux/timer.h>


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("DIPTARSHI CHAKRABORTY");

static struct task_struct *thread1;

int thread_fn(void) 
{


printk(KERN_INFO "Hello ! My name is thread1.I am a baby thread\n");


return 0;
}

static int  thread_init(void)
{ 
	
   
	char  our_thread[8]="thread1";
	printk(KERN_INFO "in init\n");
	thread1 = kthread_run(thread_fn,NULL,our_thread);
	if((thread1))
        {
        printk(KERN_INFO "in if\n");
        wake_up_process(thread1);
        }

    return 0;


}

static void  thread_exit(void)
{

	int ret;
	ret = kthread_stop(thread1);
	if(!ret)
	{
	printk(KERN_INFO "Thread stopped\n");
	}
}


module_init(thread_init);
module_exit(thread_exit);

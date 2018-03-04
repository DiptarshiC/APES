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
#include <linux/kfifo.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>


/* fifo size in elements (ints) */
#define FIFO_SIZE	32

/* name of the proc entry */
#define	PROC_FIFO	"int-fifo"

/* lock for procfs read access */
static DEFINE_MUTEX(read_lock);

/* lock for procfs write access */
static DEFINE_MUTEX(write_lock);

/*This function declares a dynamically allocated FIFO*/
static DECLARE_KFIFO_PTR(test, int);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("DIPTARSHI CHAKRABORTY");

static struct task_struct *thread_st;
static struct task_struct *thread_st1;

/**
*@function:writer
*
*@description:This thread writes to the kfifo 
*	at regular intervals and calls the kern
*	logger thread.The info that it writes is about 
*	the currently scheduled process in the rbtree.
*	i.e, the id, vruntime of the current prev and 
*	next pid.
*
*@param:none(void *unused) 	
*
*@return:(int)
*/

static int writer(void *unused)
{
    
    
    printk(KERN_INFO "This is writer thread \n");
        
    
    printk(KERN_INFO "Writer thread stopping\n");
    do_exit(0);
    return 0;
}


/**
*@function:kern_logger
*
*@description:This thread reads from the kfifo
*       and prints to the kernel log when called
*      	by the reader function.
*
*@param:none(void *unused)
*
*@return:(int)
*/

static int kern_logger(void *unused)
{


	printk(KERN_INFO "This is kern logger thread\n");
	printk(KERN_INFO "kern logger thread Stopping\n");
	do_exit(0);
	return 0;
}

/**
*@function:init_thread
*
*@description:This function creates the two threads
*       kern_logger and writer.
*       
*
*@param:none(void)
*
*@return:(int)
*/

static int __init init_thread(void)
{
	printk(KERN_INFO "Creating Writer thread\n");
	//Create the kernel thread with name 'mythread'
	thread_st = kthread_run(writer, NULL, "Writer");
	if (thread_st)
	{
        printk("Writer thread Created successfully\n");
	}
    	else
	{
        printk(KERN_INFO "Writer thread creation failed\n");
	}

	printk(KERN_INFO "Creating Kern Logger thread\n");
	//Create the kernel thread2 with name 'mythread'
	thread_st1 = kthread_run(kern_logger, NULL, "Kern_logger");
	if (thread_st1)
        {
        printk("Kern logger Created successfully\n");
        }
	else
	{
        printk(KERN_INFO "Kern logger thread creation failed\n");
        }

	
    return 0;
}


/**
*@function:cleanup_thread
*
*@description:This function is called during the 
*       cleanup of the kernel module kthread.c
*
*
*@param:none(void)
*
*@return:void
*/

static void __exit cleanup_thread(void)
{
    printk(KERN_INFO "Cleaning Up\n");
}

module_init(init_thread);
module_exit(cleanup_thread);


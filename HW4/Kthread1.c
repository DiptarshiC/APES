
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
#include <linux/timer.h>

struct task_struct *task;


/*Create a timer of type list_timer*/
static struct timer_list my_timer;

/* fifo size in elements (ints)The fifo 
	stores 3 elements of size integer */
#define FIFO_SIZE	32


/* lock for procfs read access */
static DEFINE_MUTEX(read_lock);

/* lock for procfs write access */
static DEFINE_MUTEX(write_lock);

/*This function declares a dynamically allocated FIFO*/
static DECLARE_KFIFO(test, unsigned char,FIFO_SIZE);


static int kern_logger(void *unused);
static int writer(void *unused);



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
    task=current;
    unsigned char	buf[6];
    unsigned char	i, j;
    unsigned int	ret;
    unsigned char 	p;	
    unsigned char       *g;

	 INIT_KFIFO(test);    	
    	 printk(KERN_INFO "\n\n");
	 printk(KERN_INFO "Now let us put data into the fifo. \n");
	
    for (i = 0; i != 10; i++)
	{
	p=kfifo_put(&test, i);
printk(KERN_INFO "The number of processed elements in the kfifo is %d\n",p);
	}

   for (i = 0; i != 10; i++)
        {
        p=kfifo_get(&test, g);
printk(KERN_INFO "The element in the kfifo is %d\n",(*g));

        }

 
   
    
    printk(KERN_INFO "This is writer thread \n");
    printk(KERN_INFO "The name of the task is %s\n",(task->comm));
    printk(KERN_INFO "The PID of the task is %d\n",(task->pid));
    printk(KERN_INFO "Writer thread stopping\n");
    printk(KERN_INFO "\n\n");
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

	task=current;
	printk(KERN_INFO "\n\n");
	
		

	printk(KERN_INFO "This is kern logger thread\n");
	printk(KERN_INFO "The name of the task is %s\n",(task->comm));
	printk(KERN_INFO "The PID of the task is %d\n",(task->pid));
	printk(KERN_INFO "kern logger thread Stopping\n");
	printk(KERN_INFO "\n\n");
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
	int ret;
 
	ret = del_timer( &my_timer );
    	printk(KERN_INFO "Cleaning Up\n");
}

module_init(init_thread);
module_exit(cleanup_thread);


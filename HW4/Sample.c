#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/irq.h>
#include <linux/clk.h>
#include <linux/list.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/rtmutex.h>
#include <linux/hrtimer.h>
#include <linux/delay.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("DIPTARSHI CHAKRABORTY");

 
static struct task_struct *thread_10ms;
 
static int bus_rt_timer_thread(void *arg) 
{
	ktime_t timeout = ktime_get();
	while(!kthread_should_stop()) {
	
	printk("Hello!\n");
	timeout = ktime_add_us(timeout, 10000);
	__set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_hrtimeout_range(&timeout, 100, HRTIMER_MODE_ABS);
	}
	return 0;	
}
 
int __init bus_timer_interrupt_init(void) 
{
	struct sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };
	
	thread_10ms = kthread_create(bus_rt_timer_thread, NULL, "bus_10ms");
	if (IS_ERR(thread_10ms)) {
	printk(KERN_ERR "RT Failed to create RT thread\n");
	return -ESRCH;
	}
	sched_setscheduler(thread_10ms, SCHED_FIFO, &param);
	wake_up_process(thread_10ms);
	return 0;
}
 
void __exit  bus_timer_interrupt_exit(void) 
{
	kthread_stop(thread_10ms);
	printk(KERN_INFO "RT Thread removed.\n");
}

   
module_init(bus_timer_interrupt_init);
module_exit(bus_timer_interrupt_exit);


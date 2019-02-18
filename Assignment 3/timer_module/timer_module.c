/*
*@name animal_sorting.c
*@description: taking an array of animal names from the user and then processing them as per the input paramters.
*@reference: https://qnaplus.com/how-to-implement-periodic-timer-in-linux-kernel/
*author Puneet Bansal
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h> 		/*To include timer functionality*/

static char *name="Puneet";
static int ms_period=500;  //default period in ms
static uint32_t count=0;

struct timer_list timerInstance;

void timer_handler(unsigned long data)
{
count=count+1;
mod_timer( &timerInstance, jiffies + msecs_to_jiffies(ms_period));

printk(KERN_INFO "Entered the handler function.\n");
printk(KERN_INFO "Name is %s and the count is %d",name,count);
}


int __init timer_module_init(void)
{
printk(KERN_ALERT "Welcome to the world of kernel modules\n");
setup_timer(&timerInstance, timer_handler, 0);
mod_timer( &timerInstance, jiffies + msecs_to_jiffies(ms_period));
return 0;
}

void __exit timer_module_exit(void)
{
del_timer(&timerInstance);
printk(KERN_ALERT "I guess it's time to say goodbye\n");
}

module_init(timer_module_init);
module_exit(timer_module_exit);

module_param(name, charp, S_IRUGO);
module_param(ms_period, int, S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PUNEET");
MODULE_DESCRIPTION("IMPLEMENTING Q4 OF HOMEWORK 3");

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h> /*To include timer functionality*/

//#include <linux/sort.h>	
	
//static char *name="Puneet";
//static int ms_period=500;
//static uint32_t count=0;
//struct timer_list timerInstance;

static char *animal_arr[]={"cat", "frog", "dog"};


static int compare(const void *a, const void *b)
{
return strcmp(*(const char**a), *(const char **b));
}


int __init sort_module_init(void)
{
printk(KERN_ALERT "Welcome to the world of kernel modules\n");
int n= sizeof(animal_arr)/sizeof(animal_arr[0]);
qsort(animal_arr,n,sizeof(const char *),compare);
for(int i=0;i<n;i++)
{
printk("%s\n",animal_arr[i]);
}
return 0;
}

void __exit sort_module_exit(void)
{

printk(KERN_ALERT "I guess it's time to say goodbye\n");
}

module_init(sort_module_init);
module_exit(sort_module_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("PUNEET");
MODULE_DESCRIPTION("IMPLEMENTING SORTING");

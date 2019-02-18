/*
*@name animal_sorting.c
*@description: taking an array of animal names from the user and then processing them as per the input paramters.
*@reference: https://isis.poly.edu/kulesh/stuff/src/klist/
             https://www.geeksforgeeks.org/c-program-sort-array-names-strings/
*@author: Puneet Bansal
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h> /*To include timer functionality*/

#include <linux/sort.h>	
#include <linux/slab.h>


static char *animal_arr[50];//={};{"cat", "frog", "dog", "dog","horse","tiger","pigeon","pigeon","pigeon","pigeon"};
static char *name=NULL;
static int count=0;
static int count1;
static uint32_t no_of_nodes, memory_allocated;
static uint32_t memory_allocated1;
static uint32_t no_of_nodes1=0;

struct animals
{
char *animName;
int count;
struct list_head list;
};

typedef struct
{
char *filtered_animName;
int filtered_count;
struct list_head filtered_list;
}filtered_animals;

struct animals head;
struct animals *ptr,*temp;
filtered_animals head1;
filtered_animals *ptr1,*temp1;

/*Compare function for sorting*/
static int compare(const void *a, const void *b)
{
return strcmp(*(const char **)a , *(const char **)b);
}


/*Init*/
int __init sort_module_init(void)
{

int n,i,j;

INIT_LIST_HEAD(&(head.list));
INIT_LIST_HEAD(&(head1.filtered_list));

printk(KERN_ALERT "Welcome to the world of kernel modules\n");
printk("Unsorted\n");

n= sizeof(animal_arr)/sizeof(animal_arr[0]);
for(i=0;i<count1;i++)
{
printk("%s\n",animal_arr[i]);
}

sort(animal_arr,count1,sizeof(const char *),compare,NULL);

printk(KERN_ALERT "*************************SORTED ARRAY IS*************************\n");
for(i=0;i<count1;i++)
{
printk("%s\n",animal_arr[i]);
}

ptr=kmalloc(sizeof(struct animals),GFP_KERNEL);
if(ptr!=NULL)
{
	no_of_nodes=1;	
	INIT_LIST_HEAD(&(ptr->list));
	ptr->animName=animal_arr[0];	
	ptr->count=1;
	list_add_tail(&(ptr->list),&head.list);
}
else
{
	printk("Kmalloc failed\n");	
	return -1;
}

for(j=1;j<count1;j++)
{
	if(strcmp(ptr->animName,animal_arr[j])==0)
	{
		ptr->count++;
		printk(KERN_INFO "Duplicate instance of %s\n",animal_arr[j]);
	}
	else
	{
		ptr=kmalloc(sizeof(struct animals),GFP_KERNEL);
		if(ptr!=NULL)
		{
			no_of_nodes++;
			INIT_LIST_HEAD(&(ptr->list));
			ptr->animName=animal_arr[j];
			ptr->count=1;
			list_add_tail(&(ptr->list),&head.list);
		}
		else
		{
			printk("Kmalloc failed\n");
			return -1;
		}

	}
}

printk(KERN_ALERT "*********************ECOSYSTEM PRINTING*************************\n");
memory_allocated= no_of_nodes * sizeof(struct animals);
printk("Total number of linked list nodes created %d\n", no_of_nodes);
printk("Total memory dynamically allocated %d\n",memory_allocated);

//list_for_each_entry(ptr, &(head.list),list)
//	printk("Name is %s and count is %d\n",ptr->animName,ptr->count);


list_for_each_entry(ptr, &(head.list),list)
	{
		printk("Name is %s and count is %d\n",ptr->animName,ptr->count);
		if(name!=NULL)
		{
			if( (strcmp(ptr->animName,name)==0) && ptr->count>=count)	
			{
				ptr1=kmalloc(sizeof(filtered_animals),GFP_KERNEL);
				if(ptr1!=NULL)
				{
					INIT_LIST_HEAD(&(ptr1->filtered_list));
					ptr1->filtered_animName=ptr->animName;
					ptr1->filtered_count=ptr->count;
					list_add_tail(&(ptr1->filtered_list),&head1.filtered_list);
					no_of_nodes1++;
				}
				else
				{
					printk("Kmalloc failed\n");
					return -1;
				}
			}
			else if( (strcmp(ptr->animName,name)==0))
			{
				ptr1=kmalloc(sizeof(filtered_animals),GFP_KERNEL);
				if(ptr1!=NULL)
				{
					INIT_LIST_HEAD(&(ptr1->filtered_list));
					ptr1->filtered_animName=ptr->animName;
					ptr1->filtered_count=ptr->count;
					list_add_tail(&(ptr1->filtered_list),&head1.filtered_list);
					no_of_nodes1++;
				}
				else
				{
				printk("Kmalloc failed\n");
				return -1;
				}
	
			}		
		}
	
		else if(ptr->count>=count)
		{
			ptr1=kmalloc(sizeof(filtered_animals),GFP_KERNEL);
			if(ptr1!=NULL)
			{
				INIT_LIST_HEAD(&(ptr1->filtered_list));
				ptr1->filtered_animName=ptr->animName;
				ptr1->filtered_count=ptr->count;
				list_add_tail(&(ptr1->filtered_list),&head1.filtered_list);
				no_of_nodes1++;
			}
			else
			{
				printk("Kmalloc failed\n");
				return -1;
			}	
		}
	
		else
		{
			ptr1=kmalloc(sizeof(filtered_animals),GFP_KERNEL);
			if(ptr1!=NULL)
			{
				INIT_LIST_HEAD(&(ptr1->filtered_list));
				ptr1->filtered_animName=ptr->animName;
				ptr1->filtered_count=ptr->count;
				list_add_tail(&(ptr1->filtered_list),&head1.filtered_list);
				no_of_nodes1++;
			}
			else
			{
				printk("Kmalloc failed\n");
				return -1;
			}
		}	
	
	}
	
printk(KERN_ALERT "*************************FILTERS APPLIED*************************\n");

memory_allocated1= no_of_nodes1*sizeof(filtered_animals);
printk("The number of nodes allocated is %d\n",no_of_nodes1);
printk("Memory allocated is %d\n",memory_allocated1);
list_for_each_entry(ptr1, &(head1.filtered_list),filtered_list)
	printk("Name is %s and count is %d\n",ptr1->filtered_animName,ptr1->filtered_count);

printk("\n");
return 0;

}

void __exit sort_module_exit(void)
{
printk("Freeing the first list\n");
list_for_each_entry(ptr, &(head.list),list)
{
	temp=ptr;
	kfree(temp);
}
printk("Freeing the second list\n");
list_for_each_entry(ptr1, &(head1.filtered_list),filtered_list)
{
	temp1=ptr1;
	kfree(temp1);
}
printk(KERN_ALERT "I guess it's time to say goodbye\n");
}

module_init(sort_module_init);
module_exit(sort_module_exit);

module_param(name,charp, S_IRUGO);
module_param(count, int, S_IRUGO);
module_param_array(animal_arr,charp,&count1,S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PUNEET");
MODULE_DESCRIPTION("IMPLEMENTING SORTING");

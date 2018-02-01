/**
*sys_sort is a system call that sorts a given data 
*array from user space and
*
*
*
*/

#include <linux/kernel.h>//gives us access to KERN_ALERT macro and things like that
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/gfp.h>
#include <uapi/asm-generic/errno-base.h>



asmlinkage long sys_sort(int32_t __user * src,size_t len, int32_t __user* dst)
{


	if(src==NULL)
	{

		printk(KERN_ALERT "The user has entered a null pointer \n");
		return EINVAL;

	}


	int32_t i,j;


	int32_t * temp = ( int32_t*) kmalloc(len*sizeof(int32_t),GFP_KERNEL);
	int32_t t;

	if(temp==NULL)
	{
		printk(KERN_ALERT "The creation of memory in kernel  has failed \n");
		return EINVAL;
	}

	printk(KERN_ALERT "Now I shall print characters from user space!\n");

	for(i=0;i<len;i++){

	printk( KERN_ALERT "%d\n",*(src+i));

	}

	

	if(copy_from_user(temp,src,4*len)==0)
	{

		printk(KERN_ALERT "The copy of user buffer into kernel space has been successful \n");

	}
	else
	{

		printk(KERN_ALERT "The copy of memory from user space to kernel space has not been successful \n");
		return EFAULT;
	}

	printk(KERN_ALERT "Now I shall print characters from kernel space!\n");

        for(i=0;i<len;i++){

        printk( KERN_ALERT "%d\n",*(temp+i));

        }



	printk(KERN_ALERT "System call sorting has started \n");
	for(i=0; i<(len-1); i++)
	{
		for(j=0;j<(len-i-1);j++)
		{

			if(*(temp+j)<*(temp+j+1))
			{

			t=*(temp+j);
			*(temp+j)=*(temp+j+1);
			*(temp+j+1)=t;

			}
		}
	}
	printk(KERN_ALERT "System call for sorting ended \n");


	       printk(KERN_ALERT "Now I shall print characters from user space after sorting!\n");

        for(i=0;i<len;i++){

        printk( KERN_ALERT "%d\n",*(temp+i));

        }


	printk(KERN_ALERT "Now we shall again copy memory from kernel space to user space \n");

	if(copy_to_user(dst,temp,4*len)==0)
	{

		printk(KERN_ALERT "System call successfully transferred from kernel space to user space \n");

	}
	else
	{

		printk(KERN_ALERT "System call was unsuccessfull in transferring memory from kernel space to user space \n");
	}


	printk(KERN_ALERT "Now I shall print characters from user space!\n");

        for(i=0;i<len;i++){

        printk( KERN_ALERT "%d\n",*(dst+i));

        }


	kfree(temp);

	return 0;
}


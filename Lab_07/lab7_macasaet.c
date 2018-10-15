/*
 * John Rufino I. Macasaet
 * 2013-18722
 * CoE 135 HLM FVW
 */

#include <linux/init.h>
#include <linux/kernel.h>  
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#define BUFFER 128

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("John Rufino Macasaet");
MODULE_DESCRIPTION("Prints nth triangular number to /proc/macasaet_procfs");

static int n=1; // default value of n is 1
module_param(n,int,0);


static struct proc_dir_entry *mymod;

/* Define read for file operations */
static ssize_t read_op(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) {
	char buf[BUFFER];
	int len=0;

    /* Calculate the nth triangular number */
    int n_tri = (n*(n+1))/2;

    /* error checking */
	if(*ppos > 0 || count < BUFFER){
		return 0;
    }
    /* Write nth triangular number to  */
	len += sprintf(buf,"n = %d\nResult = %d\n",n, n_tri);
    //printk(KERN_INFO "n = %d Result = %d\n", n, (n*(n+1))/2);
	
    /* copy to user space */
	if(copy_to_user(ubuf,buf,len)){
		return -EFAULT; // check if bad address
    }

	*ppos = len;
	return len;
}

/* Define write operation: get data from user space then copy to kernel space */
static ssize_t write_op(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos){
	int num, i, var;
	char buf[BUFFER];
    /* error checking */
	if(*ppos > 0 || count > BUFFER){
		return -EFAULT; 
    }

    /* copy the data from the user address space to the kernel address space */
	if(copy_from_user(buf, ubuf, count)){
		return -EFAULT; // check if bad address
    }

	num = sscanf(buf, "%d", &i);

	n = i; 
	var = strlen(buf);
	*ppos = var;

    /* print nth triangular number to kernel ring buffer */
    printk(KERN_INFO "n = %d Result = %d\n", n, (n*(n+1))/2);
	return var;
}

/* define file op to be passed to proc_create() */
static struct file_operations operations = {
	.owner = THIS_MODULE,
	.read = read_op,
	.write = write_op,
};

/* module_init */
static int lab7_macasaet_init(void){
	mymod = proc_create("macasaet_procfs", 0, NULL, &operations);
	printk(KERN_INFO "LKM inserted.\n");
	return 0;
}

/* module_exit */
static void lab7_macasaet_cleanup(void){
	proc_remove(mymod);
	printk(KERN_INFO "LKM removed.\n");
}

module_init(lab7_macasaet_init);
module_exit(lab7_macasaet_cleanup);
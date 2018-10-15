/*
 * John Rufino I. Macasaet
 * 2013-18722
 * CoE 135 HLM FVW
 */

/*#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

int len, temp;

char *msg;

int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{

    if(count>temp){
        count=temp;
    }
    temp=temp-count;
    copy_to_user(buf,msg, count);
    if(count==0){
        temp=len;
    }
    return count;
}

struct file_operations proc_fops = {
    read: read_proc
};
void create_new_proc_entry(void) {
    proc_create("hello",0,NULL,&proc_fops);

    msg=" Hello World ";
    len=strlen(msg);
    temp=len;
    printk(KERN_INFO "1.len=%d",len);
}

int proc_init (void) {
    create_new_proc_entry();
    return 0;
}

void proc_cleanup(void) {
    remove_proc_entry("hello",NULL);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);*/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#define BUFSIZE  100
 
 
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Liran B.H");
 
 
static struct proc_dir_entry *ent;
 
static ssize_t mywrite(struct file *file, const char __user *ubuf,size_t count, loff_t *ppos) 
{
	printk( KERN_DEBUG "write handler\n");
	return -1;
}
 
static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
	printk( KERN_DEBUG "read handler\n");
	return 0;
}
 
static struct file_operations myops = 
{
	.owner = THIS_MODULE,
	.read = myread,
	.write = mywrite,
};
 
static int simple_init(void)
{
	ent=proc_create("mydev",0660,NULL,&myops);
	return 0;
}
 
static void simple_cleanup(void)
{
	proc_remove(ent);
}
 
module_init(simple_init);
module_exit(simple_cleanup);
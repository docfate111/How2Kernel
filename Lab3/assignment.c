/*  
 * Perform a single key xor on the string with the given key and print 
 * the resulting hex encoded string by implementing read 
 * for the procfs file. 
 * 
 */
#include <linux/module.h>       /* Specifically, a module */
#include <linux/kernel.h>       /* We're doing kernel work */
#include <linux/proc_fs.h>      /* Necessary because we use the proc fs */
#include <linux/uaccess.h>        /* for copy_from_user */
#include<linux/string.h>
#define PROCFS_MAX_SIZE         1024
#define PROCFS_NAME             "assignment"

MODULE_LICENSE("GPL");

/**
 *
 * This structure hold information about the /proc file
 *
 */
static struct proc_dir_entry *Our_Proc_File;
/**
 * The buffer used to store character for this module
 *
 */
static char procfs_buffer[PROCFS_MAX_SIZE];
/**
 * The size of the buffer
 *
 */
static unsigned long procfs_buffer_size = 0;
/**
 * This function is called then the /proc file is read
 *
 */
static
ssize_t
procfile_read(struct file *file,
              char *buffer,
              size_t buffer_length, loff_t *offset)
{
	static int flag = 0; 
	if(flag) {
	  printk(KERN_INFO "read : END\n");
		flag = 0;
		return 0;
	}
	printk(KERN_INFO "read (/proc/%s) : called\n",PROCFS_NAME);
	flag = 1;
	procfs_buffer_size = buffer_length;
        if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
                procfs_buffer_size = PROCFS_MAX_SIZE;
        }
        /* write data to the buffer */
        if ( copy_to_user(procfs_buffer, buffer, procfs_buffer_size) ) {
                return -EFAULT;
        }
        return procfs_buffer_size;
}
/**
 * This function is called with the /proc file is written
 *
 */
static
ssize_t procfile_write(struct file *file,const char *buffer, size_t count, loff_t *offset)
{
        /* get buffer size */
          size_t i;
        procfs_buffer_size = count;
        if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
                procfs_buffer_size = PROCFS_MAX_SIZE;
        }
        for(i=0; i<PROCFS_MAX_SIZE; i++){
          procfs_buffer[i]^= 13;
        }
        /* write data to the buffer */
        if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
                return -EFAULT;
        }
        return procfs_buffer_size;
}

static struct file_operations fops_struct = {
	.read = procfile_read,
	.write = procfile_write,
};

/**
 *This function is called when the module is loaded
 *
 */
static int __init assignment_init(void)
{
        /* create the /proc file */
        Our_Proc_File = proc_create(PROCFS_NAME, 0644, NULL, &fops_struct);
        if (Our_Proc_File == NULL) {
                remove_proc_entry(PROCFS_NAME, NULL);
                printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
                         PROCFS_NAME);
                return -ENOMEM;
        }
//        Our_Proc_File->owner       = THIS_MODULE;
//        Our_Proc_File->mode        = S_IFREG | S_IRUGO;
//        Our_Proc_File->uid         = 0;
//        Our_Proc_File->gid         = 0;
//        Our_Proc_File->size        = 37;
        printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);
        return 0;        /* everything is ok */
}
/**
 *This function is called when the module is unloaded
 *
 */
static void __exit assignment_exit(void)
{
        remove_proc_entry(PROCFS_NAME, NULL);
        printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}

module_init(assignment_init);
module_exit(assignment_exit);

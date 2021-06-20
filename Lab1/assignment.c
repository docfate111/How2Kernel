/*  
 *  Let's Test - Complete the module as mentioned 
- Take a string as a argument during `insmod`
- Add the name of the file in the Makefile
- Compile and test it.
 * 
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("docfate111 <tdwilliams@gmail.com>");
MODULE_DESCRIPTION("A sample driver");

static char *mystring = "blah";
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");
// Have a integer variable 'key' defined as 13.
static int myint = 13;
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");

static int __init assignment_init(void)
{
  // Print the string taken from the user and the key in the init_module()
  printk("String taken from the user: %s\n", mystring);
  printk("The key %d\n", myint);
  return 0;
}

static void __exit assignment_exit(void)
{
  size_t i;
  // Perform Ceaser Cipher on the string and print the result during cleanup_module()
  if(!mystring){ return; }
  for(i=0; i < strlen(mystring); i++){
    mystring[i] += myint;
  }
  printk("Result: %s\n", mystring);
}

module_init(assignment_init);
module_exit(assignment_exit);

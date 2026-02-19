#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int __init hw_init(void) {
  printk(KERN_INFO "hwmod: Hello World from kernel!\n");
  return 0;
}

static void __exit hw_exit(void) { printk(KERN_INFO "hwmod: Goodbye!!!\n"); }

module_init(hw_init);
module_exit(hw_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Surtsev");
MODULE_DESCRIPTION("Simple kernel module with hello world printing :3");

#include <linux/atomic.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

#define DEFAULT_PATH "/var/tmp/test_module"

static char *log_file_path = "test.log";
module_param(log_file_path, charp, 0644);
MODULE_PARM_DESC(log_file_path, "Filename in /var/tmp/test_module/");

static int period = 5;
module_param(period, int, 0644);
MODULE_PARM_DESC(period, "Timer period in seconds");

static struct timer_list my_timer;
static struct work_struct my_work;
static atomic_t module_active = ATOMIC_INIT(1);
static int counter = 1;

static void write_to_file(const char *path, int n) {
  struct file *f;
  char buf[128];
  loff_t pos = 0;
  int len;

  len = scnprintf(buf, sizeof(buf), "Hello from kernel module (%d)\n", n);

  f = filp_open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (IS_ERR(f)) {
    printk(KERN_ERR "test_module: Failed to open %s: %ld\n", path, PTR_ERR(f));
    return;
  }

  kernel_write(f, buf, len, &pos);
  filp_close(f, NULL);
}

static void work_handler(struct work_struct *w) {
  char full_path[256];

  if (!atomic_read(&module_active))
    return;

  snprintf(full_path, sizeof(full_path), "%s/%s", DEFAULT_PATH,
           log_file_path ? log_file_path : "test.log");

  write_to_file(full_path, counter++);

  if (atomic_read(&module_active)) {
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(period * 1000));
  }
}

static void timer_callback(struct timer_list *t) {
  if (atomic_read(&module_active))
    schedule_work(&my_work);
}

static int __init test_module_init(void) {
  printk(KERN_INFO "test_module: Loading '%s' period=%d\n", log_file_path,
         period);

  timer_setup(&my_timer, timer_callback, 0);
  INIT_WORK(&my_work, work_handler);
  mod_timer(&my_timer, jiffies + msecs_to_jiffies(period * 1000));

  printk(KERN_INFO "test_module: Started\n");
  return 0;
}

static void __exit test_module_exit(void) {
  printk(KERN_INFO "test_module: Unloading...\n");
  atomic_set(&module_active, 0);
  del_timer_sync(&my_timer);
  flush_work(&my_work);
  printk(KERN_INFO "test_module: Unloaded\n");
}

module_init(test_module_init);
module_exit(test_module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Surtsev");
MODULE_DESCRIPTION("Test module: /var/tmp/test_module/FILENAME");

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define PROC_NAME "uptime_report"

static unsigned long start_jiffies;

static int uptime_show(struct seq_file *m, void *v)
{
	unsigned long seconds = (jiffies - start_jiffies) / HZ;
	seq_printf(m, "%lu seconds\n", seconds);
	return 0;
}

static int uptime_report_open(struct inode *inode, struct file *file) 
{
	return single_open(file, uptime_show, NULL);
}

static const struct proc_ops proc_file_ops = 
{
	.proc_open    = uptime_report_open,
	.proc_read    = seq_read,
	.proc_lseek   = seq_lseek,
	.proc_release = single_release,
};

static int create_uptime_proc_entry(void)
{
	if (!proc_create(PROC_NAME, 0, NULL, &proc_file_ops)) {
		pr_err("Failed to create /proc/%s\n", PROC_NAME);
		return -ENOMEM;
	}
	pr_info("/proc/%s created\n", PROC_NAME);
	return 0;
}

static int __init uptime_module_init(void)
{
	start_jiffies = jiffies;
	create_uptime_proc_entry();
	pr_info("Uptime Module: Loaded\n");
	return 0;
}

static void __exit uptime_module_exit(void)
{
	remove_proc_entry(PROC_NAME, NULL);
	pr_info("Uptime Module: Unloaded\n");
}

module_init(uptime_module_init);
module_exit(uptime_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Max Altenhofen");
MODULE_DESCRIPTION("A simple hello world kernel module");

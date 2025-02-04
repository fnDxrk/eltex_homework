#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/time.h>

static void write_time_to_file(void)
{
    struct timespec64 time_ts;
    ktime_get_real_ts64(&time_ts);

    int timezone_offset = 7; // NSK (UTC+7)
    int hours = (time_ts.tv_sec / 3600) % 24 + timezone_offset;
    int minutes = (time_ts.tv_sec % 3600) / 60;
    int seconds = time_ts.tv_sec % 60;

    printk("Time: %02d:%02d:%02d\n", hours, minutes, seconds);
}

static int __init hello_init(void)
{

    pr_info("Module loaded\n");
    write_time_to_file();
    return 0;
}

static void __exit hello_exit(void)

{
    pr_info("Module unloaded\n");
    write_time_to_file();
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

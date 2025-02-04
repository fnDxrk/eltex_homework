#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/uaccess.h>

#define FILE_PATH "/var/log/my_module.log"

static void write_time_to_file(const char* message)
{
    struct file* file;
    struct timespec64 ts;
    struct tm tm;
    char buffer[128];
    int len;

    int timezone_offset = 7 * 3600; // NSK (UTC+7)

    ktime_get_real_ts64(&ts);
    time64_to_tm(ts.tv_sec + timezone_offset, 0, &tm);

    len = snprintf(buffer, sizeof(buffer), "%s: %02d:%02d:%02d\n", message, tm.tm_hour, tm.tm_min, tm.tm_sec);

    file = filp_open(FILE_PATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (!IS_ERR(file)) {
        kernel_write(file, buffer, len, &file->f_pos);
        filp_close(file, NULL);
    }
}

static int __init my_module_init(void)
{

    pr_info("Module loaded\n");
    write_time_to_file("Loaded");
    return 0;
}

static void __exit my_module_exit(void)

{
    pr_info("Module unloaded\n");
    write_time_to_file("Unloaded");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#define DEVICE_NAME "btn_poll"
#define GPIO_BTN 529

static int major;

static ssize_t btn_read(struct file *file, char __user *buf,
                        size_t count, loff_t *ppos)
{
    int value;
    char data;

    /* polling: 버튼 눌릴 때까지 대기 */
    do {
        value = gpio_get_value(GPIO_BTN);
        msleep(10);
    } while (value == 1);   // pull-up → 눌리면 0

    data = '1';
    copy_to_user(buf, &data, 1);
    return 1;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = btn_read,
};

static int __init btn_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);

    gpio_request(GPIO_BTN, "btn");
    gpio_direction_input(GPIO_BTN);

    pr_info("Polling driver loaded %d\n",major);
    return 0;
}

static void __exit btn_exit(void)
{
    gpio_free(GPIO_BTN);
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("Polling driver unloaded\n");
}

module_init(btn_init);
module_exit(btn_exit);
MODULE_LICENSE("GPL");

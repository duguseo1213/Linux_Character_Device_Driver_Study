#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define BUTTON_GPIO 529
#define LED_GPIO    539
#define DEVICE_NAME "btnled"
#define CLASS_NAME  "btnled_class"

static int major;
static struct class *btnled_class;
static struct device *btnled_device;

/* 파일 operations */
static ssize_t btnled_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int val = gpio_get_value(BUTTON_GPIO);   // 버튼 읽기
    gpio_set_value(LED_GPIO, val);           // 버튼 상태 따라 LED 켜기/끄기

    if (copy_to_user(buf, &val, sizeof(val)))
        return -EFAULT;

    return sizeof(val);
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = btnled_read,
};

/* 모듈 초기화 */
static int __init btnled_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0)
        return major;

    btnled_class = class_create(CLASS_NAME);
    if (IS_ERR(btnled_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(btnled_class);
    }

    btnled_device = device_create(btnled_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(btnled_device)) {
        class_destroy(btnled_class);
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(btnled_device);
    }

    gpio_request(LED_GPIO, "LED");
    gpio_direction_output(LED_GPIO, 0);

    gpio_request(BUTTON_GPIO, "BUTTON");
    gpio_direction_input(BUTTON_GPIO);

    pr_info("btnled driver loaded (major=%d)\n", major);
    return 0;
}

/* 모듈 종료 */
static void __exit btnled_exit(void)
{
    gpio_set_value(LED_GPIO, 0);
    gpio_free(LED_GPIO);
    gpio_free(BUTTON_GPIO);

    device_destroy(btnled_class, MKDEV(major, 0));
    class_destroy(btnled_class);
    unregister_chrdev(major, DEVICE_NAME);

    pr_info("btnled driver unloaded\n");
}

module_init(btnled_init);
module_exit(btnled_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YourName");
MODULE_DESCRIPTION("Simple Button+LED Driver (Polling only)");

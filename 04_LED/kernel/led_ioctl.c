#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/cdev.h>

#define DEVICE_NAME "myled"
#define MY_MAGIC 'L'
#define IOCTL_LED_ON  _IO(MY_MAGIC, 0)
#define IOCTL_LED_OFF _IO(MY_MAGIC, 1)

/* 안전 GPIO 번호 사용: Raspberry Pi 4에서 안전한 출력 핀 */
#define LED_GPIO 516

static int major;
static struct cdev my_cdev;

static long led_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case IOCTL_LED_ON:
            gpio_set_value(LED_GPIO, 1);
            printk(KERN_INFO "LED ON\n");
            break;
        case IOCTL_LED_OFF:
            gpio_set_value(LED_GPIO, 0);
            printk(KERN_INFO "LED OFF\n");
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

static int led_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "LED device opened\n");
    return 0;
}

static int led_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "LED device closed\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .release = led_release,
    .unlocked_ioctl = led_ioctl,
};

static int __init led_init(void)
{
    int ret;

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if(major < 0){
        printk(KERN_ALERT "Failed to register LED device\n");
        return major;
    }

    /* GPIO 요청 */
    ret = gpio_request_one(LED_GPIO, GPIOF_OUT_INIT_LOW, "myled");
    if(ret){
        printk(KERN_ALERT "Failed to request GPIO %d, ret=%d\n", LED_GPIO, ret);
        unregister_chrdev(major, DEVICE_NAME);
        return ret;
    }

    printk(KERN_INFO "LED device registered, major=%d\n", major);
    return 0;
}

static void __exit led_exit(void)
{
    gpio_set_value(LED_GPIO, 0);  /* LED 끄기 */
    gpio_free(LED_GPIO);           /* GPIO 해제 */
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "LED device unregistered\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("현대");
MODULE_DESCRIPTION("LED control via ioctl example - safe GPIO");

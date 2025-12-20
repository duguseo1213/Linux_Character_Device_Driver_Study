#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#define DEVICE_NAME "mychardev"
#define MY_MAGIC 'M'
#define IOCTL_SET_VAL _IOW(MY_MAGIC, 1, int)
#define IOCTL_GET_VAL _IOR(MY_MAGIC, 2, int)

static int major;
static int device_value = 0;
static struct cdev my_cdev;

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int tmp;

    switch (_IOC_NR(cmd)) {
        case 1: // IOCTL_SET_VAL
            if (_IOC_DIR(cmd) & _IOC_WRITE) {
                if (copy_from_user(&tmp, (int __user *)arg, _IOC_SIZE(cmd)))
                    return -EFAULT;
                device_value = tmp;
                printk(KERN_INFO "Device value set to %d\n", device_value);
            }
            break;

        case 2: // IOCTL_GET_VAL
            if (_IOC_DIR(cmd) & _IOC_READ) {
                tmp = device_value;
                if (copy_to_user((int __user *)arg, &tmp, _IOC_SIZE(cmd)))
                    return -EFAULT;
            }
            break;

        default:
            return -EINVAL;
    }

    return 0;
}

static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
};

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Failed to register char device\n");
        return major;
    }
    printk(KERN_INFO "Registered char device with major %d\n", major);
    return 0;
}

static void __exit chardev_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Char device unregistered\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("현대");
MODULE_DESCRIPTION("Simple ioctl char device example");

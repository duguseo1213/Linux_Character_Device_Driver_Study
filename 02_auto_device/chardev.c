#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chardev"
#define CLASS_NAME  "chardev_class"
#define BUF_SIZE    128
#define DEV_CNT 3

/* ===== 전역 변수 ===== */
static dev_t dev_num;              // major + minor
static struct cdev chardev_cdev;   // character device 구조체
static struct class *chardev_class;
static struct device *chardev_devices[DEV_CNT];

/* 커널 버퍼 */

struct mydev{
    char kbuf[BUF_SIZE];
    size_t kbuf_len;
};

static struct mydev devices[DEV_CNT];


/* ===== file operations 함수 ===== */
static int dev_open(struct inode *inode, struct file *file)
{

    int minor = iminor(inode);

    if(minor >= DEV_CNT)
        return -ENODEV;

    file->private_data = &devices[minor];

    printk(KERN_INFO "chardev: device opened\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "chardev: device closed\n");
    return 0;
}

static ssize_t dev_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *offset)
{

    struct mydev *data = file->private_data;

    size_t to_copy;

    to_copy = min(len, (size_t)(BUF_SIZE - 1));

    if (copy_from_user(data->kbuf, buf, to_copy)) {
        return -EFAULT;
    }

    data->kbuf[to_copy] = '\0';
    data->kbuf_len = to_copy;

    printk(KERN_INFO "chardev: write \"%s\" (%zu bytes)\n",
           data->kbuf, data->kbuf_len);

    return to_copy;
}

static ssize_t dev_read(struct file *file,
                        char __user *buf,
                        size_t len,
                        loff_t *offset)
{

    struct mydev *data = file->private_data;

    if (*offset >= data->kbuf_len)
        return 0;   // EOF

    if (len > data->kbuf_len)
        len = data->kbuf_len;

    if (copy_to_user(buf, data->kbuf, len)) {
        return -EFAULT;
    }

    *offset += len;

    printk(KERN_INFO "chardev: read %zu bytes\n", len);

    return len;
}

/* ===== file_operations 구조체 ===== */
static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .read    = dev_read,
    .write   = dev_write,
    .release = dev_release,
};

/* ===== 모듈 init ===== */
static int __init chardev_init(void)
{
    int ret;

    /* 1. major/minor 자동 할당 */
    ret = alloc_chrdev_region(&dev_num, 0, DEV_CNT, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "chardev: failed to allocate chrdev region\n");
        return ret;
    }

    /* 2. cdev 초기화 및 등록 */
    cdev_init(&chardev_cdev, &fops);
    chardev_cdev.owner = THIS_MODULE;

    ret = cdev_add(&chardev_cdev, dev_num, DEV_CNT);
    if (ret < 0) {
        printk(KERN_ERR "chardev: failed to add cdev\n");
        goto err_cdev;
    }

    /* 3. class 생성 */
    chardev_class = class_create(CLASS_NAME);
    if (IS_ERR(chardev_class)) {
        printk(KERN_ERR "chardev: failed to create class\n");
        ret = PTR_ERR(chardev_class);
        goto err_class;
    }

    /* 4. device 생성 → /dev/chardev 자동 생성 */
    for (int i = 0; i < DEV_CNT; i++) {
        devices[i].kbuf_len = 0;

        chardev_devices[i] = device_create(
            chardev_class,
            NULL,
            MKDEV(MAJOR(dev_num), i),
            NULL,
            DEVICE_NAME "%d",
            i
        );

        if (IS_ERR(chardev_devices[i])) {
            ret = PTR_ERR(chardev_devices[i]);
            goto err_device;
        }
    }

    printk(KERN_INFO "chardev: loaded (major=%d minor=%d)\n",
           MAJOR(dev_num), MINOR(dev_num));

    return 0;

/* ===== 에러 처리 ===== */
err_device:
    class_destroy(chardev_class);
err_class:
    cdev_del(&chardev_cdev);
err_cdev:
    unregister_chrdev_region(dev_num, DEV_CNT);
    return ret;
}

/* ===== 모듈 exit ===== */
static void __exit chardev_exit(void)
{
    int i;

    for (i = 0; i < DEV_CNT; i++)
        device_destroy(chardev_class,
            MKDEV(MAJOR(dev_num), i));

    class_destroy(chardev_class);
    cdev_del(&chardev_cdev);
    unregister_chrdev_region(dev_num, DEV_CNT);

    printk(KERN_INFO "chardev: unloaded\n");
}


module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("현대");
MODULE_DESCRIPTION("Character Device with auto /dev node (Raspberry Pi)");

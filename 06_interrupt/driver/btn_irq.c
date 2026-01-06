#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/wait.h>

#define DEVICE_NAME "btn_irq"
#define GPIO_BTN 529   // BCM GPIO 번호

static int major;
static int irq_num;

/* 버튼 이벤트 플래그 */
static int btn_event = 0;

/* wait queue */
DECLARE_WAIT_QUEUE_HEAD(btn_wq);

/* ================= ISR ================= */
static irqreturn_t btn_isr(int irq, void *dev_id)
{
    btn_event = 1;                     // 이벤트 발생
    wake_up_interruptible(&btn_wq);    // 잠든 read 깨우기
    return IRQ_HANDLED;
}

/* ================= read ================= */
static ssize_t btn_read(struct file *file, char __user *buf,
                        size_t count, loff_t *ppos)
{
    char data = '1';

    /* 이미 읽었으면 EOF */
    if (*ppos > 0)
        return 0;

    /* 버튼 눌릴 때까지 sleep */
    wait_event_interruptible(btn_wq, btn_event == 1);

    btn_event = 0;   // 이벤트 소비
    *ppos = 1;       // EOF 처리

    copy_to_user(buf, &data, 1);
    return 1;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = btn_read,
};

/* ================= init ================= */
static int __init btn_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);

    gpio_request(GPIO_BTN, "btn");
    gpio_direction_input(GPIO_BTN);

    irq_num = gpio_to_irq(GPIO_BTN); //gpio를 irq로 변환

    request_irq(irq_num,
                btn_isr,
                IRQF_TRIGGER_FALLING, // pull-up → falling
                DEVICE_NAME,
                NULL);

    pr_info("btn_irq driver loaded\n");
    return 0;
}

/* ================= exit ================= */
static void __exit btn_exit(void)
{
    free_irq(irq_num, NULL);
    gpio_free(GPIO_BTN);
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("btn_irq driver unloaded\n");
}

module_init(btn_init);
module_exit(btn_exit);
MODULE_LICENSE("GPL");

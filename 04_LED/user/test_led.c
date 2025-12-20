#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MY_MAGIC 'L'
#define IOCTL_LED_ON  _IO(MY_MAGIC, 0)
#define IOCTL_LED_OFF _IO(MY_MAGIC, 1)

int main() {
    int fd = open("/dev/myled", O_RDWR);
    if(fd < 0){
        perror("open");
        return -1;
    }

    printf("LED ON\n");
    ioctl(fd, IOCTL_LED_ON, 0);
    sleep(2);

    printf("LED OFF\n");
    ioctl(fd, IOCTL_LED_OFF, 0);

    close(fd);
    return 0;
}

//abstraction - 추상화 운영체제의 핵심
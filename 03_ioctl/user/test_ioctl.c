#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MY_MAGIC 'M'
#define IOCTL_SET_VAL _IOW(MY_MAGIC, 1, int)
#define IOCTL_GET_VAL _IOR(MY_MAGIC, 2, int)

int main() {
    int fd = open("/dev/mychardev", O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    int val = 7823;
    if (ioctl(fd, IOCTL_SET_VAL, &val) < 0) {
        perror("ioctl set");
        return -1;
    }

    val = 0;
    if (ioctl(fd, IOCTL_GET_VAL, &val) < 0) {
        perror("ioctl get");
        return -1;
    }

    printf("Device value: %d\n", val);

    close(fd);
    return 0;
}

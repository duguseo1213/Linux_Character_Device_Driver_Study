#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/btnled", O_RDONLY);
    if(fd < 0){ perror("open"); return 1; }

    while(1){
        int val;
        read(fd, &val, sizeof(val));
        printf("BUTTON = %d\n", val);
        usleep(100000);  // 0.1초
    }

    close(fd);
    return 0;
}

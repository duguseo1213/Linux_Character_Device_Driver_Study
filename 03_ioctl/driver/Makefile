# 커널 소스 경로
KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

# 모듈 이름
obj-m := chardev_ioctl.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

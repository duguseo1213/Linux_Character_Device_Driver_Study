## VFS, cdev

echo hi > /dev/chardev 명령을 치면 내부에서 일어나는 일

open("/dev/chardev")

VFS:

1. inode에서 major/minor 확인

2. major/minor → cdev 찾음 cdev중에 특정 major/minor를 가지고 있는 cdev를 찾는 것.

3. cdev → file_operations 획득

4. .open = dev_open 호출

5. write → .write = dev_write

## class

+ 장치파일 자동생성

+ class란 디바이스가 사용자 공간에서 어떤 종류의 장치인지 설명하기 위한 커널 객체

+ sysfs -> 커널 내부 객체를 사용자 공간에 파일 형태로 노출하는 가상 파일 시스템

+ udev -> sysfs와 커널 이벤트를 기반으로 /dev 디바이스 노드를 동적으로 관리하는 사용자 공간 데몬 

``` c

// 1. class 생성 (분류 생성)
struct class *my_class;
my_class = class_create(THIS_MODULE, "my_class");

// 2. device 생성 (장치 하나)
device_create(my_class, NULL, dev_num, NULL, "mydev");

```

+ my_class 라는 종류에 mydev, major ,minor 번호를 유저 공간에 알림. udev는 이를 감지해 dev파일 자동 생성

struct file *file이란?

리눅스 커널에서 열린 파일 하나를 나타내는 구조체

유저가 open("/dev/chardev0", O_RDWR) 같은 시스템 콜을 호출하면
커널은 struct file 인스턴스를 만들어서, 파일 디스크립터와 연결함

장치 드라이버 입장에서는 file 포인터 하나로 해당 open 요청의 상태와 정보를 모두 관리할 수 있음

즉, **“유저 프로그램에서 열린 파일 핸들”**을 커널에서 표현한 구조체라고 생각하면 돼요.
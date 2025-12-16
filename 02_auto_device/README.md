## VFS, cdev

echo hi > /dev/chardev 명령을 치면 내부에서 일어나는 일

open("/dev/chardev")

VFS:

1. inode에서 major/minor 확인

2. major/minor → cdev 찾음

3. cdev → file_operations 획득

4. .open = dev_open 호출

5. write → .write = dev_write

## class

+ 장치파일 자동생성

+ class란 디바이스가 사용자 공간에서 어떤 종류의 장치인지 설명하기 위한 커널 객체
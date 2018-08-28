#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static char msg[10];

int main() {
	char *send_msg = "hello";
	int fd = open("/dev/hello_device", O_RDWR);
	write(fd, send_msg, strlen(send_msg)+1);
	read(fd, msg, 256);
	printf("hello? %s", msg);
	return 0;
}
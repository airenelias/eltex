#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_DIGITS 10
char msg[MAX_DIGITS];

int main() {
	char *send_msg = "222";
	int fd = open("/sys/kernel/bufint/bufint", O_RDWR);
	write(fd, send_msg, strlen(send_msg));
	read(fd, msg, 2);
	printf("hello? %s", msg);
	close(fd);
	return 0;
}
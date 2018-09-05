#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_DIGITS 10
static char msg[MAX_DIGITS];

int main() {
	char *send_msg = "512";
	int fd = open("/proc/bufint", O_RDWR);
	write(fd, send_msg, strlen(send_msg));
	read(fd, msg, MAX_DIGITS);
	printf("hello? %s", msg);
	return 0;
}
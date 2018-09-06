#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_DIGITS 10
char msg[MAX_DIGITS];

int main() {
	char *send_msg = "666777";
	int fd = open("/sys/kernel/bufint/bufint", O_RDWR);
	write(fd, send_msg, strlen(send_msg));
	lseek(fd, 0, SEEK_SET);
	read(fd, msg, MAX_DIGITS);
	printf("hello? %s\n", msg);
	close(fd);
	return 0;
}
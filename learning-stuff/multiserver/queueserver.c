#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

#define QS_SOCKNUM 3
#define QS_MSGLEN 256

int *sfd;
pthread_t *tid;
//pthread_t *addtid;
char *buf;
struct queue msgqueue;
//pthread_mutex_t spinlock;
pthread_spinlock_t spinlock; //valgrind 3.13 wont lock spinlock?

void finish(int sig)
{
	int i;
	for(i = 0; i<QS_SOCKNUM; i++)
	{
		pthread_cancel(tid[i]);
		pthread_join(tid[i], NULL);
	}
	for(i = 0; i<QS_SOCKNUM+1; i++)
	{
		close(sfd[i]);
	}
	//pthread_mutex_destroy(&spinlock);
	pthread_spin_destroy(&spinlock);
	free(sfd);
	free(tid);
	free(buf);
	exit(0);
}

struct queue_item {
	char *buf;
	struct sockaddr_in client;
	struct queue_item *next;
};

struct queue {
	struct queue_item *head;
	struct queue_item *tail;
};

/*void *addtoqueue(void *vmsg)
{
	struct queue_item *msg = vmsg; 
	printf("RECV: %4s FROM %d\n", msg->buf, msg->client.sin_port);
	pthread_spin_lock(&spinlock);
	if(msgqueue.head == NULL) {
		msgqueue.head = msg;
		msgqueue.tail = msgqueue.head;
		pthread_spin_unlock(&spinlock);
	}
	else {
		msgqueue.tail->next = msg;
		msgqueue.tail = msgqueue.tail->next;
		pthread_spin_unlock(&spinlock);
	}
}*/

void *queuehandler(void *vsocket)
{
	int *socket = vsocket;
	struct queue_item *msg;
	while(1)
	{
		pthread_testcancel();
		//pthread_mutex_lock(&spinlock);
		pthread_spin_lock(&spinlock);
		if(msgqueue.head != NULL)
		{
			msg = msgqueue.head;
			msgqueue.head = msgqueue.head->next;
			//pthread_mutex_unlock(&spinlock);
			pthread_spin_unlock(&spinlock);
			sendto(*socket, msg->buf, QS_MSGLEN, 0, (struct sockaddr*)&msg->client, sizeof(msg->client));
			printf("SENT: %4s  TO  %d\n", msg->buf, msg->client.sin_port);
			free(msg->buf);
			free(msg);
		}
		else
			//pthread_mutex_unlock(&spinlock);
			pthread_spin_unlock(&spinlock);
	}
}

int main()
{
	sigset_t sigmask;
	sigfillset(&sigmask);
	struct sigaction act;
	act.sa_mask = sigmask;
	act.sa_flags = SA_RESTART;
	act.sa_handler = &finish;
	sigaction(SIGINT, &act, NULL);

	struct sockaddr_in sockaddr;
	int i, snum;
	tid = malloc(QS_SOCKNUM * sizeof(pthread_t));
	sfd = malloc((QS_SOCKNUM+1) * sizeof(int));
	buf = malloc(QS_MSGLEN);
	msgqueue.head = NULL;
	msgqueue.tail = NULL;
	//pthread_mutex_init(&spinlock, NULL);
	pthread_spin_init(&spinlock, PTHREAD_PROCESS_SHARED);

	sfd[0] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3110);
	inet_aton("127.0.0.1", &sockaddr.sin_addr);
	bind(sfd[0], (struct sockaddr*)&sockaddr, sizeof(sockaddr));

	for(i=0, snum=1; i<QS_SOCKNUM; i++, snum++)
	{
		sfd[snum] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = 0;
		inet_aton("127.0.0.1", &sockaddr.sin_addr);
		bind(sfd[snum], (struct sockaddr*)&sockaddr, sizeof(sockaddr));
		pthread_create(&tid[i], NULL, queuehandler, &sfd[snum]);
	}
	while(1)
	{
		socklen_t addrlen = sizeof(sockaddr);
		recvfrom(sfd[0], buf, QS_MSGLEN, 0, (struct sockaddr*)&sockaddr, &addrlen);
		struct queue_item *msg;
		msg = malloc(sizeof(struct queue_item));
		msg->buf = malloc(QS_MSGLEN);
		strcpy(msg->buf, buf);
		msg->client = sockaddr;
		msg->next = NULL;
		printf("RECV: %4s FROM %d\n", msg->buf, msg->client.sin_port);
		//pthread_mutex_lock(&spinlock);
		pthread_spin_lock(&spinlock);
		printf("RECV: %4s FROM %d\n", msg->buf, msg->client.sin_port);
		if(msgqueue.head == NULL) {
			msgqueue.head = msg;
			msgqueue.tail = msgqueue.head;
			//pthread_mutex_unlock(&spinlock);
			pthread_spin_unlock(&spinlock);
		}
		else {
			msgqueue.tail->next = msg;
			msgqueue.tail = msgqueue.tail->next;
			//pthread_mutex_unlock(&spinlock);
			pthread_spin_unlock(&spinlock);
		}
	}
}
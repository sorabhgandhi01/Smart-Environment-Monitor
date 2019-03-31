#ifndef MAIN_TASK_H
#define MAIN_TASK_H

/* Standard C Library Headers */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <mqueue.h>
#include <stdarg.h>
#include <time.h>

#define BUILD_MESSAGE(buffer, format, ...) \
do{ \
sprintf(buffer, "[PID:%d][TID:%ld][Timestamp = %lu]" format, getpid(), syscall(SYS_gettid), time(NULL), ##__VA_ARGS__); \
}while(0)

#define LOG_PRINT(format, ...) \
do{ \
	printf("[PID:%d][TID:%ld]", getpid(), syscall(SYS_gettid)); \
	printf(format, ##__VA_ARGS__); \
	fflush(stdout); \
}while(0)

/* Macros */
#define QUEUE_NAME					"/my_queue"
#define SOCKET_QUEUE_NAME			"/my_queue_2"
#define SIZE_OF_QUEUE		10
#define QUEUE_PERMISSIONS	0666

/* Log File Pointer */
FILE *fptr;



/* Mutex lock declaration */
pthread_mutex_t lock;

/* Global variables for queue */
mqd_t logger_queue;
struct mq_attr queue_attr;

mqd_t socket_queue;
struct mq_attr socket_queue_attr;


/* Buffer for storing queue data */
char buffer[50];

#endif
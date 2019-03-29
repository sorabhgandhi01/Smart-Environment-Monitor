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


/* Macros */
#define QUEUE_NAME			"/my_queue"
#define SIZE_OF_QUEUE		10
#define QUEUE_PERMISSIONS	0666

/* Log File Pointer */
FILE *fptr;



/* Mutex lock declaration */
pthread_mutex_t lock;

/* Global variables for queue */
mqd_t logger_queue;
struct mq_attr queue_attr;



/* Buffer for storing queue data */
char buffer[50];


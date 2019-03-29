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

FILE *fptr;

/* Pthreads */
pthread_t light_thread;
pthread_t temp_thread;
pthread_t logger_thread;
pthread_t socket_thread;

/* Mutex lock declaration */
pthread_mutex_t lock;


/* Global variables for queue */
mqd_t logger_queue;
struct mq_attr queue_attr;

int flag;

/* Posix timer variables */
static timer_t light_timerid;
static timer_t temp_timerid;
static timer_t main_timerid;
static timer_t logger_timerid;
static timer_t socket_timerid;

/* Thread Alive/Dead Counter check structure */
typedef struct{
	int temp_count;
	int light_count;
	int logger_count;
	int socket_count;
}thread_status_tracker_t;


/* Create structure instances */
thread_status_tracker_t current = {0};
thread_status_tracker_t prev = {0};


/* Thread IDs */
pid_t light_tid;
pid_t temp_tid;
pid_t logger_tid;
pid_t socket_tid;

/* Buffer for storing queue data */
char buffer[50];

char *proj1 = "/tmp/proj1";

/* Function Prototypes */
void *light_thread_handler();
void *temp_thread_handler();
void *logger_thread_handler();
void *socket_thread_handler();
void *main_thread_handler();


/* Function Prototypes for timer handler */
void light_timer_handler(void);
void temp_timer_handler(void);
void logger_timer_handler(void);
void socket_timer_handler(void);



/* Light Timer handler */
void light_timer_handler(void)
{
	char buffer[50];
	pthread_mutex_lock(&lock);
	
	printf("LIGHT TIMER HANDLER\n");

	int fd = open(proj1,O_WRONLY);
	sprintf(buffer,"LIGHT THREAD DATA\nTID:%ld\n",syscall(SYS_gettid));

	mq_send(logger_queue,buffer,50,0);

	write(fd,"L",1);

	close(fd); 
	
	pthread_mutex_unlock(&lock);	
}


/* Temperature Timer Handler */
void temp_timer_handler(void)
{
	char buffer[50];
	pthread_mutex_lock(&lock);
	
	printf("TEMPERATURE TIMER HANDLER\n");
	
	int fd = open(proj1,O_WRONLY);

	sprintf(buffer,"TEMP THREAD DATA\nTID:%ld\n",syscall(SYS_gettid));

	mq_send(logger_queue,buffer,50,0);

	write(fd,"T",1);

	close(fd);
	
	pthread_mutex_unlock(&lock);

	
}

/* Logger Timer Handler */
void logger_timer_handler(void)
{
	char buffer[50];
	//pthread_mutex_lock(&lock);

	printf("LOGGER TIMER HANDLER\n");
	
	int fd = open(proj1,O_WRONLY);

	sprintf(buffer,"LOGGER THREAD DATA\nTID:%ld\n",syscall(SYS_gettid));

	mq_send(logger_queue,buffer,50,0);

	write(fd,"O",1);

	close(fd);
	
	//pthread_mutex_unlock(&lock);
}


/* Socket Timer Handler */
void socket_timer_handler(void)
{
	char buffer[50];
	pthread_mutex_lock(&lock);
	
	printf("SOCKET TIMER HANDLER\n");
	
	int fd = open(proj1,O_WRONLY);

	sprintf(buffer,"SOCKET THREAD DATA\nTID:%ld\n",syscall(SYS_gettid));

	mq_send(logger_queue,buffer,50,0);


	write(fd,"S",1);

	close(fd);
	
	pthread_mutex_unlock(&lock);
}

void *light_thread_handler()
{
	
	char light_buffer[50];
	char light_info[]="Taking Light Reading......\n";

	struct sigevent sev;
	struct timespec mainTimeSpec;
	struct itimerspec trigger;

	memset(&sev,0,sizeof(struct sigevent));
	memset(&trigger,0,sizeof(struct itimerspec));

	/* 
     * Set the notification method as SIGEV_THREAD:
     *
     * Upon timer expiration, `sigev_notify_function` (thread_handler()),
     * will be invoked as if it were the start function of a new thread.
     *
     */
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = &light_timer_handler;
	sev.sigev_value.sival_ptr = &light_info;

	 /*
    * Create the timer. In this example, CLOCK_REALTIME is used as the
    * clock, meaning that we're using a system-wide real-time clock for 
    * this timer.
    */
	timer_create(CLOCK_REALTIME, &sev, &light_timerid);

	/* Timer expiration will occur withing 2 seconds after being armed
     * by timer_settime(). Then the interval timer will takeover 
     */
    trigger.it_value.tv_sec = 2;

	/* Uncomment the following line to set the interval timer and
	 * and see the threadhandler() execute periodically.
	 */
    trigger.it_interval.tv_sec = 2;

	timer_settime(light_timerid,0,&trigger,NULL);

	

	 pid_t light_tid = syscall(SYS_gettid);	//Get thread id
	 printf("LIGHT TID:%d\n",light_tid);
	 // pid_t light_tid = getpid();	//Get thread id
	 // printf("LIGHT TID:%d\n",light_tid);



	//pthread_cancel(pthread_self());
	//timer_delete(light_timerid);


}

void *temp_thread_handler()
{

	char temp_buffer[50];
	char temp_info[]="Taking Temperature Reading......\n";

	struct sigevent temp_sev;
	struct timespec temp_mainTimeSpec;
	struct itimerspec temp_trigger;

	memset(&temp_sev,0,sizeof(struct sigevent));
	memset(&temp_trigger,0,sizeof(struct itimerspec));

	/* 
     * Set the notification method as SIGEV_THREAD:
     *
     * Upon timer expiration, `sigev_notify_function` (thread_handler()),
     * will be invoked as if it were the start function of a new thread.
     *
     */
    temp_sev.sigev_notify = SIGEV_THREAD;
    temp_sev.sigev_notify_function = &temp_timer_handler;
	temp_sev.sigev_value.sival_ptr = &temp_info;

	 /*
    * Create the timer. In this example, CLOCK_REALTIME is used as the
    * clock, meaning that we're using a system-wide real-time clock for 
    * this timer.
    */
	timer_create(CLOCK_REALTIME, &temp_sev, &temp_timerid);

	/* Timer expiration will occur withing 2 seconds after being armed
     * by timer_settime(). Then the interval timer will takeover 
     */
    temp_trigger.it_value.tv_sec = 2;

	/* Uncomment the following line to set the interval timer and
	 * and see the threadhandler() execute periodically.
	 */
    temp_trigger.it_interval.tv_sec = 2;

    timer_settime(temp_timerid,0,&temp_trigger,NULL);

}

void *logger_thread_handler()
{

	char buffer[50];
	char logger_info[]="Logging Data......\n";


	//while(1){
	struct sigevent logger_sev;
	struct timespec logger_mainTimeSpec;
	struct itimerspec logger_trigger;

	memset(&logger_sev,0,sizeof(struct sigevent));
	memset(&logger_trigger,0,sizeof(struct itimerspec));

	/* 
     * Set the notification method as SIGEV_THREAD:
     *
     * Upon timer expiration, `sigev_notify_function` (thread_handler()),
     * will be invoked as if it were the start function of a new thread.
     *
     */
    logger_sev.sigev_notify = SIGEV_THREAD;
    logger_sev.sigev_notify_function = &logger_timer_handler;
	logger_sev.sigev_value.sival_ptr = &logger_info;

	 /*
    * Create the timer. In this example, CLOCK_REALTIME is used as the
    * clock, meaning that we're using a system-wide real-time clock for 
    * this timer.
    */
	timer_create(CLOCK_REALTIME, &logger_sev, &logger_timerid);

	/* Timer expiration will occur withing 2 seconds after being armed
     * by timer_settime(). Then the interval timer will takeover 
     */
    logger_trigger.it_value.tv_sec = 2;

	/* Uncomment the following line to set the interval timer and
	 * and see the threadhandler() execute periodically.
	 */
    logger_trigger.it_interval.tv_sec = 2;

    timer_settime(logger_timerid,0,&logger_trigger,NULL);


    while(1)
    {
    	fptr = fopen("log.txt","a");
		mq_receive(logger_queue,buffer,50,0);
		fprintf(fptr,"%s\n",buffer);
		fclose(fptr);

    }

	// pid_t logger_tid = syscall(SYS_gettid);	//Get thread id	
	// printf("LOGGER TID:%d\n",logger_tid);
}

void *socket_thread_handler()
{
	char socket_buffer[50];
	char socket_info[]="Socket Thread Alive......\n";


	struct sigevent socket_sev;
	struct timespec socket_mainTimeSpec;
	struct itimerspec socket_trigger;

	memset(&socket_sev,0,sizeof(struct sigevent));
	memset(&socket_trigger,0,sizeof(struct itimerspec));

	/* 
     * Set the notification method as SIGEV_THREAD:
     *
     * Upon timer expiration, `sigev_notify_function` (thread_handler()),
     * will be invoked as if it were the start function of a new thread.
     *
     */
    socket_sev.sigev_notify = SIGEV_THREAD;
    socket_sev.sigev_notify_function = &socket_timer_handler;
	socket_sev.sigev_value.sival_ptr = &socket_info;

	 /*
    * Create the timer. In this example, CLOCK_REALTIME is used as the
    * clock, meaning that we're using a system-wide real-time clock for 
    * this timer.
    */
	timer_create(CLOCK_REALTIME, &socket_sev, &socket_timerid);

	/* Timer expiration will occur withing 2 seconds after being armed
     * by timer_settime(). Then the interval timer will takeover 
     */
   	socket_trigger.it_value.tv_sec = 2;

	/* Uncomment the following line to set the interval timer and
	 * and see the threadhandler() execute periodically.
	 */	
    socket_trigger.it_interval.tv_sec = 2;

    timer_settime(socket_timerid,0,&socket_trigger,NULL);

}


void *main_thread_handler(void)
{
	if(current.temp_count <= prev.temp_count)
	{
		printf("\nTemperature Thread : DEAD\n");
	}
	else
	{
		printf("\nTemperature Thread : ALIVE\n");
	}

	if(current.light_count <= prev.light_count)
	{
		printf("\nLight Thread : DEAD\n");
	}
	else
	{
		printf("\nLight Thread : ALIVE\n");		
	}

	if(current.logger_count <= prev.logger_count)
	{
		printf("\nLogger Thread : DEAD\n");
	}
	else
	{
		printf("\nLogger Thread : ALIVE\n");		
	}

	if(current.socket_count <= prev.socket_count)
	{
		printf("\nSocket Thread : DEAD\n");
	}
	else
	{
		printf("\nSocket Thread : ALIVE\n");		
	}


	prev.temp_count 	= current.temp_count;
	prev.light_count 	= current.light_count;
	prev.logger_count 	= current.logger_count;
	prev.socket_count 	= current.socket_count;
}



/* Main Thread */
int main(int argc, char *argv[])
{
	printf("Initiating the Project!\n");

	/* Create Named Pipe */
	mkfifo(proj1,QUEUE_PERMISSIONS);


	if(pthread_mutex_init(&lock,NULL) !=0)
	{
		printf("Mutex Initialization Failed!\n");
		exit(0);
	}

	// mqd_t logger_queue;
	// struct mq_attr queue_attr;
	queue_attr.mq_maxmsg = SIZE_OF_QUEUE;
	queue_attr.mq_msgsize = 50;

	logger_queue = mq_open(QUEUE_NAME,O_CREAT | O_RDWR, QUEUE_PERMISSIONS,&queue_attr);

	if(logger_queue == (mqd_t)-1)
	{
		perror("Failed to Open Queue");
		exit(0);
	}

	/* Create Light Thread */
	pthread_create(&light_thread,NULL,light_thread_handler,(void *)NULL);

	/* Create Temperature Thread */
	pthread_create(&temp_thread,NULL,temp_thread_handler,(void *)NULL);

	/* Create Logger Thread */
	pthread_create(&logger_thread,NULL,logger_thread_handler,(void *)NULL);

	/* Create Socket Thread */
	pthread_create(&socket_thread,NULL,socket_thread_handler,(void *)NULL);

	// int fd = open(proj1,O_RDONLY); 

	char heartbeat[1];

	ssize_t read_status;


	struct sigevent main_sev;
	struct timespec main_mainTimeSpec;
	struct itimerspec main_trigger;

	memset(&main_sev,0,sizeof(struct sigevent));
	memset(&main_trigger,0,sizeof(struct itimerspec));

	/* 
     * Set the notification method as SIGEV_THREAD:
     *
     * Upon timer expiration, `sigev_notify_function` (thread_handler()),
     * will be invoked as if it were the start function of a new thread.
     *
     */
    main_sev.sigev_notify = SIGEV_THREAD;
    main_sev.sigev_notify_function = &main_thread_handler;
	main_sev.sigev_value.sival_ptr = &main_timerid;

	 /*
    * Create the timer. In this example, CLOCK_REALTIME is used as the
    * clock, meaning that we're using a system-wide real-time clock for 
    * this timer.
    */
	timer_create(CLOCK_REALTIME, &main_sev, &main_timerid);

	/* Timer expiration will occur withing 2 seconds after being armed
     * by timer_settime(). Then the interval timer will takeover 
     */
    main_trigger.it_value.tv_sec = 5;

	/* Uncomment the following line to set the interval timer and
	 * and see the threadhandler() execute periodically.
	 */
    main_trigger.it_interval.tv_sec = 5;

    timer_settime(main_timerid,0,&main_trigger,NULL);


	while(1)
	{
		

		int fd = open(proj1,O_RDONLY); 

		memset(heartbeat,0,1);

		read(fd,heartbeat,1);

		if(!strcmp(heartbeat,"L"))
		{
			printf("Hearbeat from LIGHT\n");
			(current.light_count)++;
			printf("Light_check = %d\n",current.light_count);
		}

		if(!strcmp(heartbeat,"T"))
		{
			printf("Hearbeat from TEMPERATURE\n");
			(current.temp_count)++;
			printf("Temp_check = %d\n",current.temp_count);
		}

		if(!strcmp(heartbeat,"O"))
		{
			printf("Hearbeat from LOGGER\n");
			(current.logger_count)++;
			printf("Logger_check = %d\n",current.logger_count);
		}

		if(!strcmp(heartbeat,"S"))
		{
			printf("Hearbeat from SOCKET\n");
			(current.socket_count)++;
			printf("Socket_check = %d\n",current.socket_count);
		}


		//close(fd);

		//printf("%s\n",heartbeat);
	

	}

	pthread_join(light_thread,NULL);
	pthread_join(temp_thread,NULL);
	pthread_join(logger_thread,NULL);
	pthread_join(socket_thread,NULL);

	pthread_mutex_destroy(&lock);




	return 0;
}



//create 4 threads (temp,lux,socket,logger) which will send periodic 
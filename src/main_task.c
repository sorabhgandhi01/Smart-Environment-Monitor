/*@Filename	: main_task.c
 * @Author	: Om Raheja & Sorabh Gandhi
 * @Course	: [PROJECT 1]Advanced Embedded Software Development Spring 2019
 * @Date	: 31st March 2019
 * @References	: https://www.softprayog.in/programming/interprocess-communication-using-posix-message-queues-in-linux
 * @brief	: Creates all the threads(light,temperature,socket,logger) and checks if all the threads are alive/dead.
 * 		  If this thread is terminates, it terminates the program after releasing all the resources,
 * 		  closing the I2C bus, Closing the message queue and unlinking it. This task is also responsible
 * 		  for logging the status of all other threads (alive/dead) to the log file.
 * */


/******************************
 * USER DEFINED HEADER FILES  *
 * ****************************/
#include "main_task.h"
#include "temp_task.h"
#include "light_task.h"
#include "socket_task.h"
#include "logger_task.h"
#include "i2c_helper.h"
#include "led.h"


/******************************
 * POSIX TIMER VARIABLES      *
 * ****************************/
static timer_t main_timerid;


/* Thread Alive/Dead Counter check structure */
typedef struct{
	int temp_count;
	int light_count;
	int logger_count;
	int socket_count;
}thread_status_tracker_t;


/******************************
 * STRUCTURE INSTANCES	      *
 * ****************************/
thread_status_tracker_t current = {0};
thread_status_tracker_t prev = {0};


/******************************
 * NAMED PIPE FILE PATH       *
 * ****************************/
char *proj1 = "/tmp/proj1";


/******************************
 * FUNCTION PROTOTYPES *
 * ****************************/
void main_thread_handler();
void main_signal_handler(int signo, siginfo_t *info,void *extra);
void set_main_signal_handler(void);


/********************************
 * SIGNAL HANDLER FOR MAIN TASK *
 * ******************************/
void main_signal_handler(int signo, siginfo_t *info,void *extra)
{
	printf("\nKilling MAIN THREAD\n");
	i2c_close();			//close i2c bus
	timer_delete(main_timerid);	//delete timer
	mq_close(logger_queue);		//close logger queue
	mq_unlink(QUEUE_NAME);		//unlink logger queue
	exit(0);
}


/************************************
 * SET SIGNAL HANDLER FOR MAIN TASK *
 * * ********************************/
void set_main_signal_handler(void)
{
	struct sigaction action;
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = main_signal_handler;

	if(sigaction(SIGALRM,&action,NULL) == -1)
	{
		perror("Sigusr : Sigaction");
		_exit(1);
	}
}


/******************************
 * MAIN THREAD HANDLER        *
 * ****************************/
void main_thread_handler(union sigval val)
{
	char buffer[LOGGER_QUEUE_SIZE];

	if(current.temp_count <= prev.temp_count)
	{
		LOG_PRINT("[MAIN TASK][ERROR] TEMPERATURE TASK DEAD\n");
		TEMP_ERROR_LED_ON();
		BUILD_MESSAGE(buffer, "[MAIN TASK][ERROR] TEMPERATURE TASK DEAD");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
		memset(buffer, 0, sizeof(buffer));
	}
	else
	{
		LOG_PRINT("[MAIN TASK]\t[DEBUG] TEMPERATURE TASK ALIVE\n");
		TEMP_ERROR_LED_OFF();
		BUILD_MESSAGE(buffer, "[MAIN TASK][DEBUG] TEMPERATURE TASK ALIVE");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
		memset(buffer, 0, sizeof(buffer));
	}

	if(current.light_count <= prev.light_count)
	{
		LOG_PRINT("[MAIN TASK][ERROR] LIGHT TASK DEAD\n");
		LIGHT_ERROR_LED_ON();
		BUILD_MESSAGE(buffer, "[MAIN TASK][ERROR] LIGHT TASK DEAD");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
		memset(buffer, 0, sizeof(buffer));
	}
	else
	{
		LOG_PRINT("[MAIN TASK]\t[DEBUG] LIGHT TASK ALIVE\n");
		LIGHT_ERROR_LED_OFF();
		BUILD_MESSAGE(buffer, "[MAIN TASK][DEBUG] LIGHT TASK ALIVE");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
		memset(buffer, 0, sizeof(buffer));		
	}

	if(current.logger_count <= prev.logger_count)
	{
		LOG_PRINT("[MIAN TASK][ERROR] LOGGER TASK DEAD\n");
		LOGGER_ERROR_LED_ON();
		BUILD_MESSAGE(buffer, "[MAIN TASK][ERROR] LOGGER TASK DEAD");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
		memset(buffer, 0, sizeof(buffer));
	}
	else
	{
		LOG_PRINT("[MAIN TASK]\t[DEBUG] LOGGER TASK ALIVE\n");
		LOGGER_ERROR_LED_OFF();
		BUILD_MESSAGE(buffer, "[MAIN TASK][DEBUG] LOGGER TASK ALIVE");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
		memset(buffer, 0, sizeof(buffer));
	}

	if(current.socket_count <= prev.socket_count)
	{
		LOG_PRINT("[Main TASK][ERROR] SOCKET TASK DEAD\n");
		SOCKET_ERROR_LED_ON();
		BUILD_MESSAGE(buffer, "[MAIN TASK][ERROR] SOCKET TASK DEAD");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
		memset(buffer, 0, sizeof(buffer));
	}
	else
	{
		LOG_PRINT("[MAIN TASK]\t[DEBUG] SOCKET TASK ALIVE\n");
		SOCKET_ERROR_LED_OFF();
		BUILD_MESSAGE(buffer, "[MAIN TASK][DEBUG] SOCKET TASK ALIVE");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
		memset(buffer, 0, sizeof(buffer));	
	}


	prev.temp_count 	= current.temp_count;
	prev.light_count 	= current.light_count;
	prev.logger_count 	= current.logger_count;
	prev.socket_count 	= current.socket_count;
}



/******************************
 * MAIN THREAD	              *
 * ****************************/
int main(int argc, char *argv[])
{
	/*check for appropriate commandline arguments*/
	if ((argc < 2) || (argc > 2)) {				
		printf("Usage --> ./[%s] [LOG FILENAME]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	LOG_PRINT("[MAIN TASK]\t [DEBUG] Initiating the project\n");	

	char buffer[LOGGER_QUEUE_SIZE];

	/* Create Named Pipe */
	mkfifo(proj1, QUEUE_PERMISSIONS); //heartbeat signals

	/* Mutex Initialization */
	if(pthread_mutex_init(&lock, NULL) !=0)
	{
		LOG_PRINT("[MAIN TASK]\t [ERROR] Mutex Initialization Failed\n");
		exit(0);
	}

	/* Populating Message queue structure */
	queue_attr.mq_maxmsg = SIZE_OF_QUEUE;
	queue_attr.mq_msgsize = LOGGER_QUEUE_SIZE;

	/* Populating Message queue structure [Socket Task] */
	socket_queue_attr.mq_maxmsg = SIZE_OF_QUEUE;
	socket_queue_attr.mq_msgsize = 50;

	/* Open Message queue */
	logger_queue = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, QUEUE_PERMISSIONS, &queue_attr);

	/* Open Message queue [Socket Task] */
	socket_queue = mq_open(SOCKET_QUEUE_NAME, O_CREAT | O_RDWR, QUEUE_PERMISSIONS, &socket_queue_attr);

	if(logger_queue == (mqd_t)-1)
	{
		perror("Failed to Open Queue");
		exit(0);
	}

	if(socket_queue == (mqd_t)-1)
	{
		perror("Failed to Open Queue");
		exit(0);
	}

	/* Set signal handlers for all threads/tasks (light,logger,temperature,socket,main) */
	set_light_signal_handler();
	set_logger_signal_handler();
	set_temp_signal_handler();
	set_socket_signal_handler();
	set_main_signal_handler();

	/* Create Logger Thread */
	if ((pthread_create(&logger_thread,NULL,logger_thread_handler,(void *)argv[1]) || \
		pthread_create(&light_thread,NULL,light_thread_handler,(void *)NULL) || \
		pthread_create(&temp_thread,NULL,temp_thread_handler,(void *)NULL) || \
		pthread_create(&socket_thread,NULL,socket_thread_handler,(void *)NULL)) == 0) {

		BUILD_MESSAGE(buffer, "[MAIN TASK][INFO] BIST for THREAD CREATION PASSED");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
	}
	else {
		BUILD_MESSAGE(buffer, "[MAIN TASK][ERROR] BIST for THREAD CREATION FAILED");
		mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
	}

	/* Character to store heartbeat message from each task */
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
		/* Open Named pipe for read only operation */	
		int fd = open(proj1,O_RDONLY); 

		/* Clear Heartbeat buffer */
		memset(heartbeat,0,1);

		/* Read the data(heartbeat) from respective threads */
		read(fd,heartbeat,1);

		/* Check heartbeat received from each thread */
		if(!strcmp(heartbeat,"L"))
		{
			LOG_PRINT("[MAIN TASK]\t [DEBUG] Recieved HEARTBEAT from 'LIGHT TASK'\n");
			(current.light_count)++;
			LOG_PRINT("[MAIN TASK]\t [INFO] Number of HEARTBEAT signals recieved from 'LIGHT TASK' = %d\n", current.light_count);
		}

		if(!strcmp(heartbeat,"T"))
		{
			LOG_PRINT("[MAIN TASK]\t [DEBUG] Recieved HEARTBEAT from 'TEMPERATURE TASK'\n");
			(current.temp_count)++;
			LOG_PRINT("[MAIN TASK]\t [INFO] Number of HEARTBEAT signals recieved from 'TEMPERATURE TASK' = %d\n", current.temp_count);
		}

		if(!strcmp(heartbeat,"O"))
		{
			LOG_PRINT("[MAIN TASK]\t [DEBUG] Recieved HEARTBEAT from 'LOGGER TASK'\n");
			(current.logger_count)++;
			LOG_PRINT("[MAIN TASK]\t [INFO] Number of HEARTBEAT signals recieved from 'LOGGER TASK' = %d\n", current.logger_count);
		}

		if(!strcmp(heartbeat,"S"))
		{
			LOG_PRINT("[MAIN TASK]\t [DEBUG] Recieved HEARTBEAT from 'SOCKET TASK'\n");
			(current.socket_count)++;
			LOG_PRINT("[MAIN TASK]\t [INFO] Number of HEARTBEAT signals recieved from 'SOCKET TASK' = %d\n", current.socket_count);
		}
	}

	/* Wait for child threads to terminate */
	pthread_join(light_thread,NULL);
	pthread_join(temp_thread,NULL);
	pthread_join(logger_thread,NULL);
	pthread_join(socket_thread,NULL);

	/* Destroys the mutex object */
	pthread_mutex_destroy(&lock);

	return 0;
}

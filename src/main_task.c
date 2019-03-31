/* Standard C Library Headers */
#include "main_task.h"
#include "temp_task.h"
#include "light_task.h"
#include "socket_task.h"
#include "logger_task.h"

/* Posix timer variables */
static timer_t main_timerid;

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


char *proj1 = "/tmp/proj1";

/* Function Prototypes */
void main_thread_handler();

/* Main Thread Handler */
void main_thread_handler(union sigval val)
{
	if(current.temp_count <= prev.temp_count)
	{
		LOG_PRINT("[TEMPERATURE TASK][ERROR] DEAD\n");

	}
	else
	{
		LOG_PRINT("[TEMPERATURE TASK]\t[DEBUG] ALIVE\n");
	}

	if(current.light_count <= prev.light_count)
	{
		LOG_PRINT("[LIGHT TASK][ERROR] DEAD\n");
	}
	else
	{
		LOG_PRINT("[LIGHT TASK]\t[DEBUG] ALIVE\n");		
	}

	if(current.logger_count <= prev.logger_count)
	{
		LOG_PRINT("[LOGGER TASK][ERROR] DEAD\n");
	}
	else
	{
		LOG_PRINT("[LOGGER TASK]\t[DEBUG] ALIVE\n");		
	}

	if(current.socket_count <= prev.socket_count)
	{
		LOG_PRINT("[SOCKET TASK][ERROR] DEAD\n");
	}
	else
	{
		LOG_PRINT("[SOCKET TASK]\t[DEBUG] ALIVE\n");		
	}


	prev.temp_count 	= current.temp_count;
	prev.light_count 	= current.light_count;
	prev.logger_count 	= current.logger_count;
	prev.socket_count 	= current.socket_count;
}



/* Main Thread */
int main(int argc, char *argv[])
{
	/*check for appropriate commandline arguments*/
	if ((argc < 2) || (argc > 2)) {				
		printf("Usage --> ./[%s] [LOG FILENAME]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//printf("Initiating the Project!\n");
	LOG_PRINT("[MAIN TASK]\t [DEBUG] Initiating the project\n");	

	/* Create Named Pipe */
	mkfifo(proj1,QUEUE_PERMISSIONS); //heartbeat signals

	/* Mutex Initialization */
	if(pthread_mutex_init(&lock,NULL) !=0)
	{
		//printf("Mutex Initialization Failed!\n");
		LOG_PRINT("[MAIN TASK]\t [ERROR] Mutex Initialization Failed\n");
		exit(0);
	}

	/* Populating Message queue structure */
	queue_attr.mq_maxmsg = SIZE_OF_QUEUE;
	queue_attr.mq_msgsize = 256;

	/* Populating Message queue structure [Socket Task] */
	socket_queue_attr.mq_maxmsg = SIZE_OF_QUEUE;
	socket_queue_attr.mq_msgsize = 50;

	/* Open Message queue */
	logger_queue = mq_open(QUEUE_NAME,O_CREAT | O_RDWR, QUEUE_PERMISSIONS,&queue_attr);

	/* Open Message queue [Socket Task] */
	socket_queue = mq_open(SOCKET_QUEUE_NAME,O_CREAT | O_RDWR, QUEUE_PERMISSIONS,&socket_queue_attr);

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

	/* Create Logger Thread */
	pthread_create(&logger_thread,NULL,logger_thread_handler,(void *)argv[1]);

	/* Create Light Thread */
	pthread_create(&light_thread,NULL,light_thread_handler,(void *)NULL);

	/* Create Temperature Thread */
	pthread_create(&temp_thread,NULL,temp_thread_handler,(void *)NULL);

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
			//printf("Hearbeat from LIGHT\n");
			LOG_PRINT("[MAIN TASK]\t [DEBUG] Recieved HEARTBEAT from 'LIGHT TASK'\n");
			(current.light_count)++;
			//printf("Light_check = %d\n",current.light_count);
			LOG_PRINT("[MAIN TASK]\t [INFO] Number of HEARTBEAT signals recieved from 'LIGHT TASK' = %d\n", current.light_count);
		}

		if(!strcmp(heartbeat,"T"))
		{
			//printf("Hearbeat from TEMPERATURE\n");
			LOG_PRINT("[MAIN TASK]\t [DEBUG] Recieved HEARTBEAT from 'TEMPERATURE TASK'\n");
			(current.temp_count)++;
			//printf("Temp_check = %d\n",current.temp_count);
			LOG_PRINT("[MAIN TASK]\t [INFO] Number of HEARTBEAT signals recieved from 'TEMPERATURE TASK' = %d\n", current.temp_count);
		}

		if(!strcmp(heartbeat,"O"))
		{
			//printf("Hearbeat from LOGGER\n");
			LOG_PRINT("[MAIN TASK]\t [DEBUG] Recieved HEARTBEAT from 'LOGGER TASK'\n");
			(current.logger_count)++;
			//printf("Logger_check = %d\n",current.logger_count);
			LOG_PRINT("[MAIN TASK]\t [INFO] Number of HEARTBEAT signals recieved from 'LOGGER TASK' = %d\n", current.logger_count);
		}

		if(!strcmp(heartbeat,"S"))
		{
			//printf("Hearbeat from SOCKET\n");
			LOG_PRINT("[MAIN TASK]\t [DEBUG] Recieved HEARTBEAT from 'SOCKET TASK'\n");
			(current.socket_count)++;
			//printf("Socket_check = %d\n",current.socket_count);
			LOG_PRINT("[MAIN TASK]\t [INFO] Number of HEARTBEAT signals recieved from 'SOCKET TASK' = %d\n", current.socket_count);
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
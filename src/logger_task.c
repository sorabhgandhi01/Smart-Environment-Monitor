/*@Filename     : logger_task.c
 * @Author      : Om Raheja & Sorabh Gandhi
 * @Course      : [PROJECT 1]Advanced Embedded Software Development Spring 2019
 * @Date        : 31st March 2019
 * @brief       : Logger task is responsible to log all the Error/Info/Debug messages
 * 		  from all threads in a log file.
 * */


/******************************
 * USER DEFINED HEADER FILES  *
 * ****************************/
#include "main_task.h"
#include "logger_task.h"
#include "temp_task.h"
#include "light_task.h"
#include "socket_task.h"


/******************************
 * FILE PATH FOR NAMED PIPE   *
 * ****************************/
char *proj5 = "/tmp/proj1";


/***********************************
 * SIGNAL HANDLER FOR LOGGER TASK  *
 * *********************************/
void logger_signal_handler(int signo, siginfo_t *info,void *extra)
{
	printf("\nKilling LOGGER THREAD\n");
	
	timer_delete(logger_timerid);
	pthread_cancel(logger_thread);
	kill(getpid(),SIGKILL);
	
}

/******************************
 * SET LOGGER SIGNAL HANDLER  *
 * ****************************/
void set_logger_signal_handler(void)
{
	struct sigaction action;
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = logger_signal_handler;

	if(sigaction(SIGQUIT,&action,NULL) == -1)
	{
		perror("Sigusr : Sigaction");
		_exit(1);
	}
}


/**********************************
 * TIMER HANDLER FOR LOGGER TASK  *
 * ********************************/
void logger_timer_handler(union sigval val)
{
	//char buffer[50];
	//pthread_mutex_lock(&lock);

	LOG_PRINT("[LOGGER TASK]\t [DEBUG] Invoking timer handler\n");
	
	int fd = open(proj5,O_WRONLY);

	write(fd,"O",1);

	close(fd);
	
	//pthread_mutex_unlock(&lock);
}


/******************************
 * LOGGER THREAD HANDLER      *
 * ****************************/
void *logger_thread_handler(void *arg)
{

	char buffer[LOGGER_QUEUE_SIZE];
	char logger_info[]="Logging Data......\n";

	file_name = (char *)arg;

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

    	timer_settime(logger_timerid, 0, &logger_trigger, NULL);

    	fptr = fopen(file_name, "r+");
	
	/* Remove file if it already exists */
    	if (fptr)
	{
    		fclose(fptr);
    		remove(file_name);
    	}

    	while(1)
    	{
		mq_receive(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);
		fptr = fopen(file_name, "a");
		LOG_TO_FILE(fptr, "%s\n", buffer);
		fclose(fptr);

		memset(buffer, 0, sizeof(buffer));
    	}
}

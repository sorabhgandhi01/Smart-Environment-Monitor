#include "main_task.h"
#include "logger_task.h"

char *proj5 = "/tmp/proj1";

/* Logger Timer Handler */
void logger_timer_handler(union sigval val)
{
	//char buffer[50];
	//pthread_mutex_lock(&lock);

	//printf("LOGGER TIMER HANDLER\n");
	LOG_PRINT("[LOGGER TASK]\t [DEBUG] Invoking timer handler");
	
	int fd = open(proj5,O_WRONLY);

	//sprintf(buffer,"LOGGER THREAD DATA\nTID:%ld\n",syscall(SYS_gettid));

	//mq_send(logger_queue,buffer,50,0);

	write(fd,"O",1);

	close(fd);
	
	//pthread_mutex_unlock(&lock);
}



void *logger_thread_handler()
{

	char buffer[256];
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
		mq_receive(logger_queue,buffer,256,0);
		//fprintf(fptr,"%s\n",buffer);
		fptr = fopen("log.txt","a");
		LOG_TO_FILE(fptr, "%s\n", buffer);
		fclose(fptr);

		memset(buffer, 0, sizeof(buffer));
    }

	// pid_t logger_tid = syscall(SYS_gettid);	//Get thread id	
	// printf("LOGGER TID:%d\n",logger_tid);
}

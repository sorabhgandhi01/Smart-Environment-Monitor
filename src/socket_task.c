#include "main_task.h"
#include "socket_task.h"


char *proj4 = "/tmp/proj1";


/* Socket Timer Handler */
void socket_timer_handler(void)
{
	char buffer[50];
	pthread_mutex_lock(&lock);
	
	printf("SOCKET TIMER HANDLER\n");
	
	int fd = open(proj4,O_WRONLY);

	sprintf(buffer,"SOCKET THREAD DATA\nTID:%ld\n",syscall(SYS_gettid));

	mq_send(logger_queue,buffer,50,0);


	write(fd,"S",1);

	close(fd);
	
	pthread_mutex_unlock(&lock);
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
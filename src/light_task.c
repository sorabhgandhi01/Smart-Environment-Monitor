#include "main_task.h"
#include "light_task.h"
#include "light_sensor.h"

char *proj3 = "/tmp/proj1";
extern int SOCKET;

void light_signal_handler(int signo, siginfo_t *info,void *extra)
{
	printf("\nKilling LIGHT THREAD\n");

	sensor_disable();
	// i2c_close();
	timer_delete(light_timerid);
	pthread_cancel(light_thread);
	
	
}


void set_light_signal_handler(void)
{
	struct sigaction action;
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = light_signal_handler;

	if(sigaction(SIGUSR2,&action,NULL) == -1)
	{
		perror("Sigusr : Sigaction");
		_exit(1);
	}
}

/* Light Timer handler */
void light_timer_handler(union sigval val)
{
	char buffer[LOGGER_QUEUE_SIZE];
	char socket_buffer[50];
	float data;

	pthread_mutex_lock(&lock);
	
	//printf("LIGHT TIMER HANDLER\n");
	LOG_PRINT("[LIGHT TASK]\t [DEBUG] Invoking timer handler\n");
	//LOG_PRINT("[LIGHT TASK]\t [DEBUG] Invoking timer handler");
	
	data = get_sensorlux();

	int fd = open(proj3,O_WRONLY);

	if (data == -1) {
		//sprintf(buffer,"LIGHT THREAD DATA\tTID:%ld\tLight Sensor Down\n",syscall(SYS_gettid));
		BUILD_MESSAGE(buffer, "[LIGHT TASK] [ERROR] Light sensor down");
	} else {
		//sprintf(buffer,"LIGHT THREAD DATA\tTID:%ld\tLight = %f\n",syscall(SYS_gettid), data);
		BUILD_MESSAGE(buffer, "[LIGHT TASK] [INFO] LUX = %f", data);
	}

	mq_send(logger_queue, buffer, LOGGER_QUEUE_SIZE, 0);

	write(fd,"L",1);

	close(fd); 
	
	pthread_mutex_unlock(&lock);

	sprintf(socket_buffer,"Light (Lux): %f\n",data);
	if(SOCKET == 4)
	{
		mq_send(socket_queue,socket_buffer,50,0);
		SOCKET=0;
	}
	
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

	

	 //pid_t light_tid = syscall(SYS_gettid);	//Get thread id
	 //printf("LIGHT TID:%d\n",light_tid);
	 // pid_t light_tid = getpid();	//Get thread id
	 // printf("LIGHT TID:%d\n",light_tid);

	 int status = i2c_open();

    if (status != 0) {
        printf("Failed to open I2C Bus\n");
        //return -1;
    }

    if ((sensor_enable()) != MRAA_SUCCESS) {

        printf("Failed to enable the sensor\n");
        //return -1;
    }



	//pthread_cancel(pthread_self());
	//timer_delete(light_timerid);


}
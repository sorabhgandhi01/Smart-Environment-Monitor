/*@Filename     : light_task.c
 * @Author      : Om Raheja & Sorabh Gandhi
 * @Course      : [PROJECT 1]Advanced Embedded Software Development Spring 2019
 * @Date        : 31st March 2019
 * @brief       : Light task is triggered every 2 seconds. It then send the lux value
 * 		  to the log file. 
 * */


/******************************
 * USER DEFINED HEADER FILES  *
 * ****************************/
#include "main_task.h"
#include "light_task.h"
#include "light_sensor.h"
#include "led.h"

/******************************
 * GLOBAL VARIABLES           *
 * ****************************/
char *proj3 = "/tmp/proj1";	/* Path for named pipe */
extern int SOCKET;
int dark = 1;
int prev_state = -1;


/******************************
 * SIGNAL HANDLER FOR LIGHT   *
 * ****************************/
void light_signal_handler(int signo, siginfo_t *info,void *extra)
{
	printf("\nKilling LIGHT THREAD\n");
	sensor_disable();		/* Disable Light sensor */
	timer_delete(light_timerid);	/* Delete Posix timer */
	pthread_cancel(light_thread);	/* Kill Light Thread */
}


/*********************************
 * SET SIGNAL HANDLER FOR LIGHT  *
 * *******************************/
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


/*********************************
 * TIMER HANDLER FOR LIGHT TASK  *
 * *******************************/
void light_timer_handler(union sigval val)
{
	char buffer[LOGGER_QUEUE_SIZE];
	char socket_buffer[50];
	float data;

	pthread_mutex_lock(&lock);
	
	LOG_PRINT("[LIGHT TASK]\t [DEBUG] Invoking timer handler\n");
		
	/* Get LUx value from light sensor */	
	data = get_sensorlux();

	/* Open pipe for write only operation */
	int fd = open(proj3,O_WRONLY);

	if (data == -1)
	{
		BUILD_MESSAGE(buffer, "[LIGHT TASK] [ERROR] Light sensor down");
		/* Switch on LED to indicate error */
		LIGHT_ERROR_LED_ON();
	}
       	else
       	{
		/* LED remains OFF when no error */
		LIGHT_ERROR_LED_OFF();

		if (data < 50) {
			dark = 0;
		} else {
			dark = 1;
		}

		/* LOG if light state changes from dark to light or from light to dark */
		if (dark != prev_state) {
			if (dark) {
				BUILD_MESSAGE(buffer, "[LIGHT TASK] [INFO] Lux = %f CURRENT STATE = LIGHT", data);
			} else {
				BUILD_MESSAGE(buffer, "[LIGHT TASK] [INFO] Lux = %f CURRENT STATE = DARK", data);
			}
			prev_state = dark;
		} else {
			BUILD_MESSAGE(buffer, "[LIGHT TASK] [INFO] LUX = %f", data);
		}
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


/******************************
 * BIST LIGHT FUNCTION        *
 * ****************************/
int BIST_light()
{
	int status = i2c_open();

    	if (status != 0) {
        	printf("Failed to open I2C Bus\n");
        	return -1;
    	}

    	status = sensor_enable();
    	if (status != MRAA_SUCCESS) {

        	printf("Failed to enable the sensor\n");
        	return -1;
    	}

    	return status;
}


/******************************
 * LIGHT TASK THREAD HANDLER  *
 * ****************************/
void *light_thread_handler()
{
	
	char light_buffer[LOGGER_QUEUE_SIZE];
	char light_info[]= "Taking Light Reading......\n";

	if (BIST_light() == -1) {
		printf("BIST for light sensor failed due to Sensor inactive");
		LIGHT_ERROR_LED_ON();
		BUILD_MESSAGE(light_buffer, "[LIGHT TASK] [ERROR] BIST for light sensor failed due to Sensor inactive");

		mq_send(logger_queue, light_buffer, LOGGER_QUEUE_SIZE, 0);

		pthread_cancel(light_thread);
	} else {
		BUILD_MESSAGE(light_buffer, "[LIGHT TASK] [DEBUG] BIST for light sensor passed");
		mq_send(logger_queue, light_buffer, LOGGER_QUEUE_SIZE, 0);

		LIGHT_ERROR_LED_OFF();
	}

	mq_send(logger_queue, light_buffer, LOGGER_QUEUE_SIZE, 0);

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

}

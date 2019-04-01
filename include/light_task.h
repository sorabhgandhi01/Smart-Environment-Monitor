/*@filename     : light_task.h
 * @author      : Om Raheja & Sorabh Gandhi
 * @brief       : Contains all the function prototypes realted to light task
 * @date        : 31st March 2019
 * */

#ifndef LIGHT_TASK_H
#define LIGHT_TASK_H


/******************************
 * PTHREAD VARIABLES          *
 * ****************************/
pthread_t light_thread;

/******************************
 * POSIX TIMER VARIABLES      *
 * ****************************/
static timer_t light_timerid;


/******************************
 * THREAD IDs                 *
 * ****************************/
pid_t light_tid;


/*****************************************
 * FUNCTION PROTOTYPE FOR THREAD HANDLER *
 * ***************************************/
/********************************************
 * LIGHT THREAD HANDLER                     *
 * ******************************************/
/*@brief: The light thread handler triggers the light timer handler every
 *        2 seconds. It also initializes the light sensor.
 *
 *@param: void
 *
 *@return: No Return value
 * */
void *light_thread_handler();


/********************************************
 * FUNCTION PROTOTYPE (POSIX TIMER HANDLER) *
 * ******************************************/
/********************************************
 * LIGHT TIMER HANDLER                     *
 * ******************************************/
/*@brief: Gets called every 2 seconds. Measures the lux value and logs
 * 	 it to the log file. If a remote client socket request comes in,
 * 	 this function sends the most recent lux value to the Server via 
 * 	 message queue. The server then sends the data to the client via 
 * 	 socket. 
 *
 *@param: signal value
 *
 *@return: No Return value
 * */
void light_timer_handler(union sigval val);


/********************************************
 * FUNCTION PROTOTYPE (SIGNAL HANDLER)      *
 * ******************************************/
/********************************************
 * SET LIGHT SIGNAL HANDLER                     *
 * ******************************************/
/*@brief: Sets the signal handler for light task/thread
 *
 *@param: void
 *
 *@return: No Return value
 * */
void set_light_signal_handler(void);



/********************************************
 * LIGHT SIGNAL HANDLER                     *
 * ******************************************/
/*@brief: On reception of a SIGUSR2 signal, this function will be invoked.
 * 	  It deletes the light timer, kills the light threads and disables the
 * 	  light sensor.
 *
 *@param: param1: signo
	  param2: info string 
 *	  param3: extra
 *
 *@return: No Return value
 * */
void light_signal_handler(int signo, siginfo_t *info,void *extra);

#endif

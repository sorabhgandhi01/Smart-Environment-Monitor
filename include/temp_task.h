/*@filename     : temp_task.h
 * @author      : Om Raheja & Sorabh Gandhi
 * @brief       : Contains all the function prototypes realted to temperature task
 * @date        : 31st March 2019
 * */

#ifndef TEMP_TASK_H
#define TEMP_TASK_H


/******************************
 * PTHREAD VARIABLES          *
 * ****************************/
pthread_t temp_thread;


/******************************
 * POSIX TIMER VARIABLES      *
 * ****************************/
static timer_t temp_timerid;



/******************************
 * THREAD IDs                 *
 * ****************************/
pid_t temp_tid;



/*****************************************
 * FUNCTION PROTOTYPE FOR THREAD HANDLER *
 * ***************************************/
/********************************************
 * TEMPERATURE THREAD HANDLER               *
 * ******************************************/
/*@brief: The temperature thread handler triggers the temperature timer handler every
 *        2 seconds. It also initializes the temperature sensor.
 *
 *@param: void
 *
 *@return: No Return value
 * */

void *temp_thread_handler();


/********************************************
 * FUNCTION PROTOTYPE (POSIX TIMER HANDLER) *
 * ******************************************/
/********************************************
 * TEMPERATURE TIMER HANDLER                *
 * ******************************************/
/*@brief: Gets called every 2 seconds. Measures the temperature value and logs
 *       it to the log file. If a remote client socket request comes in,
 *       this function sends the most recent lux value to the Server via 
 *       message queue. The server then sends the data to the client via 
 *       socket. 
 *
 *@param: signal value
 *
 *@return: No Return value
 * */

void temp_timer_handler(union sigval val);




/********************************************
 * FUNCTION PROTOTYPE (SIGNAL HANDLER)      *
 * ******************************************/
/********************************************
 * SET TEMP SIGNAL HANDLER                     *
 * ******************************************/
/*@brief: Sets the signal handler for temperature task/thread
 *
 *@param: void
 *
 *@return: No Return value
 * */

void set_temp_signal_handler(void);



/********************************************
 * LIGHT SIGNAL HANDLER                     *
 * ******************************************/
/*@brief: On reception of a SIGUSR1 signal, this function will be invoked.
 *        It deletes the light timer, kills the light threads and disables the
 *        light sensor.
 *
 *@param: param1: signo
          param2: info string
 *        param3: extra
 *
 *@return: No Return value
 * */
void temp_signal_handler(int signo, siginfo_t *info,void *extra);


#endif

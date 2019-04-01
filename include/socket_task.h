/*@filename     : socket_task.h
 * @author      : Om Raheja & Sorabh Gandhi
 * @brief       : Contains all the function prototypes realted to socket task
 * @date        : 31st March 2019
 * */

#ifndef SOCKET_TASK_H
#define SOCKET_TASK_H

/******************************
 * PTHREAD VARIABLES          *
 * ****************************/
pthread_t socket_thread;


/******************************
 * POSIX TIMER VARIABLES      *
 * ****************************/
static timer_t socket_timerid;


/******************************
 * THREAD IDs                 *
 * ****************************/
pid_t socket_tid;


/*****************************************
 * FUNCTION PROTOTYPE FOR THREAD HANDLER *
 * ***************************************/
/********************************************
 * SOCKET THREAD HANDLER                     *
 * ******************************************/
/*@brief: The socket thread handler triggers the socket timer handler every
 *        2 seconds. It waits for any remote client request that may come in
 *        and sends the requested data to the client.
 *
 *@param: void
 *
 *@return: No Return value
 * */
void *socket_thread_handler();


/********************************************
 * FUNCTION PROTOTYPE (POSIX TIMER HANDLER) *
 * ******************************************/
/********************************************
 * SOCKET TIMER HANDLER                     *
 * ******************************************/
/*@brief: Gets called every 2 seconds. Sends a heartbeat to the main task
 * 	  to indicate that it is alive.
 *
 *@param: signal value
 *
 *@return: No Return value
 * */
void socket_timer_handler(union sigval val);



/********************************************
 * FUNCTION PROTOTYPE (SIGNAL HANDLER)      *
 * ******************************************/
/********************************************
 * SET SOCKET SIGNAL HANDLER                *
 * ******************************************/
/*@brief: Sets the signal handler for socket task/thread
 *
 *@param: void
 *
 *@return: No Return value
 * */
void set_socket_signal_handler(void);


/********************************************
 * SOCKET SIGNAL HANDLER                     *
 * ******************************************/
/*@brief: On reception of a SIGTERM signal, this function will be invoked.
 *        It deletes the socket timer, kills the socket threads,closes the
 *        message queue and the socket.
 *
 *@param: param1: signo
          param2: info string
 *        param3: extra
 *
 *@return: No Return value
 * */
void socket_signal_handler(int signo, siginfo_t *info,void *extra);


#endif

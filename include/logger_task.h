/* @filename:logger_task.h
*/

/*pthread variables*/
pthread_t logger_thread;

/* Posix timer variables */
static timer_t logger_timerid;

/* Thread IDs */
pid_t logger_tid;

/* Function Prototype for thread handler */
void *logger_thread_handler();

/* Function Prototype (posix timer handler) */
void logger_timer_handler(void);
/* @filename:light_task.h
*/

/*pthread variables*/
pthread_t light_thread;

/* Posix timer variables */
static timer_t light_timerid;

/* Thread IDs */
pid_t light_tid;

/* Function Prototype for thread handler */
void *light_thread_handler();


/* Function Prototype (posix timer handler) */
void light_timer_handler(union sigval val);

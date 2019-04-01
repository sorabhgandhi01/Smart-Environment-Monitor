#ifndef TEMP_TASK_H
#define TEMP_TASK_H

/*pthread variables*/
pthread_t temp_thread;

/* Posix timer variables */
static timer_t temp_timerid;

/* Thread IDs */
pid_t temp_tid;


/* Function Prototype for thread handler */
void *temp_thread_handler();


/* Function Prototype (posix timer handler) */
void temp_timer_handler(union sigval val);

/* Function Prototype (Signal Handler) */
void set_temp_signal_handler(void);
void temp_signal_handler(int signo, siginfo_t *info,void *extra);


#endif
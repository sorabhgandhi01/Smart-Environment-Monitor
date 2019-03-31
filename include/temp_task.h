/* @filename:temp_task.h
*/
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

#endif
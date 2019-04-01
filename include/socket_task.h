#ifndef SOCKET_TASK_H
#define SOCKET_TASK_H

/*pthread variables*/
pthread_t socket_thread;

/* Posix timer variables */
static timer_t socket_timerid;

/* Thread IDs */
pid_t socket_tid;

/* Function Prototype for thread handler */
void *socket_thread_handler();

/* Function Prototype (posix timer handler) */
void socket_timer_handler(union sigval val);

/* Function Prototype (Signal Handler) */
void set_socket_signal_handler(void);
void socket_signal_handler(int signo, siginfo_t *info,void *extra);


#endif

#ifndef LOGGER_TASK_H
#define LOGGER_TASK_H



#define LOG_TO_FILE(fp, format, ...) \
do{ \
	fprintf(fp, format, ##__VA_ARGS__); \
	fflush(fp); \
}while(0)

#ifndef UNIT_TEST
/*pthread variables*/
pthread_t logger_thread;

/* Posix timer variables */
static timer_t logger_timerid;

/* Thread IDs */
pid_t logger_tid;

char *file_name;

/* Function Prototype for thread handler */
void *logger_thread_handler();

/* Function Prototype (posix timer handler) */
void logger_timer_handler(union sigval val);


/* Function Prototype (Signal Handler) */
void set_logger_signal_handler(void);
void logger_signal_handler(int signo, siginfo_t *info,void *extra);
#endif

#endif
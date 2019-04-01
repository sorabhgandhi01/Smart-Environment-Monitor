#include "main_task.h"
#include "socket_task.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>



char *proj4 = "/tmp/proj1";
int sock;
int mysock;
int flag=0;
extern int SOCKET;


void socket_signal_handler(int signo, siginfo_t *info,void *extra)
{
	printf("\nKilling SOCKET THREAD\n");
	timer_delete(socket_timerid);	//delete timer	
	mq_close(socket_queue);			//close message queue
	mq_unlink(SOCKET_QUEUE_NAME);	//unlink queue
	close(sock);					//close socket
	pthread_cancel(socket_thread);	//kill thread

}


void set_socket_signal_handler(void)
{
	struct sigaction action;
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = socket_signal_handler;

	if(sigaction(SIGTERM,&action,NULL) == -1)
	{
		perror("Sigusr : Sigaction");
		_exit(1);
	}
}


/* Socket Timer Handler */
void socket_timer_handler(union sigval val)
{
	pthread_mutex_lock(&lock);

	LOG_PRINT("[SOCKET TASK]\t [DEBUG] Invoking timer handler\n");
	
	int fd = open(proj4,O_WRONLY);

	write(fd,"S",1);

	close(fd);
	
	pthread_mutex_unlock(&lock);
}


void *socket_thread_handler()
{
	char socket_buffer[50];
	char log_buffer[LOGGER_QUEUE_SIZE];
	char socket_info[]="Socket Thread Alive......\n";


	struct sigevent socket_sev;
	struct timespec socket_mainTimeSpec;
	struct itimerspec socket_trigger;

	memset(&socket_sev,0,sizeof(struct sigevent));
	memset(&socket_trigger,0,sizeof(struct itimerspec));

	/* 
     * Set the notification method as SIGEV_THREAD:
     *
     * Upon timer expiration, `sigev_notify_function` (thread_handler()),
     * will be invoked as if it were the start function of a new thread.
     *
     */
    socket_sev.sigev_notify = SIGEV_THREAD;
    socket_sev.sigev_notify_function = &socket_timer_handler;
	socket_sev.sigev_value.sival_ptr = &socket_info;

	 /*
    * Create the timer. In this example, CLOCK_REALTIME is used as the
    * clock, meaning that we're using a system-wide real-time clock for 
    * this timer.
    */
	timer_create(CLOCK_REALTIME, &socket_sev, &socket_timerid);

	/* Timer expiration will occur withing 2 seconds after being armed
     * by timer_settime(). Then the interval timer will takeover 
     */
   	socket_trigger.it_value.tv_sec = 2;

	/* Uncomment the following line to set the interval timer and
	 * and see the threadhandler() execute periodically.
	 */	
    socket_trigger.it_interval.tv_sec = 2;

    timer_settime(socket_timerid,0,&socket_trigger,NULL);

    printf("Connecting with Client........\n");

    struct sockaddr_in server;
	char buffer1[1024];		//buffer to hold data.
	char data_for_client[50];
	int ret_val;	
	int len;
	char filereader[30];


	/* Create TCP/IP socket */
	sock = socket(AF_INET,SOCK_STREAM,0);



	if(sock < 0)
	{
		BUILD_MESSAGE(log_buffer, "[SOCKET TASK][ERROR] FAILED TO CREATE SOCKET");
		mq_send(logger_queue, log_buffer, LOGGER_QUEUE_SIZE, 0);
		memset(log_buffer, 0, sizeof(log_buffer));

		perror("Failed to create Socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(9000);


	/* Call Bind */
	if(bind(sock, (struct sockaddr *)&server, sizeof(server)))
	{
		BUILD_MESSAGE(log_buffer, "[SOCKET TASK][ERROR] FAILED TO BIND SOCKET");
		mq_send(logger_queue, log_buffer, LOGGER_QUEUE_SIZE, 0);
		memset(log_buffer, 0, sizeof(log_buffer));

		perror("Bind Failed");
		exit(1);
	}

	/* Listen to network */
	listen(sock,5);

	mysock = accept(sock, (struct sockaddr *) 0,0);

	while(1){

	recv(mysock,buffer1,20,0);

	printf("Server string received:%s\n",buffer1);

	BUILD_MESSAGE(log_buffer, "[SOCKET TASK][INFO] RECIEVED NEW REMOTE REQUEST -> %s", buffer1);
	mq_send(logger_queue, log_buffer, LOGGER_QUEUE_SIZE, 0);
	memset(log_buffer, 0, sizeof(log_buffer));

	if(!strcmp(buffer1,"1"))
	{
		SOCKET = 1;
		mq_receive(socket_queue,data_for_client,50,0);
		//fprintf(fptr,"%s\n",buffer);
		//strcpy()
		//printf("Temp in C : 25 C\n");
		//strcpy(buffer1,"Temp in C : 25 C\n");
		send(mysock,data_for_client,50,0);
	}
	else if (!strcmp(buffer1,"2"))
	{
		SOCKET = 2;
		mq_receive(socket_queue,data_for_client,50,0);
		//printf("Temp in F : 255 C\n");
		//strcpy(buffer1,"Temp in F : 255 C\n");
		send(mysock,data_for_client,50,0);

	}
	else if (!strcmp(buffer1,"3"))
	{

		SOCKET = 3;
		mq_receive(socket_queue,data_for_client,50,0);
		//printf("Temp in K : 1050 C\n");		
		//strcpy(buffer1,"Temp in K : 1050 C\n");
		send(mysock,data_for_client,50,0);

	}
	else if(!strcmp(buffer1,"4"))
	{
		SOCKET = 4;
		mq_receive(socket_queue,data_for_client,50,0);
		//fprintf(fptr,"%s\n",buffer);
		//strcpy()
		//printf("Temp in C : 25 C\n");
		//strcpy(buffer1,"Temp in C : 25 C\n");
		send(mysock,data_for_client,50,0);
		//printf("Lux : 35.58 C\n");		
		//strcpy(buffer1,"Lux : 35.58 C\n");
		//send(mysock,buffer1,20,0);

	}

	BUILD_MESSAGE(log_buffer, "[SOCKET TASK][INFO] SENDING RESPONSE TO CLIENT -> %s", data_for_client);
	mq_send(logger_queue, log_buffer, LOGGER_QUEUE_SIZE, 0);
	memset(log_buffer, 0, sizeof(log_buffer));
	memset(data_for_client, 0, sizeof(data_for_client));
	memset(buffer1, 0, sizeof(buffer1));
}

}
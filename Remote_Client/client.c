/*@Filename	: client.c
 * @Author	: Om Raheja & Sorabh Gandhi
 * @Course	: [PROJECT 1]Advanced Embedded Software Development Spring 2019
 * @Date	: 31st March 2019
 * @References	: https://www.youtube.com/watch?v=pFLQmnmDOo
 *		: https://gist.github.com/sevko/d23646ba07c77c15fde9
 * @brief	: Connects to the Smart environment monitoring system via a
 * 		  server-client architecture. Client can ask the system for
 * 		  Temperature/Lux data. Client then receives and displays
 * 		  the received data.
 * */


/******************************
 * STANDARD C LIBRARY HEADERS *
 * ****************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>


/******************************
 * GLOBAL VARIABLES           *
 * ****************************/
int sock;


/******************************
 * MAIN FUNCTION              *
 * ****************************/
int main(int argc,char *argv[])
{
	struct sockaddr_in client;
	
	/* Create Socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock < 0)
	{
		perror("Socket Failed");
		exit(1);
	}

	/* Populate client structure with IP address and port number */
	client.sin_family = AF_INET;
	client.sin_port = htons(9000);
	client.sin_addr.s_addr = inet_addr("128.138.189.131");

	
	if(connect(sock,(struct sockaddr *)&client,sizeof(client)) < 0)
	{
		perror("Connection Failed");
		close(sock);
		exit(1);
	}

	printf("Connecting with Server........\n");

	char str[5];
	char string[50];

	while(1)
	{
		/* Clear the data buffers */
		memset(str,0,sizeof(str));
		memset(string,0,sizeof(string));

		/* Menu for obtaining data from the system */
		printf("Press Serial Number to obtain respective Data from the System\n");
		printf("1)Get Temp in Celcius.\n");
		printf("2)Get Temp in Farhenite.\n");
		printf("3)Get Temp in Kelvin.\n");
		printf("4)Get Lux value.\n");

		scanf("%s",&str);
		
		/* Send selected option to system */
		send(sock,str,20,0);

		/* Receive and print the received data */
		recv(sock,string,50,0);
		printf("In Client: %s\n",string);
}
	/* Close the socket */
	close(sock);
	return 0;
}

/* EOF */

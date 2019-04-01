/*  @Author		: Om Raheja & Sorabh Gandhi
	@Filename	: client.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@References	: https://www.youtube.com/watch?v=pFLQmnmDOo
				: https://gist.github.com/sevko/d23646ba07c77c15fde9
*/

/*Standard C Library Headers*/
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

extern int SOCKET_FLAG=0;
int sock;
int flag=0;

int main(int argc,char *argv[])
{

	//int sock;
	int ret_val;
	struct sockaddr_in server;
	char msgbuffer[1024];


	sock = socket(AF_INET, SOCK_STREAM, 0); //create socket


	if(sock < 0)
	{
		perror("Socket Failed");
		exit(1);
	}


	server.sin_family = AF_INET;
	server.sin_port = htons(9000);
	server.sin_addr.s_addr = inet_addr("10.0.0.52");


	if(connect(sock,(struct sockaddr *)&server,sizeof(server)) < 0)
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
		
		memset(str,0,sizeof(str));
		memset(string,0,sizeof(string));
		printf("Enter the data you want to retrieve!\n");
		printf("1)Get Temp in Celcius.\n");
		printf("2)Get Temp in Farhenite.\n");
		printf("3)Get Temp in Kelvin.\n");
		printf("4)Get Lux value.\n");

		scanf("%s",&str);

		//SOCKET_FLAG =1;
		send(sock,str,20,0);	
		recv(sock,string,50,0);
		printf("In Client: %s\n",string);
}

/*
	while(1)
	{

		for(int i=0;i<10;i++)
		{
			client.string = array_for_client[i];
			int a =send(sock,client.string,strlen(client.string),0);
			fileptr=fopen("om.txt","a");
			fprintf(fileptr,"Timestamp :%ld\n",getMicrotime());
			fprintf(fileptr,"Message Length = %d\n",a);
			fclose(fileptr);


			memset(msgbuffer,0,sizeof(msgbuffer));
			if((ret_val = recv(sock,msgbuffer,7,0) < 0) && (flag == 0))
			{
				perror("Reading stream message error");
			}
			else if(ret_val == 0)
			{
				//printf("Ending Connection\n");
			}
			else
			{
				fileptr=fopen("om.txt","a");
				fprintf(fileptr,"Timestamp :%ld\n",getMicrotime());
				fprintf(fileptr,"Message : %s\n",msgbuffer);
				fclose(fileptr);
			}

			fileptr=fopen("om.txt","a");
			fprintf(fileptr,"Timestamp :%ld\n",getMicrotime());
			fprintf(fileptr,"Message in Client = %s\n",msgbuffer);
			fclose(fileptr);
			sleep(1);

		}
	}
	*/
	close(sock);
	return 0;
}
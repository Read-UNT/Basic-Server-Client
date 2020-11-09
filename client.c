/*
=============================================================================================================

Name             : client.c
Author           : Read L. Ballew Jr.
Course Section   : CSCE 3530.001
Date             : February 20th, 2019
Description      : The purpse of this program is to send a sentence and receive a count of the number of
				   vowels in return. This is the client side.

				   Compilation: gcc -o client client.c
    			   Execution  : ./client "Insert_Port_of_Choice"

=============================================================================================================
*/

#include <stdio.h>                      
#include <sys/socket.h>         
#include <netinet/in.h>
#include <netdb.h>
#include <error.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int sockfd, portno, n, other;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256], discon[256];
	strcpy(discon, "Bye");

	if(argc < 2)
	{
		printf("\nPort number is missing...\n");
		exit(0);
	}

	portno = atoi(argv[1]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error(EXIT_FAILURE, 0, "ERROR opening socket");
	}
	//server = gethostbyname("129.120.151.94"); //IP address of server
	server = gethostbyname("129.120.151.94"); //Both in the same machine [IP address 127.0.0.1]
	
	if(server == NULL)
	{
		printf("\nERROR, no such host...\n");
		exit(0);
	}

	//Connecting with the server
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	memcpy(&serv_addr.sin_addr, server->h_addr, server->h_length);
	if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	{
		error(EXIT_FAILURE, 0, "ERROR connecting the server...");
	}

	while(9000)
	{
		//Sending the message to the server
		printf("\nEnter client's message: ");
		bzero(buffer,256);
		//fgets(buffer, 256, stdin);
		//scanf("%[^\n]s", buffer);
		fgets(buffer, 256, stdin);

		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
		{
			error(EXIT_FAILURE, 0, "ERROR writing to socket");
		}
		other = strncmp(buffer, discon, 3);

		if(other == 0)
		{
			printf("We're going down!!\n");
			exit(0);
		}
		
		//Receiving the message from the server
		bzero(buffer,256);
		n = read(sockfd, buffer, 255);
		if (n < 0)
		{
			error(EXIT_FAILURE, 0, "ERROR reading from socket");
		}
		else if(buffer[0] == '.' && buffer[1] == '.' && buffer[2] == '.')
		{
			//Closing the connection
			close(sockfd);	
		}
		else
		{
			printf("The number of vowels present is: %s\n", buffer);
			bzero(buffer,256);
		}
	}

	//Closing the connection
	close(sockfd);		

	return 0;	
} 

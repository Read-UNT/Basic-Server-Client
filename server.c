/*
=============================================================================================================

Name             : server.c
Author           : Read L. Ballew Jr.
Course Section   : CSCE 3530.001
Date             : February 20th, 2019
Description      : The purpse of this program is to receive a sentence and send a count of the number of
				   vowels in return. This is the server side, it accepts one client connection, and
				   expects a sentence in return.

				   Compilation: gcc -o server server.c
    			   Execution  : ./server "Insert_Port_Of_Choice"

=============================================================================================================
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen, n;
   	struct sockaddr_in serv_addr, cli_addr; 
    char buffer[256]; 
	bool checkerino = true;

	if(argc < 2)
	{
		printf("\nPort number is missing...\n");
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error(EXIT_FAILURE, 0, "ERROR opening socket");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	{
		error(EXIT_FAILURE, 0, "ERROR binding");
	}

	printf("\nServer Started and listening to the port %d\n", portno);
    listen(sockfd, 5);

	while(1)
	{
		//Connecting with the client
		clilen=sizeof(cli_addr);
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
		{
			error(EXIT_FAILURE, 0, "ERROR on accept");
		}
		else
		{
			checkerino = true;
		}
		printf("\nClient is connected...\n");

		while(checkerino)
		{
			//Receiving the message from the client
			bzero(buffer,256);
			n = read(newsockfd,buffer,255);
			if(n < 0)
			{
				error(EXIT_FAILURE, 0, "ERROR reading from socket");
			}
			else
			{
				printf("\nClient has sent: %s\n", buffer);
				//Length of buffer:
				int i, bufLen, vowelCount;
				bufLen = strlen(buffer);
				vowelCount = 0;

				//Counting the number of vowels
				for(i = 0; i < bufLen; i++)
				{
					if(buffer[i] == 'a' || buffer[i] == 'A' || 
					buffer[i] == 'e' || buffer[i] == 'E' || 
					buffer[i] == 'i' || buffer[i] == 'I' || 
					buffer[i] == 'o' || buffer[i] == 'O' || 
					buffer[i] == 'u' || buffer[i] == 'U'){

						vowelCount++;
					}
					if(buffer[0] == 'B' && buffer[1] == 'y' && buffer[2] == 'e')
					{
						bzero(buffer,256);
						snprintf(buffer, sizeof(buffer), "...");
						n = write(newsockfd, buffer, strlen(buffer));
						printf("Client has requested that the connection be closed.\n");
						//Closing the connection
						close(newsockfd);
						checkerino = false;
					}
				}

				if(!checkerino)
				{
					break;
				}

				//Sending the message to the client
				bzero(buffer,256);
				snprintf(buffer, sizeof(buffer), "%d", vowelCount);
				n = write(newsockfd, buffer, strlen(buffer));
			}
		}
	}

	return 0;
}

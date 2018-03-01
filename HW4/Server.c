/*Server side program to use socket programming*/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define	PORT	8080

int main()
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024]={0};
	char *hello= "Hello from server";
	
	/*Creating a socket file descriptor*/
	if((server_fd= socket(AF_INET,SOCK_STREAM,0))==0)
	{
                printf("Socket creation failed\n");
                exit(EXIT_FAILURE);
        }
	else
	{
		printf("Socket created successfully \n");
		
	}

	/*Attaching socket to port 8080*/
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
	{
                printf("Socket could not be attached to port. sorry!\n");
                exit(EXIT_FAILURE);
        }
	else
	{
                printf("Successful in attaching port to socket.Hurray!\n");
                
        }
	

	address.sin_family = AF_INET;
    	address.sin_addr.s_addr = INADDR_ANY;
    	address.sin_port = htons( PORT );
	
	
	/*Forcefully attaching socket to the port 8080*/
    	if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    	{
        	printf("Sorry the binding failed\n");
        	exit(EXIT_FAILURE);
    	}
	else
	{
		printf("Binding is also a success!\n");

	}
    	if (listen(server_fd, 3) < 0)
    	{
        	printf("could not listen\n");
        	exit(EXIT_FAILURE);
    	}
	else
	{

		printf("Listening\n");

	}	
    	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    	{
        	printf("did not accept\n");
        	exit(EXIT_FAILURE);
    	}
	
	else
	{
                printf("Accepted\n");
                
        }

    	valread = read( new_socket , buffer, 1024);
    	printf("%s\n",buffer );
    	send(new_socket , hello , strlen(hello) , 0 );
    	printf("Hello message sent\n");
    	return 0;
}


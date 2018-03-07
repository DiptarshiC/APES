/**
*@file: socket.c
*
*@Description: The kernel module takes a process ID
*
*
*@Date: 3rd February 2018
*
*@Author: Diptarshi Chakraborty
*
*/


#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Data
{
bool LED;
char msg[30];

}Data1;


void main()
{

	pid_t child;
	int parent_socket=0;
	int child_socket=0;
	Data1 data;
	char *parent_PATH = "/tmp/parent_sockaddr";
  	char *child_PATH = "/tmp/child_sockaddr";
	
	struct sockaddr_un parent_sockaddr, child_sockaddr;
  	socklen_t parent_sockaddr_length, child_sockaddr_length;
	
	
	bzero(&data, sizeof(Data1));
  	remove(parent_PATH);
  	remove(child_PATH);


	printf("Now let us fork a child process");

	switch(child=fork())
	
	{

	case 0:
	printf("I am a child process that has finally been borrn!!! haha\n");
	
	/*Now we shall create a datagram socket*/
	child_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
	
	if(child_socket == -1)
	perror("The creation of the socket has failed.sorry :( \n");

	parent_sockaddr_length = sizeof(parent_sockaddr);
      	child_sockaddr_length = sizeof(child_sockaddr);
      	bzero(&parent_sockaddr, parent_sockaddr_length);
      	bzero(&child_sockaddr, child_sockaddr_length);


	child_sockaddr.sun_family = AF_UNIX;
      	strncpy(child_sockaddr.sun_path, child_PATH, sizeof(child_sockaddr.sun_path) -1);
	
	if(bind(child_socket, (struct sockaddr *) &child_sockaddr, sizeof(struct sockaddr_un)) == -1)
        perror("There has been an error in binding the child\n");
	
	printf("Child socket has been created successfully!!\n");
	

	if(recvfrom(child_socket, (void *) &data, sizeof(Data1), 0, (struct sockaddr *) &parent_sockaddr, &parent_sockaddr_length) == -1)
        perror("There is an error in receiving from the parent to child\n");

	printf("child has received from %s.\n", parent_sockaddr.sun_path);
	
	printf("Child has received string from parent %s ",data.msg);

	if(data.LED==true)
	{
		printf("LED state is true\n");
	}
	else
		printf("LED state is false \n");
	

	strcat(data.msg, " Diptarshi");
        data.LED = !data.LED;
	
	 printf("Child is sending modified  string to  parent %s ",data.msg);

        if(data.LED==true)
        {
                printf("modified LED state is true\n");
        }
        else
                printf("modified LED state is false \n");

	if(close(child_socket == -1))        //close child socket
          perror("close child_socket");

        remove(child_PATH);
        printf("The child socket is now closing\n");

	break;

	default:
      	parent_socket = socket(AF_UNIX, SOCK_DGRAM, 0);

	if(parent_socket == -1)
        perror("The creation of the socket has failed.sorry :( \n");
     
	parent_sockaddr_length = sizeof(parent_sockaddr);
      	child_sockaddr_length = sizeof(child_sockaddr);
      	bzero(&parent_sockaddr, parent_sockaddr_length);
      	bzero(&child_sockaddr, child_sockaddr_length);

parent_sockaddr.sun_family = AF_UNIX;
strncpy(parent_sockaddr.sun_path, parent_PATH, sizeof(parent_sockaddr.sun_path) -1);

child_sockaddr.sun_family = AF_UNIX;
strncpy(child_sockaddr.sun_path, child_PATH, sizeof(child_sockaddr.sun_path) -1);

if(bind(parent_socket, (struct sockaddr *) &parent_sockaddr, sizeof(struct sockaddr_un)) == -1)
        perror("binding  parent failed\n");

printf("Parent socket creation successfull\n");

strcpy(data.msg, "Chakraborty");
      data.LED = false;
	
printf("Now parent will be  Sleeping for 1 second to make sure child has successfully set up its socket.\n");
      usleep(500);


 printf("parent is sending string %s ",data.msg);

        if(data.LED==true)
        {
                printf("LED state is true\n");
        }
        else
                printf("LED state is false \n");


if(sendto(parent_socket, (const void *) &data, sizeof(Data1), 0, (const struct sockaddr *) &child_sockaddr, child_sockaddr_length) == -1)
        perror("error i sending parent to child \n");

bzero(&data, sizeof(Data1));

if(recvfrom(parent_socket, (void *) &data, sizeof(Data1), 0, (struct sockaddr *) &child_sockaddr, &child_sockaddr_length) == -1)
        perror("errorin receiving from child\n");

 printf("parent has received string %s ",data.msg);

        if(data.LED==true)
        {
                printf("LED state is true\n");
        }
        else
                printf("LED state is false \n");

      

      if(close(parent_socket == -1))        
        perror("error in closing \n");

      remove(parent_PATH);
      printf("Communication successful. Closed parent_socket Congo.\n");

      break;
  }

  exit(EXIT_SUCCESS);

}

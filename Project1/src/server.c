/**
*@file: server.c
*
*@description: This header file provides definitions
*               remote server.
*
*@Date:2nd March 2017
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
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
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr

#include "../includes/i2c.h"
//#include "../includes/temp.h"
#include "../includes/logger.h"
#include "../includes/light.h"
#include "../includes/main.h"

/*
    C socket server example
*/

void main()
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create a socket\n");
    }
    printf("A socket has been created\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind has failed. Error\n");

    }
    printf("binding has been  done\n");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    printf("Waiting for incoming connections\n");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");

    }
    printf("Connection has been accepted\n");

    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
	if ((client_message[0]=='T')||(client_message[0]=='t'))
        {
        strcpy(client_message, "The temperature is:");
        }
        else if((client_message[0]=='L')||(client_message[0]=='l'))
        {
        strcpy(client_message, "The Luminosity is:");
        }
        else
        {
        strcpy(client_message, "Didnt get ur command:");
        }

        write(client_sock , client_message , strlen(client_message));

    }

    if(read_size == 0)
    {
        printf("Client has disconnected\n");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }


}

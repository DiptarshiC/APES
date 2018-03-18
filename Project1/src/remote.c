/**
*@file: remote.c
*
*@description: This header file provides definitions
*               remote server.
*
*@Date:2nd March 2017
*
*@Author:Diptarshi Chakraborty and Connor Shapiro
*/

#include <mqueue.h>
#include <pthread.h>
#include <time.h>
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
#include "../includes/remote.h"
#include "../includes/temp.h"
#include "../includes/logger.h"
#include "../includes/light.h"
#include "../includes/main.h"

/*
    C socket server example
*/

float f=0.0;
char t[20];
void *remote( void * args)
{

	struct remote_thread_info* p_targs = (struct remote_thread_info *)args;

	/* Let Main know that Temperature startup went well */
	mqd_t main_mq = mq_open(p_targs->main_mq_name, O_WRONLY);
	if (main_mq == FAILURE)
	{

        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}
	main_msg_t * p_main_msg = (main_msg_t *) malloc(sizeof(main_msg_t));
	if (!p_main_msg)
	{
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}
	p_main_msg->id = START_OK;
	p_main_msg->source = M_REMOTE;
	if (mq_send(main_mq, p_main_msg, sizeof(main_msg_t), PRIORITY_TWO))
	{
        int8_t retvalue = FAILURE;
        pthread_exit(&retvalue);
	}
	/* Opening remote_msg */
	remote_msg_t * p_remote_msg = (remote_msg_t *) malloc(sizeof(remote_msg_t));

	/* Allocate temp_msg */
	temp_msg_t * p_temp_msg = (temp_msg_t *) malloc(sizeof(temp_msg_t));

	/*Opening temperature message queue*/ 
	mqd_t temp_mq = mq_open(p_targs->temp_mq_name, O_WRONLY);

	/* Allocate light_msg */
	light_msg_t * p_light_msg = (light_msg_t *) malloc(sizeof(light_msg_t));

	 /*Opening light message queue*/
        mqd_t light_mq = mq_open(p_targs->light_mq_name, O_WRONLY);

	 /*Opening remote  message queue*/
        mqd_t remote_mq = mq_open(p_targs->remote_mq_name, O_WRONLY);






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

    /*Receive a message from client.This is the main while loop*/
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
	if ((client_message[0]=='T')||(client_message[0]=='t'))
        {

	/*In this condition we shall send the temp thread a message*/

	p_temp_msg->source=T_REMOTE;
	p_temp_msg->id=TEMP_DATAREQ;

	if (mq_send(temp_mq, p_temp_msg, sizeof(temp_msg_t), PRIORITY_TWO))
		{
        	int8_t retvalue = FAILURE;
        	pthread_exit(&retvalue);
		}
	/*Here we shall receive a message from the temperature thread*/
	mq_receive(remote_mq, p_remote_msg, sizeof(remote_msg_t), NULL); // Block empty

	f=p_remote_msg->value;
	gcvt(f, 8, t);
        strcpy(client_message, "The temperature is:");
	strcat(client_message,t);

        }

        else if((client_message[0]=='L')||(client_message[0]=='l'))
        {
	 /*In this condition we shall send the light thread a message*/

        p_light_msg->source=L_REMOTE;
        p_light_msg->id=LIGHT_DATAREQ;

        if (mq_send(light_mq, p_light_msg, sizeof(light_msg_t), PRIORITY_TWO))
                {
                int8_t retvalue = FAILURE;
                pthread_exit(&retvalue);
                }
        /*Here we shall receive a message from the light thread*/
        mq_receive(remote_mq, p_remote_msg, sizeof(remote_msg_t), NULL); // Block empty

	f=p_remote_msg->value;
	gcvt(f, 8, t);
        strcpy(client_message, "The Luminosity is:");
	strcat(client_message,t);
        }
        else
        {
        strcpy(client_message, "Didnt get ur command:");
        }

        write(client_sock , client_message , strlen(client_message));



	/*Attempt to do a graceful exit*/
	 		if (mq_close(remote_mq))
                        {

                                int8_t retvalue = FAILURE;
                                pthread_exit(&retvalue);

                        }
                        if (mq_unlink(remote_mq))
			{
                                int8_t retvalue = FAILURE;
                                pthread_exit(&retvalue);
                        }

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

	free(p_main_msg);
        free(p_remote_msg);
        int8_t retvalue = SUCCESS;
        pthread_exit(&retvalue);


}

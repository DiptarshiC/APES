/** 
*@file: message_queue.c 
* 
*@Description:Use the pipe IPC to create 
* a message queue to communicate between a parent
* and a child  
* 
*@Author : Diptarshi Chakraborty 
* 
*@Date : February 3rd 2018 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>


typedef struct DATA
{
  bool led_state;
  char string[20];

} DATA1;

/**
*@function:main
*
*@description:The parent thread creates a child thread which
*		which allows communication between them.
*@param:none(void) 	
*
*@return:(int)
*/


int main(void)
{
  pid_t child;
  mqd_t mq;
  struct mq_attr attr;
  DATA1 data;

  bzero(&data, sizeof(DATA1));
  mq_unlink("/message_queue_diptarshi");

  printf(" Creating a child process using fork system call.\n");

  switch (child = fork())
  {
    case -1: 
      perror("fork");
      break;

    case 0:
      printf("This is the forked child process.\n");
      mq = mq_open("/message_queue_diptarshi", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, NULL);

      if(mq == -1)
        perror(" creation of message queue descriptor on the side of the child");

      printf(" message queue descriptor created by child.\n");

      if(mq_receive(mq, (char *) &data, sizeof(data), 0) == -1)
        perror("receiving from parent to child process");

      printf(" Child has received string %s: %s.\n", data.string);
	if(data.led_state == true)
         printf("The value of the led is true\n");
        else
        printf("The value of the led is false\n");




      strcat(data.string, "Hello Dipto");
      data.led_state = !data.led_state;
      printf("Child is sending modified string: %s.\n", data.string);
      if(data.led_state == true)
        printf("The value of the led is true\n");
      else
        printf("The value of the led is false\n");



      if(mq_send(mq, (const char *) &data, sizeof(data), 0) == -1)
        perror("sending from child to parent process");

      mq_close(mq);
      printf("Communication successfully done. Hurray.\n");

      break;

    default: 
      bzero(&attr, sizeof(attr));
      attr.mq_flags = O_RDWR;
      attr.mq_maxmsg = 5;
      attr.mq_msgsize = sizeof(DATA1);

      mq = mq_open("/message_queue_diptarshi", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &attr);

      if(mq == -1)
        perror(" creation of message queue descriptor on the side of the parent");

      printf("Parent has created message_queue descriptor successfully.\n");

      strcpy(data.string, "Hi again Diptarshi");
      data.led_state = false;

      printf("To make sure the child gets the message queue, parent sleeps for half a second.\n");
      usleep(500);

      printf("parent is now  sending string %s \n", data.string); 
	if(data.led_state == true) 
	 printf("The value of the led is true\n");
	else
	printf("The value of the led is false\n");

      if(mq_send(mq, (const char *) &data, sizeof(data), 0) == -1)
        perror("sending from parent to child process");

      bzero(&data, sizeof(DATA1));

      if(mq_receive(mq, (char *) &data, sizeof(data), 0) == -1)
        perror("receiving from child to parent");

      printf("parent received string: %s\n", data.string); 
	if(data.led_state == true)
         printf("The value of the led is true\n");
         else
        printf("The value of the led is false\n");


      mq_close(mq);
      mq_unlink("message_queue");
      printf("Message  sent successfully congratulations.\n");

      break;
  }

  exit(EXIT_SUCCESS);

}




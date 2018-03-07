/** 
*@file: Pipe.c 
* 
*@Description:Use the pipe IPC to create 
* a pipe to communicate between a parent
* and a child  
* 
*@Author : Diptarshi Chakraborty 
* 
*@Date : February 3rd 2018 
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>

//Structure of the data which is communicated between the parent and the child using pipes.
typedef struct payload
{
  char string[30];
  bool led_state;
} payload_t;

void errExit(char *);

int main(void)
{
  pid_t Child_Pid = 0;
  payload_t data;

     /*File descriptor 1*/	
  int parent_to_child[2];     
   /*file descriptor 2*/
  int child_to_parent[2];     



  if(pipe(parent_to_child) == -1)
    errExit("pipe parent_to_child");

  if(pipe(child_to_parent) == -1)
    errExit("pipe child_to_parent");

  printf("Parent process is forking child process.\n");

  switch (Child_Pid = fork())
  {
     case -1: 
     errExit("fork");
     break;

     case 0: 
     printf(" Child process has  Forked.\n");
     if(close(child_to_parent[0]) == -1)      
        errExit("close child_to_parent read");

     if(close(parent_to_child[1]) == -1)     
        errExit("close parent_to_child write");
     printf(" Handled pipe descriptors.\n");

     if(read(parent_to_child[0], &data, sizeof(payload_t)) == -1)
        errExit("read parent_to_child");

      printf(" the child has received string: \"%s\". Received LED State: %s.\n", data.string, data.led_state ? "true" : "false");

      if(close(parent_to_child[0]) == -1)    
        errExit("close parent_to_child read");
      printf(" read end of parent_to_child closed.\n");

      strcat(data.string, " World");
      data.led_state = !data.led_state;

      printf("This is child here.Piping modified string: \"%s\". Modified Received LED State: %s.\n", data.string, data.led_state ? "true" : "false");
      if(write(child_to_parent[1], &data, sizeof(payload_t)) == -1)
        errExit("write child_to_parent");

      if(close(child_to_parent[1]) == -1)        //close child_to_parent write
        errExit("close child_to_parent write");
      printf(" Piping successful. Closed write end of child_to_parent.\n");

      break;

    default: 
      if(close(child_to_parent[1]) == -1)  /*close child_to_parent write*/
        errExit("close child_to_parent write");

      if(close(parent_to_child[0]) == -1)  /*close parent_to_child read*/
        errExit("close parent_to_child read");
      printf(" Handled pipe descriptors.\n");

      strcpy(data.string, "Hello");
      data.led_state = false;

      printf(" Piping string: \"%s\". LED State: %s.\n", data.string, data.led_state ? "true" : "false");

      if(write(parent_to_child[1], &data, sizeof(payload_t)) == -1)
        errExit("write parent_to_child");

      if(close(parent_to_child[1]) == -1)       
        errExit("close parent_to_child write");
      printf(" Piping successful. Closed write end of parent_to_child.\n");

      if(read(child_to_parent[0], &data, sizeof(payload_t)) == -1)
        errExit("read child_to_parent");

      printf(" Received string: \"%s\". Received LED State: %s.\n", data.string, data.led_state ? "true" : "false");

      if(close(child_to_parent[0]) == -1)        
        errExit("close child_to_parent read");
      printf(" Piping successful. Closed read end of child_to_parent.\n");

      break;
  }

  exit(EXIT_SUCCESS);

}


void errExit(char *strError)
{
  perror(strError);
  exit(EXIT_FAILURE);
}	
	





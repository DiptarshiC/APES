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


typedef struct DATA
{
  char string[30];
  bool led_state;
} DATA1;



int main(void)
{
  pid_t Child = 0;
  DATA1 data;

     /*File descriptor 1*/	
  int fd1[2];     
   /*file descriptor 2*/
  int fd2[2];     



  if(pipe(fd1) == -1)
    perror(" parent to child pipe");

  if(pipe(fd2) == -1)
    perror("child to parent pipe");

  printf("Parent process is forking child process.\n");

  switch (Child = fork())
  {
     case -1: 
     perror("fork");
     break;

     case 0: 
     printf(" Child process has  Forked.\n");
     if(close(fd2[0]) == -1)      
        perror("couldnt close file properly");

     if(close(fd1[1]) == -1)     
        perror("couldnt close file properly\n");
     printf(" Handled pipe descriptors.\n");

     if((fd1[0], &data, sizeof(DATA1)) == -1)
        perror("read from parent to child");

      printf("the child has received string: %s\n", data.string);
     if(data.led_state==true)
        printf("The state of the led is true\n");
      else if(data.led_state==false)
        printf("The state of the led is false\n");



      if(close(fd1[0]) == -1)    
        perror("close parent_to_child read");
      printf("parent to child closed.\n");

      strcat(data.string, "Hello Diptarshi");
      data.led_state = !data.led_state;

      printf("This is child here.The string is %s \n", data.string);
      if(data.led_state==true)
	printf("The state of the led is true\n");
      else if(data.led_state==false)
	printf("The state of the led is false\n"); 
	

      if(write(fd2[1], &data, sizeof(DATA1)) == -1)
        perror("write child_to_parent");

      if(close(fd2[1]) == -1)      
        perror("close child_to_parent write");
      printf("  Closing write end of child_to_parent.\n");

      break;

    default: 
      if(close(fd2[1]) == -1)  
        perror("close child_to_parent write");

      if(close(fd1[0]) == -1)
        perror("close parent_to_child read");
      printf(" Handled pipe descriptors.\n");

      strcpy(data.string, "Hi again");
      data.led_state = false;

      printf(" The string is: %s\n", data.string);
      if(data.led_state==true)
	{
	printf("The state of the led is true\n");
	}
      else if(data.led_state==false)
	{
       printf("The state of the led is false\n");	
        }

      if(write(fd1[1], &data, sizeof(DATA1)) == -1)
        perror("write parent_to_child");

      if(close(fd1[1]) == -1)       
        perror("close parent_to_child write");
      printf(" successfully Closed write end of parent_to_child.\n");

      if(read(fd2[0], &data, sizeof(DATA1)) == -1)
        perror("read child_to_parent");

      printf(" Received string: %s\n", data.string);
      if(data.led_state==true)
	printf("The state of the LED is true\n");
      else(data.led_state==false);
	printf("The state of the LED is false");


      if(close(fd2[0]) == -1)        
        perror("close child_to_parent read");
      printf("Closed read end of child_to_parent.\n");

      break;
  }

  exit(EXIT_SUCCESS);

}


	
	





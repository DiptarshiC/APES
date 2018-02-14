/**
 *@filename: pthread_practice.c
 *
 *@Description: A parent thread creates two child thread.The job of
 *child thread 1 is to look for a character which has occured 3 times
 *in a .txt file.The file i am using here is the valentinesday.txt file
 *which is present in the HW3 directory.The search i have used is a
 *linked list search.The second child task prints CPU usage stats
 *every 100 msecs. All these three tasks write to a common file
 *whose name is accepted at the beginning. To see that there is no
 *corruption in file data, i am passing the data to the individual
 *threads inside a common structure declared globally. When the individual
 *file write operations occur, the file is locked using a mutex lock, opened
 *written to and closed again. The mutex is then unlocked
 *
 *@Date:2/11/2018
 *
 *@Author: Diptarshi Chakraborty
 */

#include<errno.h>
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<signal.h>
#include<sys/time.h>
#include <string.h>

#define 	CLOCKID 	CLOCK_REALTIME
#define 	SIG 		SIGRTMIN

/*The macro below stores the number of child tasks*/
#define 	NUM_CHILD	2


/**
*Below is a structure to pass arguments to a thread.
*The structure has been declared globally so
*that all threads may have access to them.
*To make the operations thread safe, I have
*used a mutex lock to ensure that the operations
*are atomic.The structure has two fields. The 1st
*is the job_code which allows it to perform operations
*based on whether it is the first child thread
*or the second. The second is the pointer to the file
*handle which these individual child tasks write
*to.
*/

struct thread_info{

  int job_code;
  FILE *fptr;

  };

/*An array of strucutres is being created.
This array shall be passed to the individual threads*/
struct thread_info myinfo[NUM_CHILD];

void periodic_task(int signum)
{


	printf("I am printing stats \n");

		FILE *fptr;
		char c;
		//opening of a file
		fptr=fopen("/proc/stat","r");
		if(fptr==NULL)
		{

			printf("Error, Cannot open the file\n");
			exit(0);

		}

		//Read the contents from the file

		c=fgetc(fptr);
		while(c !=EOF)

		{

			printf("%c",c);
			c=fgetc(fptr);
		}

		fclose(fptr);

}
/**
*@Function pointer name:child_play
*
*@parameters:void
*
*@return:none
*
*@Description:Based on the parameters being passed
*to this function pointer, child play will either
*find the character which has three occurences in a 
*.txt file or will print CPU stats every 100 msecs. All
*this will be printed to a common log file whose name
*will be entered by the user when the main thread begins
*
*/

void * child_play(void * parm)
{
  	int *jobname;
  	jobname=(int*) parm;
	printf("I am child task (%d) \n",*(jobname));
	printf("My thread id is (%ld)\n",pthread_self());


 	if(*(jobname)==1)
    	{
		//printf("This is child task 1\n");
		printf("My job is to search text in a file\n");
    	}
  	else if(*(jobname)==2)
    	{
		//printf("This is child task 2\n");
		printf("My job is to print CPU stats\n");

		struct sigaction sa;
		struct itimerval timer;


		/*Install the periodic task as the signal handler for SIGVTALRM*/

		memset(&sa,0,sizeof(sa));
		sa.sa_handler=&periodic_task;
		sigaction(SIGVTALRM,&sa,NULL);

		/*Configure the timer to expire after 100 msec*/

		timer.it_value.tv_sec=0;
		timer.it_value.tv_usec=100000;

		/*and every 100 msec after that*/

		timer.it_interval.tv_sec=0;
		timer.it_interval.tv_usec=100000;


		/*Starts a virtual timer.It counts down whenever this process is executing*/

		setitimer(ITIMER_VIRTUAL,&timer,NULL);

		/*Do busy work*/
		while(1);

   	}


}

void main()
{


	printf("I am a parent thread\n");
	printf("My thread id is (%ld)\n",pthread_self());

	timer_t timerid;
	struct sigevent sev;
	struct itimerspec its;



	int thread_status;//This variable stores the status of a thread
	pthread_t thread1,thread2;//The first and the second child thread
	int thread_stat;

	int jobcode1=1;//Depending on the job codes the child thread does its job.This is the job code for child thread1
	int jobcode2=2;//Depending onn the job code the child thread does its job.This is the job code for child thread2

	printf("This is parent task \n");
        printf("My job is to create two child tasks\n");


	/*The line below creates the first thread*/
	thread_status=pthread_create(&thread1,NULL,child_play,(void*) &jobcode1);

	if(thread_status)
  	{
    		fprintf(stderr,"Error, pthread_create returned code: %d\n",thread_status);
    		exit(EXIT_FAILURE);
  	}



	/*The line below creates the second thread*/
	thread_status=pthread_create(&thread2,NULL,child_play,(void*) &jobcode2);

	if(thread_status)
  	{
    		fprintf(stderr,"Error, pthread_create returned code: %d\n",thread_status);
		exit(EXIT_FAILURE);
  	}


	/* The code below ensures that the main thread or
	the father thread is waiting till the child threads,
	both thread1 and thread2 get completed*/

	thread_status=pthread_join(thread1,(void*)&thread_stat);

	if(thread_status<0)
  	{
    		perror("Error, joining operation failed");

  	}


	thread_status=pthread_join(thread2,(void*)&thread_stat);


	if(thread_status<0)
  	{
     		perror("Error, joining operation failed");
  	}



	/* The code below ensures that thread1 and
	thread2 are killed by SIGUSR1,
	*/

	thread_status=pthread_kill(thread1,SIGUSR1);

	if(thread_status<0)
  	{
    		perror("Error, Kill failed");

  	}


	thread_status=pthread_kill(thread2,SIGUSR1);


	if(thread_status<0)
  	{
     		perror("Error, Kill failed");
  	}

}






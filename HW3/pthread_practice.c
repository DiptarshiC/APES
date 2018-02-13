/**
 *@filename: pthread_practice.c
 *
 *@description
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


/*struct thread_info{
  
  

  };*/


void periodic_task(int signum)
{

	static int count =0;
	printf("I am printing stats %d\n",++count);
	

}

void * child_play(void * parm)
{
  int *jobname;
  jobname=(int*) parm;
  if(*(jobname)==1)
    {
	printf("This is child task 1\n");
	printf("My job is to search text in a file\n");
    }
  else if(*(jobname)==2)
    {
	/*printf("This is child task 2\n");
	printf("My job is to print CPU stats\n");*/

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






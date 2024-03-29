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
#include<stdint.h>
#include<sys/syscall.h>

 struct node{

        char car;
        uint32_t count;
        struct node *next;
        };


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
  char filename[20];

  };

/*An array of structures is being created.
This array shall be passed to the individual threads
Here the File *ptr will be common for both the threads
The task no. for 1 will be 1 and for the other will be 2
*/
struct thread_info myinfoarray[NUM_CHILD];

pthread_t thread1,thread2;//The first and the second child thread


/*This is a globally defined mutex
that allows us to lock resources
I have named it so because it is
use to lock a file
*/
pthread_mutex_t mutexfile;


/*This is a the object for
posix timer functions
*/

struct timespec tms;

void handle_signal(int signal)
{

	FILE *ptr=fopen(myinfoarray[1].filename,"a");
	printf("Bye bye.Child thread 2 terminating \n\n");
	fprintf(ptr,"Bye bye.Child thread 2 terminating \n\n");
	fclose(ptr);
	pthread_cancel(thread2);


}


void periodic_task(int signum)
{

	FILE *ptr2=fopen(myinfoarray[1].filename,"a");
	printf(".This child thread 2 running.I am printing stats \n");
	fprintf(ptr2,".This child thread 2 running.I am printing stats \n");

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
			fprintf(ptr2,"%c",c);
			c=fgetc(fptr);
		}

		printf("\n\n");
		fprintf(ptr2,"\n\n");

		fclose(fptr);
		fclose(ptr2);

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


	struct thread_info *MY_INFO;

	MY_INFO=(struct thread_info*) parm;
	/*Install the periodic task as the signal handler for SIGUSR1*/

	struct sigaction sa1;

	/*Setup signal handler*/
	sa1.sa_handler=&handle_signal;

	/*Restart the thread if at all not possible*/
	sa1.sa_flags=SA_RESTART;

	/*Block every other signal handler*/
	sigfillset(&sa1.sa_mask);


	if(sigaction(SIGUSR1,&sa1,NULL)==-1)
	{

		perror("Error: Cannot handle SIGUSR1\n");

	}
 	if(MY_INFO->job_code==1)
    	{
	//	 pthread_mutex_lock(&mutexfile);

		FILE *ptr=fopen(MY_INFO->filename,"a");

		//printf("This is child task 1\n");
		//fprintf(ptr,"My job is to search text in a file\n");
		printf("I am child thread (%d) \n",MY_INFO->job_code);
		fprintf(ptr,"I am child thread (%d) \n",MY_INFO->job_code);
       		printf("My POSIX thread id is (%ld)\n",pthread_self());
		fprintf(ptr,"My POSIX thread id is (%ld)\n",pthread_self());
		printf("My linux thread id is (%ld)\n",syscall(__NR_gettid ));
                fprintf(ptr,"My linux thread id is (%ld)\n",syscall(__NR_gettid ));
		printf("My pid is (%d)\n",getpid( ));
	        fprintf(ptr,"My pid is (%d)\n",getpid( ));
		printf("My job is to search text in a file\n");
		fprintf(ptr,"My job is to search text in a file\n");

		printf("\n");
		printf("\n");
		fprintf(ptr,"\n");
		fprintf(ptr,"\n");
		char c;
	char newchar;
	char flag=0;

	FILE *PTR=fopen("textcopy.txt","a");
	FILE *FPTR=fopen("Valentinesday.txt","r");

	c=fgetc(FPTR);
	struct node *headnode;
	headnode = (struct node*)malloc(sizeof(struct node));
	headnode->count=1;
	headnode->car=c;
	headnode->next=NULL;
	struct node *currentnode;
	currentnode = (struct node*)malloc(sizeof(struct node));

	while(c!=EOF)
	{
			c=fgetc(FPTR);
			flag=0;
			currentnode=headnode;

			while(currentnode->next!=NULL)
			{
				if((c==(currentnode->car))||(c==(currentnode->car+32))||(c==(currentnode->car-32)))
				{
					(currentnode->count)=(currentnode->count)+1;
					flag=1;
					break;
				}
				else
				{
			 		currentnode=currentnode->next;
				}
			}
				if(flag==1)
				{

					continue;
				}


				//I move forward 
				if(currentnode->next==NULL)
				{

					if((c==(currentnode->car))||(c==(currentnode->car-32))||(c==(currentnode->car+32)))
                                	{
                                        	(currentnode->count)=(currentnode->count)+1;

						continue;

                                	}

					else
					{
                        			//create new node
                        			struct node *nextnode;
                        			nextnode = (struct node*)malloc(sizeof(struct node));
                        			nextnode->car=c;
                        			nextnode->count=1;
						nextnode->next=NULL;
                        			currentnode->next=nextnode;
						continue;
					}

                 		}




		fprintf(ptr,"%c",c);
		//c=fgetc(fptr);
	}

	fclose(FPTR);
	fclose(PTR);

	/*Now to check the count

	printf("Charactrer \t Count\n");
	currentnode=headnode;

	while(currentnode->next!=NULL)
	{

		printf("%c \t          %d\n",currentnode->car,currentnode->count);
		currentnode=currentnode->next;

	}
		printf("\n\n");*/

	     /*Now to check the count*/
	
        printf("This is thread 1 running \n");
	fprintf(ptr,"This is thread 1 running \n");
        printf("Charactrer \t Count\n");
	fprintf(ptr,"Charactrer \t Count\n");

        currentnode=headnode;

        while(currentnode->next!=NULL)
        {
		if(currentnode->count==3)
		{
                printf("%c \t          %d\n",currentnode->car,currentnode->count);
		fprintf(ptr,"%c \t          %d\n",currentnode->car,currentnode->count);

		}
                currentnode=currentnode->next;

        }
		printf("\n\n");
		fprintf(ptr,"\n\n");



		fclose(ptr);


 




	//	pthread_mutex_unlock(&mutexfile);


    	}
  	if(MY_INFO->job_code==2)
    	{




		FILE *ptr1=fopen(MY_INFO->filename,"a");


		printf("I am child thread (%d) \n",MY_INFO->job_code);
		fprintf(ptr1,"I am child thread (%d) \n",MY_INFO->job_code);
	        printf("My POSIX thread id is (%ld)\n",pthread_self());
		fprintf(ptr1,"My POSIX thread id is (%ld)\n",pthread_self());
		printf("My linux thread id is (%ld)\n",syscall(__NR_gettid ));
                fprintf(ptr1,"My linux thread id is (%ld)\n",syscall(__NR_gettid ));
		 printf("My pid is (%d)\n",getpid( ));
	        fprintf(ptr1,"My pid is (%d)\n",getpid( ));


		printf("My job is to print CPU stats\n");
		fprintf(ptr1,"My job is to print CPU stats\n");


		printf("\n");
		fprintf(ptr1,"\n");
		printf("\n");
		fprintf(ptr1,"\n");
		fclose(ptr1);
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
	//	fclose(ptr1);
	//	pthread_mutex_unlock(&mutexfile);



   	}
	pthread_exit(NULL);
}


/**
*@Function name:main
*
*@parameters:void
*
*@return:none
*
*@Description:This is the main function. The main
*thread here is the parent thread that creates the
*two children threads.It also takes the file name
*to write to from the user.
*
*/




void main()
{
clock_gettime(CLOCK_REALTIME,&tms);
struct timeval now;
int rc;


	printf("Please enter the name of the file you would like to create\n");
	scanf("%s",myinfoarray[0].filename);

	for(int i=0;i<20;i++)
	{

		myinfoarray[1].filename[i]=myinfoarray[0].filename[i];

	}

	printf("The filename that you entered is\n");

	for(int i=0;i<20;i++)
	{

		printf("%c",myinfoarray[1].filename[i]);
	}

	printf("\n\n");

	myinfoarray[0].job_code=1;
	myinfoarray[1].job_code=2;

//	pthread_mutex_lock(&mutexfile);



//	fclose(ptr);
//	pthread_mutex_unlock(&mutexfile);


	timer_t timerid;
	struct sigevent sev;
	struct itimerspec its;



	int thread_status;//This variable stores the status of a thread
	//pthread_t thread1,thread2;//The first and the second child thread
	int thread_stat;

	int jobcode1=1;//Depending on the job codes the child thread does its job.This is the job code for child thread1
	int jobcode2=2;//Depending onn the job code the child thread does its job.This is the job code for child thread2


	FILE *ptr=fopen(myinfoarray[0].filename,"a");

	printf("This is parent thread \n");
	fprintf(ptr,"This is parent thread \n");
	printf("My POSIX thread id is (%ld)\n",pthread_self());
	fprintf(ptr,"My POSIX thread id is (%ld)\n",pthread_self());
	printf("My linux thread id is (%ld)\n",syscall(__NR_gettid ));
        fprintf(ptr,"My linux thread id is (%ld)\n",syscall(__NR_gettid ));
	printf("My pid is (%d)\n",getpid( ));
        fprintf(ptr,"My pid is (%d)\n",getpid( ));




        printf("My job is to create two child threads\n");
	printf("\n");
	printf("\n");

	fprintf(ptr,"My job is to create two child threads\n");
	fprintf(ptr,"\n");
        fprintf(ptr,"\n");
	fclose(ptr);


	pthread_mutex_init(&mutexfile,NULL);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	/*The line below creates the first thread*/
	thread_status=pthread_create(&thread1,&attr,child_play,(void*) &myinfoarray[0]);

	if(thread_status)
  	{
    		fprintf(stderr,"Error, pthread_create returned code: %d\n",thread_status);
    		exit(EXIT_FAILURE);
  	}
	else
	{
		FILE *ptra=fopen(myinfoarray[0].filename,"a");
		rc=gettimeofday(&now,NULL);
                printf("Creating child thread 1\n");
		fprintf(ptra,"Creating child thread1\n");
		printf("Timestamp for thread creation:%u.%06u\n\n",now.tv_sec,now.tv_usec);
		fprintf(ptra,"Timestamp for thread creation:%u.%06u\n\n",now.tv_sec,now.tv_usec);




		fclose(ptra);


        }

	/*The line below creates the second thread*/
	thread_status=pthread_create(&thread2,&attr,child_play,(void*) &myinfoarray[1]);

	if(thread_status)
  	{
    		fprintf(stderr,"Error, pthread_create returned code: %d\n",thread_status);
		exit(EXIT_FAILURE);
  	}

	 else
        {
		FILE *ptra=fopen(myinfoarray[1].filename,"a");
		rc=gettimeofday(&now,NULL);
                printf("Creating  child thread 2!\n");
		fprintf(ptra,"Creating child thread2\n");
		printf("Timestamp for thread creation:%u.%06u\n\n",now.tv_sec,now.tv_usec);
                fprintf(ptra,"Timestamp for thread creation:%u.%06u\n\n",now.tv_sec,now.tv_usec);


		 fclose(ptra);



        }


	pthread_attr_destroy(&attr);
	/* The code below ensures that the main thread or
	the father thread is waiting till the child threads,
	both thread1 and thread2 get completed*/

	thread_status=pthread_join(thread1,(void*)&thread_stat);

	if(thread_status<0)
  	{
    		perror("Error, joining operation failed");

  	}
	else
	{
		FILE *ptra=fopen(myinfoarray[0].filename,"a");

		rc=gettimeofday(&now,NULL);


		printf("Goodbye from thread 1!\n");

		fprintf(ptra,"Goodbye from thread 1!\n");

		printf("Timestamp for thread destruction:%u.%06u\n\n",now.tv_sec,now.tv_usec);
                fprintf(ptra,"Timestamp for thread destruction:%u.%06u\n\n",now.tv_sec,now.tv_usec);




		fclose(ptra);
	}


	thread_status=pthread_join(thread2,(void*)&thread_stat);


	if(thread_status<0)
  	{

		 
     		perror("Error, joining operation failed");
  	}

	 else
        {
		FILE *ptrb=fopen(myinfoarray[1].filename,"a");

		rc=gettimeofday(&now,NULL);

                printf("Goodbye from thread 2!\n");
		fprintf(ptrb,"Goodbye from thread 2!\n");

		printf("Timestamp for thread destruction:%u.%06u\n\n",now.tv_sec,now.tv_usec);
                fprintf(ptrb,"Timestamp for thread destruction:%u.%06u\n\n",now.tv_sec,now.tv_usec);


		fclose(ptrb);


        }



/*	 The code below ensures that thread1 and
	thread2 are killed by SIGUSR1,*/


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






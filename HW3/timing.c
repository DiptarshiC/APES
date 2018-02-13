#include<stdio.h>
#include<string.h>
#include<time.h>
#include<signal.h>
#include<sys/time.h>

/**
*ill make this variable global so that
*the alarm handler variable can change
*the interval value neccessary
*/

//struct itimerval it;

/*Put stuff here to perform the periodic task.
* Note  that you might have to change the values
* in the itimerval struct to compensate
* for variations in "real" time of the day 
* time due to inexactness of the linux
* system time versus the wall clock time
*/

void periodic_task(int signum)
{

	static int count =0;
	printf("Periodic task in C timer %d\n",++count);
	printf("Diptarshi u r such a stud!\n");

}
int main()
{
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








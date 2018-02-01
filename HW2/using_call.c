#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <linux/kernel.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
void main()

{

	printf("Pid of calling process= %d\n", getpid());

	printf("Pid of the parent process =%d\n", getppid());


	pid_t getpid(void);
	struct timeval time;
	char buffer[40];
	gettimeofday(&time, NULL);

	strftime(buffer, 40, "Current date/time: %m-%d-%Y/%T",localtime(&time.tv_sec));

	printf("%s\n",buffer);
	int32_t *arr1=(int32_t*) malloc(sizeof(int32_t)*300);
	int32_t *arr3=(int32_t*) malloc(sizeof(int32_t)*300);


	for(long i=0;i<300;i++)
	{
		*(arr1+i)=i+1;
	}
	printf("The values the unsorted array are as follows:");

	for (long j=0;j<300;j++)

        {
                printf("%d\n;",*(arr1+j));
        }

 	syscall(385,arr1,300,arr3);

	printf("The values the sorted array are as follows:\n");

	for (long j=0;j<300;j++)

	{
		printf("%d\n",*(arr3+j));
	}
}

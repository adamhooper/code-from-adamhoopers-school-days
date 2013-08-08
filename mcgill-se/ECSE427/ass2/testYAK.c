#include <stdio.h>
#include <unistd.h>

#include "YAKThreads.h"

void testthread_1()
{
	int i;
	for (i=0; i<2; i++)
	{
		printf("Hello from thread[type 1] %d, now i = %d \n", getYAKThreadid(), i);
		sleep(1);
	}

	printf("Thread %d exitting\n",  getYAKThreadid());
	closeYAKThread();
}


void testthread_2()
{
	int i;
	for (i=0; i<2; i++)
	{
		printf("Hello from Other thread[type 2] %d, now i = %d \n", getYAKThreadid(),i);
		sleep(1);
	}

	printf("Thread %d exitting\n",  getYAKThreadid());
}

void testthread_3()
{
	int i;
	for (i=0; i< 3; i++)
	{
		printf("Hello from Other thread[type 3] %d, now i = %d \n", getYAKThreadid(),i);
		sleep(1);
	}

	printf("Thread %d exitting\n",  getYAKThreadid());
	closeYAKThread();
}


int main(void)
{
	int i;


	initYAKThreads();
	for (i=0; i< 10; i++){

		YAKSpawn( testthread_1 );
		YAKSpawn( testthread_1 );
		YAKSpawn( testthread_2 );
		YAKSpawn( testthread_2 );
		YAKSpawn( testthread_3 );
		YAKSpawn( testthread_3 );

		sleep(1);
		printf("The parent can do something too while the children are running\n");

		YAKWaitall();
		printf("All child threads in phase %d terminated\n", i);
	}

	return 0;
}

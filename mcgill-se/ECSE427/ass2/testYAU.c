#include <stdio.h>
#include <unistd.h>
#include "YAUThreads.h"

void testthread()
{
	int i;

	for (i = getYAUThreadid(); i > -4; i--)
	{
		printf("Hello from thread %d \n", getYAUThreadid());
		sleep(1);
	}

	printf ("Goodbye from thread %d\n", getYAUThreadid());
 
	closeYAUThread ();
}
 
void testthread_other()
{
	int i;

	for (i = 0; i < 3; i++)
	{
		printf("Hello from Other thread %d \n", getYAUThreadid());
		sleep(1);
	}

	printf ("Goodbye from thread %d\n", getYAUThreadid());

	closeYAUThread ();
} 
 
int main()
{
	initYAUThreads();
	YAUSpawn(testthread, 1);
	YAUSpawn(testthread, 2);
	YAUSpawn(testthread, 3);
	YAUSpawn(testthread_other, 4);

	startYAUThreads(PRIORITY);

	YAUWaitall();

	return 0;
}

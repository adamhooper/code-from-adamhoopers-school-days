#include <stdio.h>
#include <unistd.h>

#include "YAUThreads.h"

void testthread_1(void)
{
  int i;
        for (i=0; i<2; i++)
        {
                printf("Hello from thread[type 1] %d, now i = %d \n", getYAUThreadid(), i);
                sleep(1);
        }

	printf("Thread %d exitting\n",	getYAUThreadid());
	closeYAUThread();
}
 

void testthread_2()
{
  int i;
        for (i=0; i< 5; i++)
        {
                printf("Hello from Other thread[type 2] %d, now i = %d \n", getYAUThreadid(),i);
                sleep(1);
        }

	printf("Thread %d exitting\n",	getYAUThreadid());
	closeYAUThread();
} 
 
void testthread_3()
{
  int i;
        for (i=0; i< 3; i++)
        {
                printf("Hello from Other thread[type 3] %d, now i = %d \n", getYAUThreadid(),i);
                sleep(1);
        }

	printf("Thread %d exitting\n",	getYAUThreadid());
	closeYAUThread();
} 


int main(void)
{
  int i;

       
  initYAUThreads();
  
  for (i=0; i< 5; i++){
	
	YAUSpawn( testthread_1,0 /* priority */ );
	YAUSpawn( testthread_3,0 );
	YAUSpawn( testthread_2,0 );
	YAUSpawn( testthread_3,0 );
	YAUSpawn( testthread_1,0 );
	YAUSpawn( testthread_2,0 );

	/*should execute in the given order */
	startYAUThreads(FCFS);
	YAUWaitall();

	printf("All child threads in phase %d FCFS terminated\n", i);

	YAUSpawn( testthread_1,3 /* priority */ );
	YAUSpawn( testthread_3,2 );
	YAUSpawn( testthread_2,5 );
	YAUSpawn( testthread_3,1 );
	YAUSpawn( testthread_1,5 );
	YAUSpawn( testthread_2,2 );

	/*should execute in the given priority order (higher priority first) */
	startYAUThreads(PRIORITY);
	YAUWaitall();
	printf("All child threads in phase %d Priority Scheduling terminated\n", i);

  }

  return 0;
}

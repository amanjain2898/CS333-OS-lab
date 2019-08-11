#include <pthread.h>
#include <stdio.h>

#define NTHREADS 100
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

long int counter = 0;

//	The thread starter function
void *myThread()
{
	for(int i=0; i<1000; i++){
	    pthread_mutex_lock( &mutex1 );	
		counter++;
	    pthread_mutex_unlock(&mutex1);
	}
}

int main()
{
	// Create space for pthread variables
	pthread_t tid[NTHREADS];

	for(int i=0; i<NTHREADS; i++)
	{
		//	Create a thread with default attributes and no arguments

		pthread_create(&tid[i], NULL, myThread, NULL);
	}

	for(int i=0; i<NTHREADS; i++)
	{
		//	Wait for all threads to finish
		pthread_join(tid[i], NULL);
	}

	printf("Counter: %ld \n", counter);

	pthread_mutex_destroy(&mutex1);

	return 0;
}

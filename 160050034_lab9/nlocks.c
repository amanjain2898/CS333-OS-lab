#include <pthread.h>
#include <stdio.h>

#define NTHREADS 10
// pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex[10];
long int counter[10];

//	The thread starter function
void *myThread(void* index)
{
	int *ind = (int*)index;
	for(int i=0; i<1000; i++){
		pthread_mutex_lock(&mutex[*ind]);	
		counter[*ind]++;
	    pthread_mutex_unlock(&mutex[*ind]);
	}
}

int main()
{
	for(int i=0;i<10;i++)
		counter[i] = 0;


	for(int i=0;i<10;i++)
		pthread_mutex_init(&mutex[i],NULL);

	// Create space for pthread variables
	pthread_t tid[NTHREADS];

	int num[10];

	for(int i=0;i<10;i++)
		num[i] = i;


	for(long int i=0; i<NTHREADS; i++)
	{
		//	Create a thread with default attributes and no arguments

		pthread_create(&tid[i], NULL, myThread, (void*)&num[i]);
	}

	for(int i=0;i<1000;i++){
		for(int j=0;j<10;j++){
			pthread_mutex_lock(&mutex[j]);	
			counter[j]++;
		    pthread_mutex_unlock(&mutex[j]);
		}
	}

	for(int i=0; i<NTHREADS; i++)
	{
		//	Wait for all threads to finish
		pthread_join(tid[i], NULL);
	}

	for(int i=0;i<10;i++)
		printf("Counter: %ld \n", counter[i]);


	return 0;
}

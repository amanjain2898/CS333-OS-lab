#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

pthread_cond_t cond_read  = PTHREAD_COND_INITIALIZER;;
pthread_cond_t cond_write  = PTHREAD_COND_INITIALIZER;

struct arg_struct{
	int arg1;
	int arg2;
};


struct read_write_lock
{
	// If required, use this strucure to create
	// reader-writer lock related variables
	pthread_mutex_t read_lk;
	pthread_mutex_t write_lk;
	int nreaders;
	int nwriters;
	int wait_writers;

}rwlock;

long int data = 0;			//	Shared data variable

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
	pthread_mutex_init(&rw->read_lk,NULL);
	pthread_mutex_init(&rw->write_lk,NULL);
	rw->nreaders=0;
	rw->nwriters=0;
	rw->wait_writers=0;
}


// The pthread based reader lock
void ReaderLock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->read_lk);
	while(rw->nwriters > 0 || rw->wait_writers > 0){
		pthread_cond_wait(&cond_read,&rw->read_lk);
	}
	rw->nreaders++;
	pthread_mutex_unlock(&rw->read_lk);

}	

// The pthread based reader unlock
void ReaderUnlock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->read_lk);
	rw->nreaders--;
	if(rw->nreaders == 0){
		pthread_cond_broadcast(&cond_write);
	}
	pthread_mutex_unlock(&rw->read_lk);
}

// The pthread based writer lock
void WriterLock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->write_lk);
	rw->wait_writers++;
	while(rw->nreaders > 0 || rw->nwriters > 0){
		pthread_cond_wait(&cond_write,&rw->write_lk);
	}
	rw->wait_writers--;
	rw->nwriters++;
	pthread_mutex_unlock(&rw->write_lk);	
}

// The pthread based writer unlock
void WriterUnlock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->write_lk);
	rw->nwriters--;
	pthread_cond_broadcast(&cond_write);
	pthread_cond_broadcast(&cond_read);
	pthread_mutex_unlock(&rw->write_lk);	
}

//	Call this function to delay the execution by 'delay' ms
void delay(int delay)
{
	struct timespec wait;

	if(delay <= 0)
		return;

	wait.tv_sec = delay / 1000;
	wait.tv_nsec = (delay % 1000) * 1000 * 1000;
	nanosleep(&wait, NULL);
}

// The pthread reader start function
void *ReaderFunction(void *args)
{
	struct arg_struct *arg_inp;
	arg_inp = (struct arg_struct*)args;
	delay(arg_inp->arg2);

	ReaderLock(&rwlock);
	printf("Thread:  %d, data: %li\n",arg_inp->arg1,data);
	delay(0.0001);
	ReaderUnlock(&rwlock);
}

// The pthread writer start function
void *WriterFunction(void *args)
{
	struct arg_struct *arg_inp;
	arg_inp = (struct arg_struct*)args;
	delay(arg_inp->arg2);

	WriterLock(&rwlock);
	data = data+1;
	printf("Thread:  %d, data: %li\n",arg_inp->arg1,data);
	delay(0.0001);
	WriterUnlock(&rwlock);
}

int main(int argc, char *argv[])
{
	
	long int reader_number = 0;
	long int writer_number = 0;
	long int thread_number = 0;
	long int total_threads = 0;	
	
	int count = 0;			// Number of 3 tuples in the inputs.	Assume maximum 10 tuples 
	int rws[10];			
	int nthread[10];
	int delay[10];

	// struct arg_struct *args;

	//	Verifying number of arguments
	if(argc<4 || (argc-1)%3!=0)
	{
		printf("reader-writer <r/w> <no-of-threads> <thread-arrival-delay in ms> ...\n");		
		printf("Any number of readers/writers can be added with repetitions of above mentioned 3 tuple \n");
		exit(1);
	}

	//	Reading inputs
	for(int i=0; i<(argc-1)/3; i++)
	{
		char rw[2];
		strcpy(rw, argv[(i*3)+1]);

		if(strcmp(rw, "r") == 0 || strcmp(rw, "w") == 0)
		{
			if(strcmp(rw, "r") == 0)
				rws[i] = 1;					// rws[i] = 1, for reader
			else
				rws[i] = 2;					// rws[i] = 2, for writer
			
			nthread[i] = atol(argv[(i*3)+2]);		
			delay[i] = atol(argv[(i*3)+3]);

			count ++;						//	Number of tuples
			total_threads += nthread[i];	//	Total number of threads
		}
		else
		{
			printf("reader-writer <r/w> <no-of-threads> <thread-arrival-delay in ms> ...\n");
			printf("Any number of readers/writers can be added with repetitions of above mentioned 3 tuple \n");
			exit(1);
		}
	}

	pthread_t threads[total_threads];

	int r=0,w=0,x=0;
	for(int i=0;i<count;i++){
		for(int j=0;j<nthread[i];j++){
			struct arg_struct *args;
			args = malloc(sizeof(struct arg_struct));
			args->arg2 = delay[i];			
			if(rws[i] == 1) {
				args->arg1 = r;
				pthread_create(&threads[x], NULL, ReaderFunction, (void*)args);
				r++;
			}
			else {
				args->arg1 = w;
				pthread_create(&threads[x], NULL, WriterFunction, (void*)args);
				w++;
			}
			x++;
		}
	}

	for(int i=0; i<x; i++)
	{
		//	Wait for all threads to finish
		pthread_join(threads[i], NULL);
	}

	exit(0);
}

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>


#include <queue>

//Initialize Queue
std::queue<long unsigned> myqueue;

//Initialize static Mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
long unsigned sum = 0;


//Method for the threads
void *consumer(void *arg){


	printf("Thread created\n");


	long tid = syscall(SYS_gettid);
	long unsigned tmp;
	int flag = 1;

	while(flag){
		//lock area
		pthread_mutex_lock(&lock);
		if(myqueue.size() > 0){
			
			//pop the values from the queue and add it to local variable
			tmp = myqueue.front();
			myqueue.pop();
			if(tmp == 0){
				flag = 0;
			}
			else{
				sum += tmp;
			}

		}
		//unlock area
		pthread_mutex_unlock(&lock);
	}


	printf("Thread: %li, SUM: %lu\n", tid, sum);

	return arg;
}



int main(void){

	int check;
	pthread_t thread_id[4];


	//create 4 consumers
	for(int i = 0; i < 4; i++){

		check = pthread_create(&thread_id[i], NULL, &consumer, NULL);
		if(check == -1){
			perror("creating thread\n");
		}

	}

	//push value 1 100000 times in the queue
	for(long unsigned i = 0; i < 100000; i++){
		pthread_mutex_lock(&lock);
		myqueue.push(1);
		pthread_mutex_unlock(&lock);	
	}

	
	//+4 Zeros
	pthread_mutex_lock(&lock);
	myqueue.push(0);
	myqueue.push(0);
	myqueue.push(0);
	myqueue.push(0);
	pthread_mutex_unlock(&lock);	


	//wait for the consumers
	printf("Producer is waiting\n");
	for(int i = 0; i < 4; i++){
		check = pthread_join(thread_id[i], NULL);
		if(check != 0){
			perror("pthread join\n");
		}	
	}

	return EXIT_SUCCESS;
}




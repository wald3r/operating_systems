#define _GNU_SOURCE        
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>

int number_of_threads;
int number_of_allocations;
int size_in_byte;


struct memory{

	char *size;

};



//thread method to allocate memory
void *memory_allocation(void *arg){

	struct memory * x[number_of_allocations];
	
	long tid = syscall(SYS_gettid);

	//allocate struct and size of "char *size"
	for(int i = 0; i < number_of_allocations; i++){
		x[i] = malloc(sizeof(struct memory));
		if(x[i] != 0){
			x[i]->size = (char*) malloc(sizeof(char) * size_in_byte);
		}		
	}

	
	size_t mem = 0;

	//count allocated memory
	for(int i = 0; i < number_of_allocations; i++){
		for(int j = 0; j < size_in_byte; j++){
			mem += sizeof(x[i]->size[j]);
		}
	}
	printf("Thread %ld has allocated %lu bytes!\n", tid, mem);


	//free memory
	for(int i = 0; i < number_of_allocations; i++){
		free(x[i]);
	}
	return arg;
}



int main(int argc, char *argv[]){


	int check = 0;
	number_of_threads = atoi(argv[1]);
	number_of_allocations = atoi(argv[2]);
	size_in_byte = atoi(argv[3]);

	pthread_t thread_id[number_of_threads];

	printf("Create threads\n");
	//Create threads
	for(int i = 0; i < number_of_threads; i++){

		check = pthread_create(&thread_id[i], NULL, &memory_allocation, NULL);
		if(check == -1){
			perror("creating thread\n");
		}

	}


	//wait for all threads
	for(int i = 0; i < number_of_threads; i++){
		check = pthread_join(thread_id[i], NULL);
		if(check != 0){
			perror("pthread join\n");
		}	
	}


	return EXIT_SUCCESS;
}

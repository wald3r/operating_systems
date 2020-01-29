#define _GNU_SOURCE        
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>

//Informations for the cleanup Handler
struct cleanup_info{
	int *id;
	FILE *f;
};


//Close File when it is not already closed
void cleanup(void *arg){
	
	struct cleanup_info *ptr = (struct cleanup_info*)arg;
	printf("Cleanup Handler: %i\n", *ptr->id);
	if(ptr->f != NULL){	
		fclose(ptr->f);
	}	
}


//create a .txt file and write the thread_id into it.
void *create_file(void *arg){

	struct cleanup_info *cleanup_ptr = (struct cleanup_info*)malloc(sizeof(struct cleanup_info));
	FILE *f;

	
	cleanup_ptr->id = (int*)arg;
	cleanup_ptr->f = f;

	printf("Thread %d started\n", *cleanup_ptr->id); 

	//Set up cleanup Handler	
	pthread_cleanup_push(cleanup, cleanup_ptr);

	//timer
	int time;
	sleep(time = rand()%3);

	//create filename
	char buffer[9];
	sprintf(buffer, "text%i.txt", *cleanup_ptr->id);

	//create file
	f = fopen(buffer, "w");
	if(f == NULL){
		perror("opening file\n");
	}

	//workaround -> gettid alone doesn't work with gclib
	long tid = syscall(SYS_gettid);
	//print into file
	fprintf(f, "ID: %ld\n", tid);


	//close file
	fclose(f);
	printf("Thread %d is done.\n", *cleanup_ptr->id); 
	
	//Delete cleanup Handler
	pthread_cleanup_pop(0);

	free(cleanup_ptr);

	return NULL;
}


//values to pass to the threads
struct value{
	
	int number[10];
};




int main(void){

	int check;
	pthread_t thread_id[10];
	struct value *pointer = (struct value*)malloc(sizeof(struct value));

	srand(time(NULL));

	
	for(int i = 0; i < 10; i++){
		pointer->number[i] = i;
	}



	//Create 10 threads
	for(int i = 0; i < 10; i++){

		check = pthread_create(&thread_id[i], NULL, &create_file, &pointer->number[i]);
		if(check == -1){
			perror("creating thread\n");
		}

	}

	//Generate a random number and cancel the thread with the generated number
	for(int i = 0; i < 5; i++){
		int cancel = rand()%9;
		printf("Cancel following Thread: %i\n", cancel);
		check = pthread_cancel(thread_id[cancel]);
		if(check != 0){
			fprintf(stderr, "cancel failed: %i\n", cancel);
		}
	}
	
	//wait for all threads
	for(int i = 0; i < 10; i++){
		int check = pthread_join(thread_id[i], NULL);
		if(check != 0){
			perror("pthread join\n");
		}	
	}


	free(pointer);
	
	return EXIT_SUCCESS;
}

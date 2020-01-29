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
long tid;

//list
struct memory{
	
	char *addr;
	int size;
	struct memory *next;
};

//root
struct allocator{

	int count;
	char *max_memory;
	int size;
	struct memory *next;

};


//Initialise allocator
void allocator_init(struct allocator *ptr, int size){

	ptr->count = 0;
	ptr->size = size;
	ptr->max_memory =  (char*) malloc(sizeof(char) * (size));
	ptr->next = NULL;

}

//free memory 
void allocator_delete(struct allocator *alloc_ptr, int size){

	if(alloc_ptr == 0){
		printf("Nothing to free!\n");
		return;
	}

	
	struct memory *mem_ptr = alloc_ptr->next;
	while(mem_ptr != NULL){
		

		if(mem_ptr->size == size){
			
			mem_ptr->size = 0;
			printf("%d bytes freed\n", size);
			return;
		}	

		mem_ptr = mem_ptr->next;
	}
	
}

//add something to the list
void allocator_add(struct allocator *alloc_ptr, int size){

	struct memory *mem_ptr = NULL;

	if(alloc_ptr->count == 0){
		if(alloc_ptr->size >= size){
			printf("Add Start: %lu\n", size);
			mem_ptr = (struct memory *) malloc(sizeof(struct memory));
			alloc_ptr->next = mem_ptr;
			mem_ptr->size = size;
			mem_ptr->addr = alloc_ptr->max_memory;
			mem_ptr->next = NULL;
			alloc_ptr->count = 1; 
			return;
		}

	}

	tid = syscall(SYS_gettid);	
	mem_ptr = alloc_ptr->next;
	struct memory *next_mem;
	char* begin;
	char* end;
	int free_space = alloc_ptr->size;
	int tmp = 0;

	//find best spot to store the value
	while(mem_ptr != NULL){

		begin = mem_ptr->addr + mem_ptr->size;		
		next_mem = mem_ptr->next;
	
		if(next_mem == NULL){
			end = alloc_ptr->max_memory+alloc_ptr->size;

		}else{
			end = next_mem->addr;
		}


		tmp = (int)(end - begin);

		if(tmp <= free_space && size <= tmp){
		free_space = tmp;
		printf("Space found: %d\n", tmp);		
		}

		mem_ptr = mem_ptr->next;
	}

	if(size > free_space){
		printf("Couldn't find free space\n");
		return;
	}

	//add value to best spot
	mem_ptr = alloc_ptr->next;
	while(mem_ptr != NULL){

		begin = mem_ptr->addr + mem_ptr->size;		
		next_mem = mem_ptr->next;
	
		if(next_mem == NULL){
			end = alloc_ptr->max_memory+alloc_ptr->size;

		}else{
			end = next_mem->addr;
		}

		tmp = (int)(end - begin);
		if(tmp == free_space){
			struct memory *mem_ptr_new = (struct memory *) malloc(sizeof(struct memory));
			mem_ptr_new->size = size;
			mem_ptr_new->addr = begin;
			mem_ptr_new->next = mem_ptr->next;
			alloc_ptr->count += 1;
			mem_ptr->next = mem_ptr_new; 
			printf("Thread %lu has allocated %d byte (%p)\n", tid, mem_ptr_new->size, mem_ptr_new->addr);
			return;
		}
		mem_ptr = mem_ptr->next;

	}
	printf("Couldn't find free space\n");

}

void allocator_free(struct memory *ptr){

	if(ptr != NULL){
		struct memory *mem_ptr = ptr->next;
		free(ptr);
		allocator_free(mem_ptr);
	}

}

//thread method to allocate memory
void *memory_allocation(void *arg){

	struct allocator *ptr = (struct allocator *) malloc(sizeof(struct allocator));

	allocator_init(ptr, (number_of_allocations * size_in_byte));
	


	for(int i = 0; i < number_of_allocations; i++){
		allocator_add(ptr, size_in_byte);
	}


	allocator_free(ptr->next);
	free(ptr);

	return NULL;
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

	printf("Waiting for threads\n");
	//wait for all threads
	for(int i = 0; i < number_of_threads; i++){
		check = pthread_join(thread_id[i], NULL);
		if(check != 0){
			perror("pthread join\n");
		}	
	}

	return EXIT_SUCCESS;
}

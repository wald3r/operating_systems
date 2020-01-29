#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <semaphore.h>

#define SEM "/mysem"

struct shared_memory { 
	int value;
};

int main (void){


	pid_t pid;
	sem_t *sem;


	int fd, len;
	char *RESULT_FIFO = "/tmp/result_fifo";

	struct shared_memory *shared_mem_ptr;


	//open semaphore
	sem = sem_open(SEM, O_CREAT, 0777, 1);
	
	//unlink when it is already linked
	shm_unlink("/posix_shared");
	//open shared memory area
	int fd_posix = shm_open("/posix_shared", O_RDWR, 0);
	if(fd_posix < -1){
		perror("fd_posix\n");
		exit(EXIT_FAILURE);
	}


	//map shared memory area to pointer
	if((shared_mem_ptr = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd_posix, 0)) == MAP_FAILED){
		perror("mapping\n");
		exit(EXIT_FAILURE);
	}
	

	printf("Parent: I will create 100 Processes now! %i\n", getpid());	

	for(int i = 0; i < 100; i++){

		pid = fork();
		if(pid < 0){
			perror("creating child...\n");
			exit(0);
		}
		if(pid == 0){

		
			int i = 0;
			while(i < 100){

				//Block
				sem_wait(sem);
				//Increase value
				shared_mem_ptr->value++;
				i++;
				//release
				sem_post(sem);
				

			}	

			exit(0);
		}
	}

	//wait for all child processes
	for(int i = 0; i < 100; i++){
		wait(NULL);
	}
	//close semaphore
	sem_close(sem);
		
	printf("The increased Value is: %i\n", shared_mem_ptr->value);
	
	//open named pipe
	fd = open(RESULT_FIFO, O_CREAT | 0777);
	if(fd < 0){
		perror("opening...\n");
		exit(EXIT_FAILURE);

	}

	//send value 
	len = write(fd, shared_mem_ptr, sizeof(struct shared_memory));
	if(len < 0){
		perror("writing...\n");
		exit(EXIT_FAILURE);
	}



	printf("Parent: I'm done: %i\n", getpid());
	close(fd_posix);

	return EXIT_SUCCESS;
}

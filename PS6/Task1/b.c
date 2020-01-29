#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>


#define SHMSZ 27


int main (void){


	pid_t pid;


	key_t key = 5555;
	int shmid;
	int *shm = NULL;
	int *s = NULL;


	int fd, len;
	char *RESULT_FIFO = "/tmp/result_fifo";


	//open shared memory area
	shmid = shmget(key, SHMSZ, 0666);
	if(shmid < 0){
		perror("getting shmid\n");
		exit(0);
	}
	
	//attach to shared memory area
	shm = shmat(shmid, NULL, 0);
	if(shm == (int*) -1){
		perror("attaching...\n");
		exit(0);
	}

	printf("Parent: I will create 100 Processes now! %i\n", getpid());	

	for(int i = 0; i < 100; i++){

		pid = fork();
		if(pid < 0){
			perror("creating child...\n");
			exit(0);
		}
		if(pid == 0){
			
			if(shm == (int*) -1){
				perror("attaching...\n");
				exit(0);
			}

			s = shm;
			int i = 0;
			while(i < 100){
				//increase values
				(*s)++;
				i++;	
			}
			exit(0);
		}
	}

	//wait for all processes
	for(int i = 0; i < 100; i++){
		wait(NULL);
	}

		
	printf("The increased Value is: %i\n", *shm);
	

	//open named pipe
	fd = open(RESULT_FIFO, O_WRONLY);
	if(fd < 0){
		perror("opening...\n");
		exit(EXIT_FAILURE);

	}

	//write values
	len = write(fd, shm, sizeof(shm));
	if(len < 0){
		perror("writing...\n");
		exit(EXIT_FAILURE);
	}



	printf("Parent: I'm done: %i\n", getpid());


	return EXIT_SUCCESS;
}


/*
The output changes every time or almost every time. Example: Two processes get the shared memory(0) at almost the same time, they both increase the value by 1 -> so they will give back the same value(1). 

*/

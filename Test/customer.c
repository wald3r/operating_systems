#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>  
#include <string.h>
#include <semaphore.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/time.h>
#include <time.h>


#define SEM "/mysem"


struct shared_memory { 
		int number;
	};



int main(void){



	pid_t pid;
	
	int fd;
	char * FIFO_BANK = "/tmp/myfifo";
	

	unlink(FIFO_BANK);


	//creating fifo
	if (mkfifo(FIFO_BANK, 0777) < 0) {
		perror("mkfifo error");
		exit(1);
	}


	//open fifo
	if ( (fd = open(FIFO_BANK, O_WRONLY)) < 0) {
			perror("open error for reading fd");
			exit(1);
	}

	//semaphore to protect shared area id
	sem_t *sem;
	sem_unlink(SEM);
	sem = sem_open(SEM, O_CREAT, 0777, 1);

	//shared memory
	struct shared_memory *shared_mem_ptr;
	int fd_posix;


	shm_unlink("/posix_shared");
	fd_posix = shm_open("/posix_shared", O_CREAT | O_RDWR | 0666, 0);
	if(fd_posix < -1){
		perror("fd_posix\n");
		exit(EXIT_FAILURE);
	}


	if(ftruncate(fd_posix, sizeof(struct shared_memory)) == -1){
		perror("truncate\n");
		exit(EXIT_FAILURE);
	}

	if((shared_mem_ptr = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd_posix, 0)) == MAP_FAILED){
		perror("mapping\n");
		exit(EXIT_FAILURE);
	}
	 
	shared_mem_ptr->number = 0;

	

	for(int i = 0; i < 20; i++){
		pid = fork();
		if(pid < 0){
			perror("creating process\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0) {
			//id number of customer 
			int l;
			int unit = 2;
			sem_wait(sem);
			shared_mem_ptr->number++;
			l = shared_mem_ptr->number;
			sem_post(sem);		

			int counter = 0;
			while(counter < 100){
				sem_wait(sem);
				write(fd, &l, sizeof(int));
				write(fd, &unit, sizeof(unit));	
				printf("ID: %i, VALUE: %i\n", l, unit);
				sem_post(sem);
				counter++;
			
			}
			exit(EXIT_SUCCESS);
		}


	}
	for(int i = 0; i <20; i++){
		wait(NULL);
	}
	close(fd);
	sem_close(sem);
	close(fd_posix);

	return EXIT_SUCCESS;
}

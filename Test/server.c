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


#define SEM "/mysem1"

//to store value and id of customer
struct quantity { 
		int q;
		int id;
	};


//store all accounts 
struct shared_memory { 
		struct quantity accounts[21];
	};


int main (void){




	///////////////////


	sem_t *sem;
	sem_unlink(SEM);
	sem = sem_open(SEM, O_CREAT, 0777, 1);


	////////////////////////////

	struct shared_memory *shared_mem_ptr;
	int fd_posix;

	//create new shared memory
	shm_unlink("/posix_shared1");
	fd_posix = shm_open("/posix_shared1", O_CREAT | O_RDWR | 0666, 0);
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
	
	shared_mem_ptr->accounts[0].q = 0;
	shared_mem_ptr->accounts[0].id = 0; 
	

	/////////////////////////////////////////////////////	

	int fd;
	char * FIFO_BANK = "/tmp/myfifo";
	
	//open fifo
	if ( (fd = open(FIFO_BANK, O_RDONLY)) < 0) {
			perror("open error for reading fd");
			exit(1);
	}

	int id;
	int q;
	pid_t pid;	

	int x = 1;


	while(x){
		int len = read(fd, &id, sizeof(int));
		if (len == 0) {
			//exit loop when pipe gets closed
			break;
		}

		len = read(fd, &q, sizeof(int));
		if (len == 0) {
			//exit loop when pipe gets closed
			break;
		}

		pid = fork();
		if(pid < 0){
			perror("creating process\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			//child process to store the value
			int o = id;
			int p = q;
			sem_wait(sem);
			shared_mem_ptr->accounts[o].id = o;
			shared_mem_ptr->accounts[o].q += p;
			sem_post(sem);
			exit(EXIT_SUCCESS);
		}
		
	}

	//wait for all process
	for(int i = 0; i < 2000; i++){
		wait(NULL);
	}

	//print all accounts
	for(int i = 1; i < 21; i++){
		printf("id: %i, q: %i\n", shared_mem_ptr->accounts[i].id, shared_mem_ptr->accounts[i].q);

	}

	sem_close(sem);
	close(fd);
	close(fd_posix);

	return EXIT_SUCCESS;
}

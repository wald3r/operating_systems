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
#include <sys/sem.h>


#define SHMSZ 27

union semun {
  int              val;    
  struct semid_ds *buf;    
  unsigned short  *array; 
  struct seminfo  *__buf;

} arg;

int main (void){


	pid_t pid;
	union semun arg;
	key_t key = 5555;
	key_t sem_key = 4444;
	int shmid;
	int *shm = NULL;


	int fd, len;
	char *RESULT_FIFO = "/tmp/result_fifo";

	//create system v semaphore
	int sem_id = semget(sem_key, 1, IPC_CREAT | 0666);
	if(sem_id < 0){
		perror("semaphore\n");
		exit(EXIT_FAILURE);
	}
	
	//set value 1 
	arg.val = 1;
	if(semctl(sem_id, 0, SETVAL, arg) < 0){
		perror("semctl\n");
		exit(EXIT_FAILURE);
	}

	//struct to operate with the value
	struct sembuf sb = {0, 0, 0};


	//open shared memory area
	shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
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
			//printf("New child: %i\n", getpid());

			int i = 0;
			while(i < 100){

				//decrease semaphore value
				sb.sem_op = -1;
				if(semop(sem_id, &sb, 1) < 0 ) { 
					perror("semop P()  failed"); 
					exit(EXIT_FAILURE); 
				}
				
				//increase values
				(*shm)++;
				i++;


				//increase semaphore value
				sb.sem_op = 1;
				if(semop(sem_id, &sb, 1) < 0 ) { 
					perror("semop V()  failed"); 
					exit(EXIT_FAILURE); 
				}

			}	
			exit(0);
		}
	}

	//wait for all processes
	for(int i = 0; i < 100; i++){
		wait(NULL);
	}

	//delete semaphore
	semctl(sem_id, 0, IPC_RMID, arg);
	printf("The increased Value is: %i\n", *shm);
	
	//open named pipe
	fd = open(RESULT_FIFO, O_WRONLY);
	if(fd < 0){
		perror("opening...\n");
		exit(EXIT_FAILURE);

	}

	//send value
	len = write(fd, shm, sizeof(shm));
	if(len < 0){
		perror("writing...\n");
		exit(EXIT_FAILURE);
	}



	printf("Parent: I'm done: %i\n", getpid());


	return EXIT_SUCCESS;
}

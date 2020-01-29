#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>



#define SHMSZ 27

int main(void){

	int value = 0;
	key_t key = 5555;
	int shmid;
	int *shm, *s;	


	int fd, len, buffer;
	char *RESULT_FIFO = "/tmp/result_fifo";
	


	//Create Shared memory area 
	shmid = shmget(key, SHMSZ, IPC_CREAT | 0666 );
	if(shmid < 0){
		perror("creating...\n");
		exit(EXIT_FAILURE);
	}

	//attach area
	shm = shmat(shmid, NULL , 0);
	if(shm == (int*) -1){
		perror("attaching...\n");
		exit(EXIT_FAILURE);
	}
	
	//point to value
	s = shm;
	*s = value;
	
	//Create named pipe
	printf("Stored value: %i\n", *s);
	unlink(RESULT_FIFO);
	if(mkfifo(RESULT_FIFO, 0777) < 0){
		perror("mkfifo error\n");
		exit(EXIT_FAILURE);
	}

	//open named pipe
	fd = open(RESULT_FIFO, O_RDONLY);
	if(fd < 0){
		perror("opening...\n");
		exit(EXIT_FAILURE);
	}

	//wait for income
	len = read(fd, &buffer, sizeof(buffer));
	if(len == 0){
		perror("reading...\n");
		exit(EXIT_FAILURE);
	}
	printf("New Value: %i\n", buffer);

	//delete shared memory area
	shmctl(shmid, IPC_RMID, 0);

	return EXIT_SUCCESS;

}

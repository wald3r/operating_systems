#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>


//struct for memory area
struct shared_memory { 
	int value;
};


int main(void){


	struct shared_memory *shared_mem_ptr;
	int fd_posix;

	int fd, len, buffer;
	char *RESULT_FIFO = "/tmp/result_fifo";
	
	//unlink memory area when it already is linked
	shm_unlink("/posix_shared");

	//create memory area
	fd_posix = shm_open("/posix_shared", O_CREAT | O_RDWR | 0666, 0);
	if(fd_posix < -1){
		perror("fd_posix\n");
		exit(EXIT_FAILURE);
	}


	//set size
	if(ftruncate(fd_posix, sizeof(struct shared_memory)) == -1){
		perror("truncate\n");
		exit(EXIT_FAILURE);
	}
	
	//map pointer to shared memory area;
	if((shared_mem_ptr = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd_posix, 0)) == MAP_FAILED){
		perror("mapping\n");
		exit(EXIT_FAILURE);
	}
	
	//initialise shared memory area with zero
	shared_mem_ptr->value = 0;

	
	printf("Stored value: %i\n", shared_mem_ptr->value);


	//unlink when it is already linked
	unlink(RESULT_FIFO);
	//create new named piped
	if(mkfifo(RESULT_FIFO, 0777) < 0){
		perror("mkfifo error\n");
		exit(EXIT_FAILURE);
	}

	//open pipe
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

	close(fd_posix);


	return EXIT_SUCCESS;

}

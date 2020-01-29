#include	<sys/types.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include 	<stdbool.h>
#include 	<string.h>


int
main(void)
{
	int	fdwrite;
	char * FIFO3 = "/tmp/myfifo3";
	char buffer[] = "[middleware]";

	if (mkfifo(FIFO3, 0777) < 0) {
		perror("mkfifo error");
		exit(1);
	}

	if ((fdwrite = open(FIFO3, O_WRONLY)) < 0){
		perror("open error for writing");
		exit(1);
	}
	srand(time(NULL));
	for(;;){	
		write(fdwrite, buffer, strlen(buffer));
		int r = rand() % 5;  
		sleep(r);
	}	
	close(fdwrite);


        unlink(FIFO3);


	exit(0);
}





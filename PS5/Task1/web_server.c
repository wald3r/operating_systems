#include	<sys/types.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<time.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include 	<stdbool.h>
#include 	<string.h>


int
main(void)
{
	int	fdwrite;
	char * FIFO1 = "/tmp/myfifo1";
	char buffer[] = "[web]";

	if (mkfifo(FIFO1, 0777) < 0) {
		perror("mkfifo error");
		exit(1);
	}

	if ((fdwrite = open(FIFO1, O_WRONLY)) < 0){
		perror("open error for writing");
		exit(1);
	}

	srand(time(NULL));
	for(;;){	
		write(fdwrite, buffer, strlen(buffer));
		int r = rand() % 8;  
		sleep(r);
	}	
	close(fdwrite);


        unlink(FIFO1);

	exit(0);
}








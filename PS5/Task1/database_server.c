#include	<sys/types.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<time.h>
#include	<stdio.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include 	<string.h>


int
main(void)
{
	int	fdwrite;
	char * FIFO2 = "/tmp/myfifo2";
	char buffer[] = "[database]";

	if (mkfifo(FIFO2, 0777) < 0) {
		perror("mkfifo error");
		exit(1);
	}

	if ((fdwrite = open(FIFO2, O_WRONLY)) < 0){
		perror("open error for writing");
		exit(1);
	}
	srand(time(NULL));
	for(;;){	
		write(fdwrite, buffer, strlen(buffer));
		int r = rand() % 9;  
		sleep(r);
	}	
	close(fdwrite);


        unlink(FIFO2);

	exit(0);
}


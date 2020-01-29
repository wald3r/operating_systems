#include	<sys/types.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include 	<sys/select.h>

int
main(void)
{

	int	fd1, fd2, fd3;
	int 	number1 = 0;
	int 	number2 = 0;
	int 	number3 = 0;
	int 	maxfd;
	char buf[1024];
	char * FIFO1 = "/tmp/myfifo1";
	char * FIFO2 = "/tmp/myfifo2";
	char * FIFO3 = "/tmp/myfifo3";

	if ( (fd1 = open(FIFO1, O_RDONLY | O_NONBLOCK)) < 0) {
		perror("open error for reading fd1");
		exit(1);
	}

	if ( (fd2 = open(FIFO2, O_RDONLY | O_NONBLOCK)) < 0) {
		perror("open error for reading fd2");
		exit(1);
	}
	if ( (fd3 = open(FIFO3, O_RDONLY | O_NONBLOCK)) < 0) {
		perror("open error for reading fd3");
		exit(1);
	}


	for(;;){
		fd_set fds;
		int maxdf;
		FD_ZERO(&fds);
		FD_SET(fd1, &fds);
		FD_SET(fd2, &fds);
		FD_SET(fd3, &fds);
	
		maxfd = fd1 > fd2 ? fd1 : fd2;
		maxfd = fd3 > maxfd ? fd3 : maxfd;
		select(maxfd + 1, &fds, NULL, NULL, NULL);
		
		if(FD_ISSET(fd1, &fds)){
			int len = read(fd1, buf, 1024);
			if (len == 0) {
				exit(0);
			} else {
    				 buf[len] = 0;
	       			 printf("%s message %i from web server!\n", buf, number1);
				 number1++;
			}
		}
		if(FD_ISSET(fd2, &fds)){
			int len = read(fd2, buf, 1024);
			if (len == 0) {
				exit(0);
			} else {
    				 buf[len] = 0;
	       			 printf("%s message %i from database\n", buf, number2);
				 number2++;
			}
		}

		if(FD_ISSET(fd3, &fds)){
			int len = read(fd3, buf, 1024);
			if (len == 0) {
				exit(0);
			} else {
    				 buf[len] = 0;
	       			 printf("%s message %i from middle-ware server\n", buf, number3);
				 number3++;
			}
		}

	}


	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}







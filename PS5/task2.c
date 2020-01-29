#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main (int argc, char *argv[]){

	char* input = argv[1];

	int	fd[2];
	pid_t	pid;

	if (pipe(fd) < 0) {
		perror("pipe error");
		exit(1);
	}

	if ( (pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	}


	if (pid > 0) {
				/* parent */
		dup2(fd[0], 0);		
		close(fd[0]);
		close(fd[1]);
		execlp("grep", "grep", input, NULL);
		


	} else {	
				/* child */

		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		execlp("ls", "ls", "-al", NULL);
		exit(0);
	}

	exit(0);



}



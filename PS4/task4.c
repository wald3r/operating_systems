#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <signal.h>


struct sigaction child_action;

void child_handler(int signalNumber){
	

	if(signalNumber == SIGCHLD){
		fprintf(stdout, "Child sends SIGCHLD. Signalnumber: %d\n", signalNumber);
	}
}





int main (void){

	pid_t pid;

	child_action.sa_handler = child_handler;
	sigemptyset(&child_action.sa_mask);
	sigaction(SIGCHLD, &child_action, NULL);


	printf("Parent Process! My pid is %d. \n", getpid());
	pid = fork();

	if(pid < 0){
		return -1;
	}
	else if(pid == 0){
		printf("Child Process! My pid is %d. \n", getpid());
		sleep(5);
		return 1;
	}
	


	pause();
	printf("Parent received Signal.\n");
	printf("Parent Process! I'm done. %d \n", getpid());		

	




	return EXIT_SUCCESS;
}



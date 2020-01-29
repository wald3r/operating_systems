#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <signal.h>


sigset_t sigset;
struct sigaction parent_action;
int flag = 0;
int counter = 0;
pid_t pid;

void parent_handler(int signalNumber){
	
	if(signalNumber == SIGALRM){
		for(int i = 0; i < 4; i++){
			if(i < 3){
				sleep(5);
				kill(pid,SIGUSR1);
				//fprintf(stdout, "try sending SIGUSR2 Signal\n");			
				//kill(pid,SIGUSR2);
		     	}
		     	else{
				sleep(5);
				kill(pid,SIGUSR2);
		         }
		}
		

	}

	
	if(signalNumber == SIGUSR2){
		fprintf(stdout, "Parent sends SIGUSR2. Signalnumer: %d\n", signalNumber);
		flag = 1;
	}

	if(signalNumber == SIGUSR1){
		fprintf(stdout, "Parent sends SIGUSR1. Signalnumber: %d\n", signalNumber);
	}
}

int main (void){


	parent_action.sa_handler = parent_handler;
	sigemptyset(&parent_action.sa_mask);
	sigemptyset(&sigset);

	sigaddset(&sigset, SIGUSR2);
	sigaction(SIGUSR1, &parent_action, NULL);
	sigaction(SIGUSR2, &parent_action, NULL);
	sigaction(SIGALRM, &parent_action, NULL);
	sigprocmask(SIG_BLOCK, &sigset, NULL);	

	fprintf(stdout, "Parent Process! My pid is %d. \n\n", getpid());
	pid = fork();

	if(pid < 0){
		return -1;
	}
	else if(pid == 0){
		while(!flag){	
			if(counter == 3){
				sigprocmask(SIG_UNBLOCK, &sigset, NULL);
			}
			pause();
			fprintf(stdout, "Child Process received Signal.\n\n");
			counter++;
		}
		printf("Child Process! I'm done working. %d\n", getpid());
		return 0;
	}
	else{
		
		alarm(1);
		
	}
        wait(NULL);
	sleep(3);
	printf("Parent Process. I'm done working. %d\n", getpid());	




	return EXIT_SUCCESS;
}



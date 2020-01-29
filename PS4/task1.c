#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 


int main (void){

	pid_t pid;
	int status = 0;
	int i;

	printf("Parent Process! My pid is %d. \n", getpid());
		

	for(i = 0; i < 9; i++){
		pid = fork();
		if(pid < 0){		
			printf("Fork error\n");
			return 0;
		}
		else if(pid == 0){
			
			printf("Child Process %d! My pid is %d. \n", i, getpid());
			return 1;
		}	
	}

	while((pid = wait(&status)) > 0);
	
	printf("Parent Process again! My pid is %d. \n", getpid());
	




	return EXIT_SUCCESS;
}



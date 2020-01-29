#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 


//The order is not consistent. The Order is not predictable. A child process runs fully independently of the parent. 
int main (void){

	pid_t pid;
	int i;

	printf("Parent Process! My pid is %d. \n", getpid());
		

	for(i = 0; i < 16; i++){
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
	
	printf("16 Child Processes has been created. %d. \n", getpid());
	




	return EXIT_SUCCESS;
}



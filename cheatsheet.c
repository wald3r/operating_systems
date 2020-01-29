Threads:
#define _GNU_SOURCE        
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>


-lpthread


//Threads erzeugen
int check;
pthread_t thread_id[10];
check = pthread_create(&thread_id[i], NULL, &thread_function, arg);
		if(check == -1){
			perror("creating thread\n");
		}


//Thread Funktion
void *thread_function(void *arg){

        	//void poiner casten
		int id = (int*)arg;

		return NULL;
	}	

//Thread abbrechen
check = pthread_cancel(thread_id[cancel]);
		if(check != 0){
			fprintf(stderr, "cancel failed: %i\n", cancel);
		}


//Auf Threads warten
for(int i = 0; i < Anzahl_von_Threads; i++){
		check = pthread_join(thread_id[i], NULL);
		if(check != 0){
			perror("pthread join\n");
		}	
	}


//thread id <- in der thread funktion aufrufen
long tid = syscall(SYS_gettid);


__________________________________________________________________________________________

Thread cleanup 

#define _GNU_SOURCE        
#include <pthread.h>

-lpthread

//1tes Argument: Name der Cleanup funktion. 2tes Argument: argument für die funktion
pthread_cleanup_push(cleanup_function, arg);



//Die Cleanup funktion wird nur aufgerufen wenn ein cancel erfolgt. Ansonsten 0 gegen 1 austauschen. 
pthread_cleanup_pop(0);


//cleanup_function
void cleanup_function(void *arg){
	
	
}
_____________________________________________________________________________________

Mutex:

#include <pthread.h>

-lpthread

//dynamisch, wenn vorher speicher alloziert werden muss
pthread_mutex_t mutex;
pthread_mutex_init (&mutex, NULL);

//statisches initialieren
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


//Bereich sperren
pthread_mutex_lock(&lock);

//Bereich entsperren
pthread_mutex_unlock(&lock);

//Mutex zerstören - dynamische
pthread_mutex_destroy(&lock);

_____________________________________________________________________________________

Spinlocks:


#include <pthread.h>

-lpthread


//initialisieren
int check;
pthread_spinlock_t lock;
check = pthread_spin_init(&lock, 0);
	if(check != 0){
		perror("init spinlock\n");
	}


//Bereich sperren
pthread_spin_lock(&lock);

//Bereich freigeben
pthread_spin_unlock(&lock);


_____________________________________________________________________________________

Condition Variables: 

#include <pthread.h>

-lpthread

//dynamisch initialisieren
pthread_cond_t empty;
pthread_cond_init(&empty, NULL);

//Statisches initialisieren
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;


//Thread in Ruhemodus versetzen(im gesperrten Bereich). Wenn zb die queue leer ist. 1tes Argument Condition variable, 2tes Argument Mutex lock
//Immer eine while Schleife statt If-Statement verwenden. 
while(myqueue.size() == 0){
	pthread_cond_wait(&empty, &lock);	
}

//Thread aufwecken(im gesperrten Bereich)
pthread_cond_signal (&empty);	

//variable löschen - dynamisch
pthread_cond_destroy(&empty);

_____________________________________________________________________________________


Prozesse:

#include <unistd.h>
#include <sys/wait.h> 
#include <sys/types.h>

pid_t pid - um die Prozess ID zu speichern
fork() erstellt neuen Prozess. Liefert einen negativen Wert bei einem Fehler retour, 0 wenn es der Child Prozess ist und > 0 wenn es der Parent ist.

pid = fork()

Wenn Parent warten soll: wait(NULL)

//////////////Child Prozesse immer mit exit beenden./////////////////
______________________________________________________________________________________

Signale:

#define _XOPEN_SOURCE
#include <signal.h>
#include <unistd.h>



//Handler erstellen und Signale hinzufügen
	struct sigaction signal_action;

	signal_action.sa_handler = signal_handler;
	sigemptyset(&signal_action.sa_mask);

	sigaction(SIGUSR1, &signal_action, NULL);
	sigaction(SIGUSR2, &signal_action, NULL);
	sigaction(SIGALRM, &signal_action, NULL);


//Zum blockieren und deblockieren von Signalen
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR2);
	sigprocmask(SIG_BLOCK, &sigset, NULL);	
	sigprocmask(SIG_UNBLOCK, &sigset, NULL);


//Handler zum verarbeiten von Signalen
void signal_handler(int signalNumber){
	
}

//Auf Signal warten
	pause();

//Versenden vom Alarmsignal
	alarm(5);
	pause();

//Versenden von User Signalen
kill(pid,SIGUSR1);
kill(pid,SIGUSR2);

Sonderfall: SIGCHLD 
Wird automatisch versendet wenn ein Child Prozess beendet wird. 

____________________________________________________________________________________________

Unnamed pipe:

#include <unistd.h>


//Pipe erstellen
	int fd[2];
	if (pipe(fd) < 0) {
		perror("pipe error");
		exit(1);
	}


//Schreiben
	close(fd[0]);
	write(fd[1], "hello world\n", 12);

//Lesen
	close(fd[1]);
	int n = read(fd[0], buffer, 1024);

______________________________________________________________________________________________

Named Pipe FIFO

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//FIFO erstellen
	int fd;
	char * FIFO = "/tmp/myfifo";
	unlink(FIFO);
	if (mkfifo(FIFO, 0777) < 0) {
		perror("mkfifo error");
		exit(1);
	}


//FIFO öffnen - NONBLOCK damit nicht auf eine Nachricht gewartet wird. 
	if ( (fd = open(FIFO, O_RDONLY | O_WRONLY | O_NONBLOCK)) < 0) {
			perror("open error for reading fd");
			exit(1);
	}

//Schreiben
	write(fd, buffer, strlen(buffer));

//Lesen
	int len = read(fd, buffer, 1024);
	if (len == 0) {
		exit(0);
	}

//Dekriptor schließen
	close(fd);

_________________________________________________________________________________________________

SELECT FIFO


#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/time.h>


//Select initialisieren und anwenden
	for(;;){
		fd_set fds; //erzeuge Menge
		int maxfd;
		FD_ZERO(&fds); // Menge auf 0 setzen
		FD_SET(fd1, &fds); //füge fd1 dazu
		FD_SET(fd2, &fds); //füge fd2 dazu
		FD_SET(fd3, &fds); //füge fd3 dazu
	
	//Finde größten fd
		maxfd = fd1 > fd2 ? fd1 : fd2;
		maxfd = fd3 > maxfd ? fd3 : maxfd;
	//select
		select(maxfd + 1, &fds, NULL, NULL, NULL);
		
	//Überprüfe alle Kanäle
		if(FD_ISSET(fd1, &fds)){
			int len = read(fd1, buf, 1024);
			if (len == 0) {
				exit(0);
			}
		}
		if(FD_ISSET(fd2, &fds)){
			int len = read(fd2, buf, 1024);
			if (len == 0) {
				exit(0);
			}
		}

		if(FD_ISSET(fd3, &fds)){
			int len = read(fd3, buf, 1024);
			if (len == 0) {
				exit(0);
			} 
		}

	}

_______________________________________________________________________________________________________

POSIX Semaphore:


#include <fcntl.h>                
#include <sys/stat.h>               
#include <semaphore.h>

compile -pthread

//Erzeuge Semaphor mit Wert 1
#define SEM "/mysem"
	sem_t *sem;
	sem_unlink(SEM);
	sem = sem_open(SEM, O_CREAT, 0777, 1);

//Bereich sperren
sem_wait(sem);
//Bereich entsperren
sem_post(sem);

//Semaphore schließen
sem_close(sem);

_________________________________________________________________________________________________________

POSIX SharedMemory:

#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>  

compile -lrt

//SharedMemory Struct anlegen. Hier mit Beispiel "int value"
	struct shared_memory { 
		int value;
	};

	struct shared_memory *shared_mem_ptr;
	int fd_posix;

//////////SHM anlegen und initialisieren///////////////////////
//Shared Memory erzeugen
	shm_unlink("/posix_shared");
	fd_posix = shm_open("/posix_shared", O_CREAT | O_RDWR | 0666, 0);
	if(fd_posix < -1){
		perror("fd_posix\n");
		exit(EXIT_FAILURE);
	}


//Größe festlegen
	if(ftruncate(fd_posix, sizeof(struct shared_memory)) == -1){
		perror("truncate\n");
		exit(EXIT_FAILURE);
	}
	
//Mit mmap auf den pointer routen
	if((shared_mem_ptr = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd_posix, 0)) == MAP_FAILED){
		perror("mapping\n");
		exit(EXIT_FAILURE);
	}
	
//den Startwert initialisieren
	shared_mem_ptr->value = 0;


/////SHM öffnen und bearbeiten///////////////
//Besetehende Area öffnen
	int fd_posix = shm_open("/posix_shared", O_RDWR, 0);
	if(fd_posix < -1){
		perror("fd_posix\n");
		exit(EXIT_FAILURE);
	}


//Mit mmap auf den pointer routen
	if((shared_mem_ptr = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd_posix, 0)) == MAP_FAILED){
		perror("mapping\n");
		exit(EXIT_FAILURE);
	}

////////////////////////////

//Deskriptor schließen am Ende.
close(fd_posix);




_________________________________________________________________

Includes:
#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>  
#include <string.h>
#include <semaphore.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/time.h>
	

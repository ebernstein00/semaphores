#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

union semun {
            int val;
            struct semid_ds *buf;
            unsigned short  *array;
} semu;

char cmdflag[10];
int fd; sema; sharmem;

int main(int argc, char *argv[]){
	semu.val = 1;
	strcpy(flag, argv[1]);
	start();
	return 0;
}

void start(){
	if (strcmp(cmdflag, "-c") == 0) create();
   	else if (strcmp(flag, "-v") == 0) view();
	else if (strcmp(flag, "-r") == 0) remove();
	else printf("Flag not recognized\nValid tags include:\n-c\n-v\n-r\n");
}

int create(){
	//Create semaphore...
	sema = semget(SEMKEY, 1, IPC_CREAT | 0644);
	if (sema < 0){
    	printf("Error: %s\n", strerror(errno));
    	return -1;
    }
    semctl(sema, 0, SETVAL, semu);

    //Create shared memory...
    sharmem = shmget(SHKEY, sizeof(char*), IPC_CREAT | 0644);
    if (sharmem < 0){
    	printf("Error: %s\n", strerror(errno));
    	return -1;
    }

    //Create file...
    fd = open("semaphone.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0){
    	printf("Error: %s\n", strerror(errno));
    	return -1;
    }
    close(fd);

    return 0;
}

int remove(){
	//Remove semaphore...
	semctl(sema, IPC_RMID, 0);

	//Remove shared memory...
	shmctl(sharmem, IPC_RMID, 0);

	//Remove file...
	remove("semaphone.txt");

	return 0;
}

int view(){
	//Open file for reading only...
	FILE *fptr;
	char ch;
	scanf("semaphone.txt");
	fptr = fopen("semaphone.txt", "r");
	if (fptr == NULL){
		printf("File can't be opened\n");
		return -1;
	}

	//Print file character by character...
	ch = fgetc(fptr);
	while (ch != EOF){
		printf("%c", ch);
		ch = fgetc(fptr);
	}
	fclose(fptr);

	return 0;
}
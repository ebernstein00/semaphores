#include "semaphore.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
} semu;

char cmdflag[10];
int fd; sema; sharmem;
struct sembuf sb;

int main(int argc, char *argv[]){
	semu.val = 1;
	sb.sem_num = 0;
	sb.sem_op = -1;
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
	sema = semget(SEMAKEY, 1, IPC_CREAT | 0644);
	if (sema < 0){
    	printf("Error: %s\n", strerror(errno));
    	return -1;
    }
    semctl(sema, 0, SETVAL, semu);
    printf("Semaphore created successfully...\n");

    //Create shared memory...
    sharmem = shmget(SHARKEY, sizeof(char*), IPC_CREAT | 0644);
    if (sharmem < 0){
    	printf("Error: %s\n", strerror(errno));
    	return -1;
    }
    printf("Shared memory created successfully...\n")

    //Create file...
    fd = open("semaphone.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0){
    	printf("Error: %s\n", strerror(errno));
    	return -1;
    }
    close(fd);
    printf("File created successfully...\n");

    return 0;
}

int remove(){
	//Checking semaphore...
	sema = semget(SEMAKEY, 1, 0);
	if (sema < -1){
		printf("Error: %s\n", strerror(errno));
		return -1;
	}
	printf("Trying to get in...\n");
	semop(sema, &sb, 1);

	//Access shared memory...
	sharmem = shmget(SHARKEY, sizeof(char*), 0);
	if (sharmem < 0){
		printf("Error: %s\n", strerror(errno));
		return -1;
	}

	//Displaying story...
	view();
	/*
	FILE *fptr;
	char ch;
	scanf("semaphone.txt");
	fptr = fopen("semaphone.txt", "r");
	if (fptr == NULL){
		printf("File can't be opened\n");
		return -1;
	}
	ch = fgetc(fptr);
	while (ch != EOF){
		printf("%c", ch);
		ch = fgetc(fptr);
	}
	fclose(fptr);
	*/

	//Remove shared memory...
	shmctl(sharmem, IPC_RMID, 0);
	printf("Shared memory removed successfully...\n");

	//Remove file...
	remove("semaphone.txt");
	printf("File removed successfully...\n");

	//Remove semaphore...
	semctl(sema, IPC_RMID, 0);
	printf("Semaphore removed successfully...\n");

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
	printf("The story so far...\n")

	//Print file character by character...
	ch = fgetc(fptr);
	while (ch != EOF){
		printf("%c", ch);
		ch = fgetc(fptr);
	}
	fclose(fptr);

	return 0;
}
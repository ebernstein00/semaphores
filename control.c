#include "semaphore.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
} semu;

char cmdflag[10];
int fd; 
int semd; 
int shmd;
struct sembuf sb;

int main(int argc, char *argv[]){
	semu.val = 1;
	sb.sem_num = 0;
	sb.sem_op = -1;
	strcpy(cmdflag, argv[1]);
	start();
	return 0;
}

int start(){
	if (strcmp(cmdflag, "-c") == 0) create();
   	else if (strcmp(cmdflag, "-v") == 0) view();
	else if (strcmp(cmdflag, "-r") == 0) removes();
	else printf("Flag not recognized\nValid tags include:\n-c\n-v\n-r\n");
	return 0;
}

int create(){
	//Create semaphore...
	semd = semget(SEMKEY, 1, IPC_CREAT | 0644);
	if (semd < 0){
    	printf("Error: %s\n", strerror(errno));
    	return -1;
    }
    semctl(semd, 0, SETVAL, semu);
    printf("Semaphore created successfully...\n");

    //Create shared memory...
    shmd = shmget(SHKEY, sizeof(char*), IPC_CREAT | 0644);
    if (shmd < 0){
    	printf("Error: %s\n", strerror(errno));
    	return -1;
    }
    printf("Shared memory created successfully...\n");

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

int removes(){
	//Checking semaphore...
	semd = semget(SEMKEY, 1, 0);
	if (semd < 0){
		printf("Error: %s\n", strerror(errno));
		return -1;
	}
	printf("Trying to get in...\n");
	semop(semd, &sb, 1);

	//Access shared memory...
	shmd = shmget(SHKEY, sizeof(char*), 0);
	if (shmd < 0){
		printf("Error: %s\n", strerror(errno));
		return -1;
	}

	//Displaying story...
	view();

	//Remove shared memory...
	shmctl(shmd, IPC_RMID, 0);
	printf("Shared memory removed successfully...\n");

	//Remove file...
	remove("semaphone.txt");
	printf("File removed successfully...\n");

	//Remove semaphore...
	semctl(semd, IPC_RMID, 0);
	printf("Semaphore removed successfully...\n");

	return 0;
}

int view(){
	//Open file for reading only...

	FILE *fptr;
	char ch;
	fptr = fopen("semaphone.txt", "r");
	if (fptr == NULL){
		printf("File can't be opened\n");
		return -1;
	}
	printf("The story so far...\n");

	//Print file character by character...
	ch = fgetc(fptr);
	while (ch != EOF){
		printf("%c", ch);
		ch = fgetc(fptr);
	}
	printf("\n");
	fclose(fptr);
	sb.sem_num = 0;
	//semop(semd, &sb, 1);

	return 0;
}
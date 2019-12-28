#include "semaphore.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
} semu;

int semd; 
int shmd; 
int fd;
struct sembuf sb;

int main(){
	sb.sem_num = 0;
	sb.sem_op = -1;
	start();
	return 0;
}

int start(){
	//Checking semaphore...
	int semd = semget(SEMKEY, 1, 0);
	if (semd < 0){
		printf("Error: %s\n", strerror(errno));
		return -1;
	}
	printf("Trying to get in...\n");
	semop(semd, &sb, 1);

	//Opening shared memory...
	shmd = shmget(SHKEY, sizeof(char*), 0);
	if (shmd < 0){
		printf("Error: %s\n", strerror(errno));
		return -1;
	}

	//Opening file for appending...
	fd = open("semaphone.txt", O_WRONLY | O_APPEND);
	char *line = shmat(shmd, 0, 0);

	//Display last addition and take in new addition...
	printf("Last addition...\n%s\n\n", line);
	printf("Your addition...\n");
	char *add;
	fgets(add, 1024, stdin);
	printf("Adding \"%s\" to the story...\n", add);

	//Write addition to file...
	write(fd, add, strlen(add));
	close(fd);

	//Setting last addition...
	strcpy(line, add);
	shmdt(line);
	sb.sem_op = 1;
	semop(semd, &sb, 1);

	return 0;
}
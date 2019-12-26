#include "semaphore.h"

int sema; sharmem; fd;
struct sembuf sb;

int main(){
	start();
	return 0;
}

int start(){
	//Checking semaphore...
	int sema = semget(SEMAKEY, 1, 0);
	if (sema < 0){
		printf("Error: %s\n", strerror(errno));
		return -1;
	}
	printf("Trying to get in...\n");
	semop(sema, &sb, 1);

	//Opening shared memory...
	sharmem = shmget(SHARKEY, sizeof(char*), 0);
	if (sharmem < 0){
		printf("Error: %s\n", strerror(errno));
		return -1;
	}

	//Opening file for appending...
	fd = open("semaphone.txt", O_WRONLY | O_APPEND);
	char *line = shmat(sharmem, 0, 0);

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
	semop(sema, *sb, 1);

	return 0;
}
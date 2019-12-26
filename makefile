all: control.o write.o
	gcc -o control control.o
	gcc -o write write.o

control.o: control.c semaphore.h
	gcc -c control.c

write.o: write.c semaphore.h
	gcc -c write.c

clean:
	rm *.exe
	rm *.o

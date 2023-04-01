all:
	gcc -Wall -c forktester.c
	gcc -Wall -o forktester forktester.o
	gcc -Wall -c forktester.c -D EXEC
	gcc -Wall -o exectester forktester.o -D EXEC

clean:
	rm -f *.o forktester exectester

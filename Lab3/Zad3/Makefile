all:
	gcc -Wall -c recursiveread.c
	gcc -Wall -o recursiveread recursiveread.o
	
run_test:
	./recursiveread `pwd` 'test'

clean:
	rm -f *.o recursiveread

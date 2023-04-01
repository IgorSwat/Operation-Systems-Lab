all: compile
	ar rcs libblockutil.a blockutil.o
	gcc -shared -fPIC -o libblockutil.so blockutil.o

compile:
	gcc -c -Wall blockutil.c
	gcc -c -Wall -fPIC blockutil.c

clean:
	rm -f *.a *.o *.so

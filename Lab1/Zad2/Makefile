LIB = libblockutil
LIBC = blockutil.c
LIBO = blockutil.o

all: 
	make create_static_lib
	make create_shared_lib
	make static_compile
	make shared_compile
	make dll_compile

static_compile:
	gcc -c -Wall main.c
	gcc -Wall -g -o main_static main.o -L. $(LIB).a
	rm -f main.o

create_static_lib:
	gcc -c -Wall $(LIBC)
	ar rcs $(LIB).a $(LIBO)

shared_compile:
	gcc -c -Wall main.c
	gcc -Wall -o main_shared main.o -L. $(LIB).so -Wl,-rpath=`pwd`
	rm -f main.o

create_shared_lib:
	gcc -c -Wall -fPIC $(LIBC)
	gcc -shared -fPIC -o $(LIB).so $(LIBO)
	
dll_compile:
	gcc -c -Wall main.c -D DLL_FLAG
	gcc -Wall  $(OPT) -o main_dll main.o -ldl -Wl,-rpath=`pwd` -D DLL_FLAG

clean:
	rm -f *.a *.o *.so main_static main_shared main_dll

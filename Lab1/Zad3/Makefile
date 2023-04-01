LIB = libblockutil
LIBC = blockutil.c
LIBO = blockutil.o

all: 
	make create_static_lib
	make create_shared_lib
	make static_compile
	make shared_compile
	make dll_compile
	
opt_O:
	make all OPT=-O

opt_O0:
	make all OPT=-O0

opt_O1:
	make all OPT=-O1

opt_O2:
	make all OPT=-O2

opt_O3:
	make all OPT=-O3

opt_Os:
	make all OPT=-Os

test:
	make opt_O
	make run_test SUFFIX=O
	make opt_O0
	make run_test SUFFIX=O0
	make opt_O1
	make run_test SUFFIX=O1
	make opt_O2
	make run_test SUFFIX=O2
	make opt_O3
	make run_test SUFFIX=O3
	make opt_Os
	make run_test SUFFIX=Os
	rm -f test.in

run_test:
	make create_test
	./main_static < test.in > results/results_static_$(SUFFIX).txt
	./main_shared < test.in > results/results_shared_$(SUFFIX).txt
	./main_dll < test.in > results/results_dll_$(SUFFIX).txt

create_test:
	echo "init 3" > test.in
	echo "count `pwd`/blockutil.c" >> test.in
	echo "count `pwd`/blockutil.h" >> test.in
	echo "show 0" >> test.in
	echo "show 1" >> test.in
	echo "delete 0" >> test.in
	echo "delete 1" >> test.in
	echo "destroy" >> test.in
	echo "exit" >> test.in
	
static_compile:
	gcc -c -Wall $(OPT) main.c
	gcc -Wall -g $(OPT) -o main_static main.o -L. $(LIB).a

create_static_lib:
	gcc -c -Wall $(OPT) $(LIBC)
	ar rcs $(LIB).a $(LIBO)

shared_compile:
	gcc -c -Wall $(OPT) main.c
	gcc -Wall $(OPT) -o main_shared main.o -L. $(LIB).so -Wl,-rpath=`pwd`

create_shared_lib:
	gcc -c -Wall $(OPT) -fPIC $(LIBC)
	gcc -shared -fPIC $(OPT) -o $(LIB).so $(LIBO)
	
dll_compile:
	gcc -c -Wall $(OPT) main.c -D DLL_FLAG
	gcc -Wall  $(OPT) -o main_dll main.o -ldl -Wl,-rpath=`pwd` -D DLL_FLAG

clean:
	rm -f *.a *.o *.so main_static main_shared main_dll

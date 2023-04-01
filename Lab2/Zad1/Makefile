
all:
	make compile_sys
	make compile_lib

test:
	make compile_sys
	make compile_lib
	make run_tests

run_tests:
	echo "Wersja systemowa:" > pomiar_zad1.txt
	./filechange_sys t x `pwd`/test1 `pwd`/test1_result_sys >> pomiar_zad1.txt
	echo "Wersja biblioteczna:" >> pomiar_zad1.txt
	./filechange_sys t x `pwd`/test1 `pwd`/test1_result_lib >> pomiar_zad1.txt

compile_sys:
	gcc -Wall -c filechange.c -D SYS_FLAG
	gcc -Wall -o filechange_sys filechange.o -D SYS_FLAG

compile_lib:
	gcc -Wall -c filechange.c
	gcc -Wall -o filechange_lib filechange.o

clean:
	rm -f *.o filechange filechange_sys filechange_lib

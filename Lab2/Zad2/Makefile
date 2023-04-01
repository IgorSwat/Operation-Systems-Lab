NAME=reverse

all:
	make compile_1
	make compile_1024

test:
	make compile_1
	make compile_1024
	make run_tests

run_tests:
	echo "Test 1 (< 1024 file size):" > pomiar_zad2.txt
	echo "- 1 byte reading:" >> pomiar_zad2.txt
	./$(NAME)_1 `pwd`/test1 `pwd`/test1_result >> pomiar_zad2.txt
	echo "- 1024 bytes reading:" >> pomiar_zad2.txt
	./$(NAME)_1024 `pwd`/test1 `pwd`/test1_result >> pomiar_zad2.txt
	echo "" >> pomiar_zad2.txt
	echo "Test 2 (> 1024 file size):" >> pomiar_zad2.txt
	echo "- 1 byte reading:" >> pomiar_zad2.txt
	./$(NAME)_1 `pwd`/test2 `pwd`/test2_result >> pomiar_zad2.txt
	echo "- 1024 bytes reading:" >> pomiar_zad2.txt
	./$(NAME)_1024 `pwd`/test2 `pwd`/test2_result >> pomiar_zad2.txt
	
compile_1:
	gcc -Wall -c $(NAME).c -D SINGLE_BYTE
	gcc -Wall -o $(NAME)_1 $(NAME).o -D SINGLE_BYTE

compile_1024:
	gcc -Wall -c $(NAME).c
	gcc -Wall -o $(NAME)_1024 $(NAME).o

clean:
	rm -f *.o $(NAME)_1 $(NAME)_1024

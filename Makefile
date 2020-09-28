CC=gcc
CFLAGS=-Wall -Werror -Wextra -lX11

run: poly.out
	./$<

mem_test: poly.out
	valgrind --track-origins=yes --leak-check=full -s ./$<

poly.out: main.o sven.o
	$(CC) $(CFLAGS) $^ -o $@


main.o: src/main.c include/sven.h
	$(CC) $(CFLAGS) -c $< -o $@ -I./include

sven.o: src/sven.c include/sven.h
	$(CC) $(CFLAGS) -c $< -o $@ -I./include

clean:
	rm -f *.o *.out

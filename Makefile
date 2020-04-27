CC=gcc
CFLAGS=-Wall -Werror -Wextra -lX11

run: poly.out
	./$<

mem_test: poly.out
	valgrind --track-origins=yes --leak-check=full -s ./$<

poly.out: main.o sven.o
	$(CC) $(CFLAGS) $^ -o $@


main.o: main.c sven.h
	$(CC) $(CFLAGS) -c $< -o $@

board.o: board.c sven.h
	$(CC) $(CFLAGS) -c $< -o $@

sven.o: sven.c sven.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.out

edit:
	vim -p Makefile main.c sven.h sven.c

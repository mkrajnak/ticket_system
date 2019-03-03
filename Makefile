all:
		gcc -pthread -Wall -Wextra -pedantic -o program ticket.c

test:
	./test.py 2 100
	./test.py 200 100
	./test.py 2000 100

clean:
		rm program out.txt

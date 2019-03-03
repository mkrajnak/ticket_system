all:
		gcc -pthread -Wall -Wextra -pedantic -o program ticket.c

test:
	python test.py 2 100
	python test.py 200 100
	python test.py 2000 100

zip:
	zip -r xkrajn02.zip Makefile ticket.c

clean:
		rm program out.txt

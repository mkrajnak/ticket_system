all:
		gcc -pthread -Wall -Wextra -pedantic -o program ticket.c

test:
	python test.py 2 100
	python test.py 200 100
	python test.py 2000 100
	python test.py 200 1000
	python test.py 200 10000
	python test.py 10000 10000000

zip:
	zip  xkrajn02.zip Makefile ticket.c

clean:
		rm -f program out.txt xkrajn02.zip

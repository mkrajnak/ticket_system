all:
		gcc -std=c11 -Wall -Wextra -pedantic -o program ticket.c 

clean:
		rm program

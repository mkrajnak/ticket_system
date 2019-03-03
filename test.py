#!/usr/bin/python
from os import system
from sys import argv

def main():
    if len(argv) != 3 and not argv[1].isdigit() and not argv[2].isdigit():
        print('./test.py <N> <M>')
        print('N - Number of Thread')
        print('M - Number of passes')
    system('make > /dev/null')
    system('./program ' + argv[1] + ' ' + argv[2] + ' > out.txt')
    with open('out.txt') as output:
        for line, i in zip(output, range(int(argv[2]))):
            ticket = int(line.split()[0])
            thread = int(line.split()[1].strip('(').strip(')'))
            assert i == ticket, 'Synchronization fail, expected ' + i
            assert thread < int(argv[2]), 'Ticked obtained by weird thread ' + thread

    print('PASS')

if __name__ == "__main__":
    main()

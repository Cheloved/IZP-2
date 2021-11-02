CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror
TESTFLAGS = -std=c99

all: compile

compile:
	$(CC) $(CFLAGS) setcal.c -o setcal 

test:
	$(CC) $(TESTFLAGS) setcal.c -o setcal

run:
	$(CC) $(TESTFLAGS) setcal.c -o setcal
	./setcal file.txt

clear:
	rm -rf setcal 

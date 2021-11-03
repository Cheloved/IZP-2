CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror
TESTFLAGS = -std=c99
DEBUGFLAGS = -std=c99 -ggdb

all: compile

compile:
	$(CC) $(CFLAGS) setcal.c -o setcal 

run:
	$(CC) $(TESTFLAGS) setcal.c -o setcal
	./setcal file.txt

debug:
	$(CC) $(DEBUGFLAGS) -m32 setcal.c -o setcal
	gdb setcal 

clear:
	rm -rf setcal 

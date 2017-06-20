# Reference: https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

CC = gcc
# CFLAGS  = -g -Wall -std=c++11
CFLAGS  = -g -Wall

all: prsa

# To create the executable file main we need the object files
# main.o:
#
prsa:  main.o
	$(CC) $(CFLAGS) -o prsa main.o

# To create the object file main.o, we need the source
# files main.cxx:
#
main.o:  main.c
	$(CC) $(CFLAGS) -c main.c

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) *.o *~
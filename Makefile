# Reference: https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

CC = g++
# CFLAGS  = -g -Wall -std=c++11
CFLAGS  = -g -Wall

all: prsa

# To create the executable file main we need the object files
# main.o:
#
prsa:  main.o
	$(CC) $(CFLAGS) -o prsa main.o -lssl -lcrypto

# To create the object file main.o, we need the source
# files main.cxx:
#
main.o:  main.cxx
	$(CC) $(CFLAGS) -c main.cxx -I /usr/local/opt/openssl/include

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) *.o *~
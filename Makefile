# Reference: https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html

CC = g++
# CFLAGS  = -g -Wall -std=c++11
CFLAGS  = -g -Wall

all: paes clean

# To create the executable file main we need the object files
# main.o:
#
paes:  main.o  utils.o crypto.o
	$(CC) $(CFLAGS) -o ./bin/paes main.o utils.o crypto.o -lssl -lcrypto

# To create the object file main.o, we need the source
# files main.cxx:
#
main.o:  main.cxx crypto.h
	$(CC) $(CFLAGS) -c main.cxx -I /usr/local/opt/openssl/include

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#

utils.o:  utils.cxx utils.h 
	$(CC) $(CFLAGS) -c utils.cxx

crypto.o:  crypto.cxx crypto.h utils.h
	$(CC) $(CFLAGS) -c crypto.cxx -I /usr/local/opt/openssl/include



clean: 
	$(RM) *.o *~

# Uncomment this for SCO.  (Note, this has only been reported to work with
# Revision 3.2.4 with the "SCO TCP/IP Development System" package installed.
# Please let me know if you have any other SCO success stories.
# C_LINK = -lsocket

# mysql flags


DEBUG= -g
COMM_DIR = lib
BINARY = srunner
SVROBJS = decoder.o script.o main.o

# You might have to change this if your c compiler is not cc
CC = cc
CPP = g++


# Comment the following line if you are not using the gnu c compiler
# C_ARGS = -Wall

C_ARGS = -O3 -g -I../include -I. 

# You shouldn't need to make any more changes below this line.

#.c.o:
#	$(CPP) -c $*.c

all:$(BINARY)

#ts: ts_server


srunner: $(SVROBJS)
	rm -f $@
	$(CPP) $(C_ARGS) -o $@ $(SVROBJS) -lpthread -lcrypt 

decoder.o: decoder.cpp script.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ decoder.cpp

script.o: script.cpp script.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ script.cpp

main.o: main.cpp script.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ main.cpp

install:
#	cp $(BINARY) $(HOME)/bin
#	qftp 172.30.10.94 $(BINARY)

clean:
	rm -f $(SVROBJS) $(BINARY)



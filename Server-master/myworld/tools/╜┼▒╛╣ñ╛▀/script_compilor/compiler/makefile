# Uncomment this for SCO.  (Note, this has only been reported to work with
# Revision 3.2.4 with the "SCO TCP/IP Development System" package installed.
# Please let me know if you have any other SCO success stories.
# C_LINK = -lsocket

# mysql flags


DEBUG= -g
COMM_DIR = lib
BINARY = scompile
SVROBJS = export.o mcmake.o readini.o listmac.o outdata.o str2num.o uedit.o parselogicexp.o

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


scompile: $(SVROBJS)
	rm -f $@
	$(CPP) $(C_ARGS) -o $@ $(SVROBJS) -lpthread -lcrypt 

export.o: export.cpp mchead.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ export.cpp

mcmake.o: mcmake.cpp mchead.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ mcmake.cpp

readini.o: readini.cpp mchead.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ readini.cpp

listmac.o: listmac.cpp mchead.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ listmac.cpp

outdata.o: outdata.cpp mchead.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ outdata.cpp

str2num.o: str2num.cpp mchead.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ str2num.cpp

uedit.o: uedit.cpp mchead.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ uedit.cpp

parselogicexp.o: parselogicexp.cpp mchead.h
	rm -f $@
	$(CPP) $(C_ARGS) -c -o $@ parselogicexp.cpp

install:
#	cp $(BINARY) $(HOME)/bin
#	qftp 172.30.10.94 $(BINARY)

clean:
	rm -f $(SVROBJS) $(BINARY)



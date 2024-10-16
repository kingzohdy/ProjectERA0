include ../../../src/mmog.mk

CC=gcc

APEXPROXY_OBJ=ApexProxyLib.o
	
APEXPROXY_LIB=ApexProxyLib.a

LIB=$(APEXPROXY_LIB) 
LIB_CFLAGS = $(CFLAGS)  


.PHONY: all clean 

all: $(LIB) 

.c.o:
	$(CC) $(LIB_CFLAGS) $(DEBUG) -c $<

.cpp.o:
	$(CC) $(LIB_CFLAGS) $(DEBUG) -c $<
	
$(APEXPROXY_LIB):$(APEXPROXY_OBJ)
	ar r ../../../src/lib/$@ $^

clean:
	$(RM) *.o 

	


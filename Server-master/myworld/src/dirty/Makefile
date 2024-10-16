include ../mmog.mk

CC=gcc

TDIRTY_OBJ=tdirtyapi.o base.o \
	md5.o sema.o CharacterMap.o convert.o \
	OIDirty.o OIDirtyInterface.o 
	
TDIRTY_API_LIB=libtdirty.a

LIB=$(TDIRTY_API_LIB) 
LIB_LIB = -lm
LIB_CFLAGS = $(CFLAGS) $(MMOG_INC) -I$(GAME_COMM_INC)/apps/ $(LIB_LIB) 


.PHONY: all clean 

all: $(LIB) 

.c.o:
	$(CC) $(LIB_CFLAGS) $(DEBUG) -c $<

.cpp.o:
	$(CC) $(LIB_CFLAGS) $(DEBUG) -c $<
	
$(TDIRTY_API_LIB):$(TDIRTY_OBJ)
	ar r ../lib/$@ $^

clean:
	$(RM) *.o 

	


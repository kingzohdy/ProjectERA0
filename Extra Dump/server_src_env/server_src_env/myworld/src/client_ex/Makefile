#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.6 2011/07/26 04:07:30 ouyang Exp $
#  @author $Author: ouyang $
#  @date $Date: 2011/07/26 04:07:30 $
#  @version $Revision: 1.6 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../mmog.mk

APP_NAME=client

#APP_CFILE=$(wildcard *.c)
APP_CFILE=client.c
APP_OBJ=$(APP_CFILE:.c=.o)

ZONE_CFLAGS = $(CFLAGS) $(MMOG_INC)
ZONE_LIB  = $(LDPATH) $(MMOG_LIB)/libnet_cs.a $(MMOG_LIB)/libmycomm.a $(MMOG_LIB)/librsa.a  $(GAME_COMM_LIB)/libcrypto.a  $(MMOG_LIB)/libresloader.a $(GAME_COMM_LIB)/libgamecomm.a  -Wl,-Bstatic -lscew -lexpat  -Wl,-Bdynamic -ldl 

.PHONY: all clean 

all: $(APP_NAME)

%.o: %.c 
	$(CC) -c $(ZONE_CFLAGS) $(DEBUG) $<
	
$(APP_NAME): $(APP_OBJ)
	$(CC) -g -o $(APP_NAME) $(ZONE_CFLAGS)  $^ $(ZONE_LIB) -lpthread

clean:
	$(RM) *.o  *.d
	$(RM) $(APP_NAME)
	


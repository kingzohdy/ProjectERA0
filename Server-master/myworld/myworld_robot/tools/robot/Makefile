#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
#  @author $Author: jerry $
#  @date $Date: 2011/01/05 10:59:28 $
#  @version $Revision: 1.1.1.1 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../../../myworld/src/mmog.mk

APP_NAME=robot

APP_CFILE=$(wildcard *.c)
APP_OBJ=$(APP_CFILE:.c=.o)
APP_INC=$(MMOG_INC) -I$(GAME_COMM_INC)/apps/

APP_CFLAGS = $(CFLAGS) $(APP_INC) -g
APP_LIB = $(MMOG_LIB)/libnet_cs.a $(MMOG_LIB)/libmycomm.a $(GAME_COMM_LIB)/libgamecomm.a  \
		  -Wl,-Bstatic -lscew -lexpat -Wl,-Bdynamic  -lm  

.PHONY: all clean 

all: $(APP_NAME)

-include $(APP_CFILE:.c=.d)

%.d: %.c
	$(CC) -MM $(APP_CFLAGS) $< -o $@

%.o: %.c %.d
	$(CC) -c  $(APP_CFLAGS) $(DEBUG) $<
	
$(APP_NAME): $(APP_OBJ)
	$(CC) -o  $(APP_NAME) $(APP_CFLAGS)  $^ $(APP_LIB) -lpthread

clean:
	$(RM) *.o  *.d
	$(RM) $(APP_NAME)
	


#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.2 2010/01/19 07:57:26 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2010/01/19 07:57:26 $
#  @version $Revision: 1.2 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../../mmog.mk

APP_NAME=testgd

APP_CFILE=$(wildcard *.c)
APP_OBJ=$(APP_CFILE:.c=.o)
ZONE_INC=$(MMOG_INC) -I$(GAME_COMM_INC)/apps/

ZONE_CFLAGS = $(CFLAGS) $(ZONE_INC)
ZONE_LIB = $(LDPATH) $(MMOG_LIB)/libdirty.a $(MMOG_LIB)/libptgen.a \
   -Wl,-Bstatic -lscew -lexpat -lgd -lfreetype  -lz  -lm -Wl,-Bdynamic 

.PHONY: all clean 

all: $(APP_NAME) 

-include $(APP_CFILE:.c=.d)

%.d: %.c
	$(CC) -MM $(ZONE_CFLAGS) $< -o $@

%.o: %.c %.d
	$(CC) -c $(ZONE_CFLAGS) $(DEBUG) $<
	
$(APP_NAME): $(APP_OBJ)
	$(CC) -o $(APP_NAME) $(ZONE_CFLAGS)  $^ $(ZONE_LIB) 

clean:
	$(RM) *.o  *.d
	$(RM) $(APP_NAME)
	


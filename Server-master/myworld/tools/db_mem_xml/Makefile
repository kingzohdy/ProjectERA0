#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.2 2009/11/24 08:11:36 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2009/11/24 08:11:36 $
#  @version $Revision: 1.2 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../../../myworld/src/mmog.mk

APP_NAME=db_mem_xml

APP_CFILE=$(wildcard *.c)
APP_OBJ=$(APP_CFILE:.c=.o)
ZONE_INC=$(MMOG_INC) -I$(GAME_COMM_INC)/apps/ -I$(MMOG_SRC)/zone/zone_svr/

ZONE_CFLAGS = $(CFLAGS) $(ZONE_INC) -g
ZONE_LIB = $(LDPATH) $(MMOG_LIB)/libdirty.a $(MMOG_LIB)/libptgen.a $(MMOG_LIB)/libnet_cs.a $(MMOG_LIB)/libmycomm.a $(MMOG_LIB)/libresloader.a   $(GAME_COMM_LIB)/libtormprotocolapi.a $(MMOG_LIB)/libscript.a  $(MMOG_LIB)/libmap.a $(GAME_COMM_LIB)/libgamecomm.a   \
   -Wl,-Bstatic -lscew -lexpat -lgd -lmysqlclient -lz -lcrypt -lnsl -lm -Wl,-Bdynamic

.PHONY: all clean 

all: $(APP_NAME)

-include $(APP_CFILE:.c=.d)

%.d: %.c
	$(CC) -MM $(ZONE_CFLAGS) $< -o $@

%.o: %.c %.d
	$(CC) -c $(ZONE_CFLAGS) $(DEBUG) $<
	
$(APP_NAME): $(APP_OBJ)
	$(CC) -o $(APP_NAME) $(ZONE_CFLAGS)  $^ $(ZONE_LIB) -lpthread

make_err:
	./makemmogerr

clean:
	$(RM) *.o  *.d
	$(RM) $(APP_NAME)
	


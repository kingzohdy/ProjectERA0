#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.4 2013/03/07 08:44:52 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2013/03/07 08:44:52 $
#  @version $Revision: 1.4 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../mmog.mk
APP_NAME=mud_svr

APP_CFILE=$(wildcard *.c)
APP_OBJ=$(APP_CFILE:.c=.o)
APP_INC=$(MMOG_INC) -I$(GAME_COMM_INC)/apps/ -I$(MMOG_SRC)/zone/zone_svr/ -I$(MMOG_HOME)/tools/apex

APP_CFLAGS = $(CFLAGS) $(APP_INC)
APP_LIB = $(LDPATH) $(MMOG_LIB)/libdirty.a \
		$(MMOG_LIB)/libtdirty.a \
		$(MMOG_LIB)/libptgen.a \
		$(MMOG_LIB)/libnet_cs.a  \
		$(MMOG_LIB)/libmycomm.a \
		$(MMOG_LIB)/librsa.a  \
		$(GAME_COMM_LIB)/libcrypto.a  \
		$(MMOG_LIB)/libresloader.a \
		$(GAME_COMM_LIB)/libtormprotocolapi.a \
		$(MMOG_LIB)/libscript.a  \
		$(MMOG_LIB)/libmap.a \
		$(GAME_COMM_LIB)/libgamecomm.a   \
		$(MMOG_LIB)/ApexProxyLib.a  \
		-Wl,-Bstatic -lscew -lexpat -lgd  -Wl,-Bdynamic -lmysqlclient -ldl
.PHONY: all clean 

all: $(APP_NAME)

-include $(APP_CFILE:.c=.d)

%.d: %.c
	$(CC) -MM $(APP_CFLAGS) $< -o $@

%.o: %.c %.d
	$(CC) -c $(APP_CFLAGS) $(DEBUG) $<
	
$(APP_NAME): $(APP_OBJ)
	@cd $(MMOG_HOME)/tools/apex/apexproxy;make clean;make
	$(CC) -o $(APP_NAME) $(APP_CFLAGS)  $^ $(APP_LIB) -lpthread

clean:
	$(RM) *.o  *.d
	$(RM) $(APP_NAME)
install:
	rm -f /usr/local/mmog/region/bin/bill_svr
	cp bill_svr /usr/local/mmog/region/bin/


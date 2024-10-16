#  @file $RCSfile: cs_test.mk,v $
#  general description of this module
#  $Id: cs_test.mk,v 1.5 2010/10/28 08:01:04 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2010/10/28 08:01:04 $
#  @version $Revision: 1.5 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

APP_NAME=cs_test

ZONE_CFLAGS = -g
ZONE_LIB = ./libgamecomm.a ./libresloader.a ./libcs_cgi.a ./libexpat.a ./libscew.a ./libmysqlclient.a -lz
                      

.PHONY: all clean 

all: $(APP_NAME)

-include $(APP_CFILE:.c=.d)

proto_comm.h:
proto_ss.h:
	cd ../meta_xml && make

cs_cgi.h:
	cd ../comm && make

cs_test.o: cs_test.c proto_comm.h proto_ss.h cs_cgi.h
	$(CC) -c $(ZONE_CFLAGS) $(DEBUG) cs_test.c

libcs_cgi.a:
	make -f Makefile

$(APP_NAME): cs_test.o libcs_cgi.a
	$(CC) -o $(APP_NAME) $(ZONE_CFLAGS)  cs_test.o libcs_cgi.a $(ZONE_LIB) -lpthread

make_err:
	./makemmogerr

clean:
	$(RM) *.o  *.d  proto_*.h
	$(RM) $(APP_NAME)
	


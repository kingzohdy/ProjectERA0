#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.6 2012/12/05 01:36:51 tjw Exp $
#  @author $Author: tjw $
#  @date $Date: 2012/12/05 01:36:51 $
#  @version $Revision: 1.6 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../mmog.mk

CS_CGI_LIB=libcs_cgi.a
CS_CGI_OBJ=cs_cgi.o comm_misc.o cgi_conf_desc.o

ZONE_INC=$(MMOG_INC) -I/usr/local/mysql/include/mysql 

ZONE_CFLAGS = $(CFLAGS) $(ZONE_INC) -g

.PHONY: all clean 

all: $(CS_CGI_LIB)

-include $(APP_CFILE:.c=.d)

proto_cs.h:
proto_comm.h:
proto_ss.h:
cgi_conf_desc.c:
cgi_conf_desc.h:
	cd ../meta_xml && make

comm_misc.o:
cs_cgi.o:
cs_cgi.h:
	cd ../comm && make

cgi_conf_desc.o: cgi_conf_desc.c cgi_conf_desc.h
	$(CC) -c $(ZONE_CFLAGS) $(DEBUG) cgi_conf_desc.c

$(CS_CGI_LIB): $(CS_CGI_OBJ) cs_cgi.h
	ar rsv $(CS_CGI_LIB) $(CS_CGI_OBJ)
	cp -f $(CS_CGI_LIB) ../world_scan/
	cp -f $(CS_CGI_LIB) ../cs_service/
	
make_err:
	./makemmogerr

clean:
	$(RM) *.o  *.d  proto_*.h  cgi_conf_desc.*  cs_cgi.{c,o,d,h}
	$(RM) $(CS_CGI_LIB)
	make -f *.mk clean
	


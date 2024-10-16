#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.12 2013/04/11 10:14:22 tjw Exp $
#  @author $Author: tjw $
#  @date $Date: 2013/04/11 10:14:22 $
#  @version $Revision: 1.12 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../../../myworld/src/mmog.mk

APP_NAME=cs_service

RAW_H=cs_serviceRaw.h
RAW_WSDL=cs_service.wsdl
CFG_XML=cs_service.xml
CS_CGI_LIB=libcs_cgi.a
CS_SERVICE_LIB=libcs_service.a
SOAP_PRE=soapCsService
CS_SERVICE_OBJ=$(SOAP_PRE)ClientLib.o
CS_SERVICE_LIB_C=$(SOAP_PRE)ClientLib.c

APP_CFILE=$(wildcard *.c)
APP_OBJ=$(APP_CFILE:.c=.o)
ZONE_INC=$(MMOG_INC) -I$(GAME_COMM_INC)/apps/ -I$(MMOG_SRC)/zone/zone_svr/

#ZONE_CFLAGS = $(CFLAGS) $(ZONE_INC) -g -DWITH_FASTCGI -DSOAP_DEBUG
ZONE_CFLAGS = $(CFLAGS) $(ZONE_INC) -g -DWITH_FASTCGI
ZONE_LIB = $(LDPATH) $(MMOG_LIB)/libdirty.a $(MMOG_LIB)/libptgen.a $(MMOG_LIB)/libnet_cs.a $(MMOG_LIB)/libmycomm.a $(MMOG_LIB)/libresloader.a   $(GAME_COMM_LIB)/libtormprotocolapi.a $(MMOG_LIB)/libscript.a  $(MMOG_LIB)/libmap.a $(GAME_COMM_LIB)/libgamecomm.a   \
	./$(CS_CGI_LIB) \
   	-Wl,-Bstatic -lscew -lexpat -lgd -lmysqlclient -lz -lcrypt -lnsl -lm -lfcgi -Wl,-Bdynamic

.PHONY: all clean 

all:  $(CS_SERVICE_LIB) $(APP_NAME) $(CFG_XML)

-include $(APP_CFILE:.c=.d)

$(CS_SERVICE_LIB):$(CS_SERVICE_OBJ)
	rm -rf $(CS_SERVICE_LIB)
	ar rsv $(CS_SERVICE_LIB) $^
	#$(RM) $(CS_SERVICE_OBJ)
	$(RM) $(CS_SERVICE_LIB_C)

$(CS_CGI_LIB):
	cd ../comm && make;
	cd ../cs_cgi && make clean; make

%.d: %.c $(RAW_H)
	$(CC) -MM $(ZONE_CFLAGS) $< -o $@

%.o: %.c %.d
	$(CC) -c $(ZONE_CFLAGS) $(DEBUG) $<

$(RAW_H): $(RAW_WSDL)
	#wsdl2h -a -c cs_service.wsdl -o $(RAW_H) 
	wsdl2h -n$(SOAP_PRE) -c cs_service.wsdl -o $(RAW_H) 
	soapcpp2 -p$(SOAP_PRE) -n -c -x -e $(RAW_H)
	$(RM) $(SOAP_PRE)ServerLib.c $(SOAP_PRE)ServerC.c

wsdl:$(RAW_H)
	echo wsdl

$(APP_NAME):$(APP_OBJ) $(CS_CGI_LIB)
	$(CC) -o $(APP_NAME) $(ZONE_CFLAGS)  $(APP_OBJ) $(ZONE_LIB) -lpthread

$(CFG_XML): $(APP_NAME)
	./$(APP_NAME) --generate

make_err:
	./makemmogerr

clean:cleanlib
	$(RM) *.o  *.d
	$(RM) $(APP_NAME)
	$(RM) $(RAW_H)
	$(RM) $(SOAP_PRE)*.c $(SOAP_PRE)*.h
	$(RM) soap*.c soap*.h
	$(RM) CsServiceSoap.*
	$(RM) RECV.log TEST.log
	$(RM) *.nsmap 

cleanlib:
	$(RM) $(CS_CGI_LIB)

DEPDIR = ~/service_bin/cs/
deploy:
	cp -f *.sh $(DEPDIR)/cfg/
	cp -f $(APP_NAME) $(DEPDIR)/bin/
	cp -f ../../cfg/proto_ss.tdr $(DEPDIR)/cfg/


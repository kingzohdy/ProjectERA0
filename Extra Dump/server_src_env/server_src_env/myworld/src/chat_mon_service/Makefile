
#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.4 2013/01/29 11:12:42 tjw Exp $
#  @author $Author: tjw $
#  @date $Date: 2013/01/29 11:12:42 $
#  @version $Revision: 1.4 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../../../myworld/src/mmog.mk

APP_NAME=chat_mon_service

RAW_H=chat_mon_serviceRaw.h
RAW_WSDL=chat_mon_service.wsdl
CFG_XML=chat_mon_service.xml
STD_SOAP_OBJ=stdsoap2.o
STD_SOAP_C=../cs_service/stdsoap2.c
STD_SOAP_LOCAL_C=stdsoap2.c


APP_CFILE=$(wildcard *.c)
APP_OBJ=$(APP_CFILE:.c=.o)
#APP_OBJ+=$(STD_SOAP_OBJ)
ZONE_INC=$(MMOG_INC) -I$(GAME_COMM_INC)/apps/ -I$(MMOG_SRC)/zone/zone_svr/

ZONE_CFLAGS = $(CFLAGS) $(ZONE_INC) -g -DWITH_FASTCGI -DSOAP_DEBUG
#ZONE_CFLAGS = $(CFLAGS) $(ZONE_INC) -g -DWITH_FASTCGI 
ZONE_LIB = $(LDPATH) $(MMOG_LIB)/libdirty.a \
	$(MMOG_LIB)/libtdirty.a \
	$(MMOG_LIB)/libptgen.a \
	$(MMOG_LIB)/libnet_cs.a \
	$(MMOG_LIB)/libmycomm.a \
	$(MMOG_LIB)/libresloader.a \
	$(GAME_COMM_LIB)/libtormprotocolapi.a \
	$(MMOG_LIB)/libscript.a \
	$(MMOG_LIB)/libmap.a \
	$(GAME_COMM_LIB)/libgamecomm.a   \
	-Wl,-Bstatic -lscew -lexpat -lgd -lmysqlclient -lz -lcrypt -lnsl -lm -lfcgi -Wl,-Bdynamic

.PHONY: all clean 

all: $(APP_NAME) $(CFG_XML) 

-include $(APP_CFILE:.c=.d)



%.d: %.c $(RAW_H) $(STD_SOAP_LOCAL_C)
	$(CC) -MM $(ZONE_CFLAGS) $< -o $@

$(STD_SOAP_LOCAL_C):$(STD_SOAP_C)
	cp $(STD_SOAP_C)  $(STD_SOAP_LOCAL_C)

#$(STD_SOAP_OBJ):$(STD_SOAP_C)
#	cd ../cs_service && make ;cp $(STD_SOAP_OBJ) ../chat_mon_service/

%.o: %.c %.d
	$(CC) -c $(ZONE_CFLAGS) $(DEBUG) $<

$(RAW_H): $(RAW_WSDL)
	wsdl2h -a -c chat_mon_service.wsdl -o $(RAW_H) 
	soapcpp2 -c -L -e $(RAW_H)

$(APP_NAME):$(APP_OBJ) $(CS_CGI_LIB)
	$(CC) -o $(APP_NAME) $(ZONE_CFLAGS)  $(APP_OBJ) $(ZONE_LIB) -lpthread

$(CFG_XML): $(APP_NAME)
	./$(APP_NAME) --generate

make_err:
	./makemmogerr

clean:
	$(RM) *.o  *.d
	$(RM) $(APP_NAME)
	$(RM) $(RAW_H)
	$(RM) soap*.c soap*.h
	$(RM) ChatMonServiceSoap.*
	$(RM) *.tgz
	

cleanlib:
	$(RM) $(CS_CGI_LIB)

DEPDIR = ~/service_bin/chat_mon/
deploy:
	cp -f *.sh $(DEPDIR)/cfg/
	cp -f $(APP_NAME) $(DEPDIR)/bin/
	cp -f ../../cfg/db_meta/role_db_meta.tdr $(DEPDIR)/cfg/




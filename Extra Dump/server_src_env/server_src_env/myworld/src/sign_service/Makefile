
#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.3 2013/08/07 06:45:28 tjw Exp $
#  @author $Author: tjw $
#  @date $Date: 2013/08/07 06:45:28 $
#  @version $Revision: 1.3 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../../../myworld/src/mmog.mk

APP_NAME=sign_service

RAW_H=sign_serviceRaw.h
RAW_WSDL=sign_service.wsdl
LJY_WSDL=sign_service_ljy.wsdl
CFG_XML=sign_service.xml
STD_SOAP_OBJ=stdsoap2.o
STD_SOAP_C=../cs_service/stdsoap2.c
STD_SOAP_LOCAL_C=stdsoap2.c
#CS_SERVICE_LIB=libcs_service.a
#OR_CS_SERVICE_LIB=../cs_service/$(CS_SERVICE_LIB)
#CS_SERVICE_H=../cs_service/soapCsServiceStub.h
#CS_SERVICE_WSDL=../cs_service/cs_service.wsdl


APP_CFILE=$(wildcard *.c)
APP_OBJ=$(APP_CFILE:.c=.o)
#APP_OBJ+=$(STD_SOAP_OBJ)
ZONE_INC=$(MMOG_INC) -I$(GAME_COMM_INC)/apps/ -I$(MMOG_SRC)/zone/zone_svr/

#ZONE_CFLAGS = $(CFLAGS) $(ZONE_INC) -g -DWITH_FASTCGI -DSOAP_DEBUG
ZONE_CFLAGS = $(CFLAGS) $(ZONE_INC) -g -DWITH_FASTCGI 
ZONE_LIB = $(LDPATH) $(MMOG_LIB)/libnet_cs.a $(MMOG_LIB)/libdirty.a $(MMOG_LIB)/libptgen.a $(MMOG_LIB)/libmycomm.a $(MMOG_LIB)/libresloader.a   $(GAME_COMM_LIB)/libtormprotocolapi.a $(MMOG_LIB)/libscript.a  $(MMOG_LIB)/libmap.a $(GAME_COMM_LIB)/libgamecomm.a $(MMOG_LIB)/librsa.a $(GAME_COMM_LIB)/libcrypto.a \
	-Wl,-Bstatic -lscew -lexpat -lgd -lmysqlclient -lz -lcrypt -lnsl -lm -lfcgi -Wl,-Bdynamic -ldl

.PHONY: all clean 

all: $(APP_NAME) $(CFG_XML) $(LJY_WSDL) 

-include $(APP_CFILE:.c=.d)



%.d: %.c $(RAW_H) $(STD_SOAP_LOCAL_C)
	$(CC) -MM $(ZONE_CFLAGS) $< -o $@

$(STD_SOAP_LOCAL_C):$(STD_SOAP_C)
	cp $(STD_SOAP_C) $(STD_SOAP_LOCAL_C)

#$(STD_SOAP_OBJ):$(STD_SOAP_C)
#	cd ../cs_service && make ;cp $(STD_SOAP_OBJ) ../sign_service/

%.o: %.c %.d
	$(CC) -c $(ZONE_CFLAGS) $(DEBUG) $<

#$(CS_SERVICE_H):$(CS_SERVICE_WSDL)
#	cd ../cs_service/ && make wsdl

#$(CS_SERVICE_LIB):$(OR_CS_SERVICE_LIB)	
#	cp -f ../cs_service/$(CS_SERVICE_LIB) $(CS_SERVICE_LIB)

#$(OR_CS_SERVICE_LIB):
#	cd ../cs_service/ && make

$(RAW_H): $(RAW_WSDL) 
	wsdl2h -a -c sign_service.wsdl -o $(RAW_H) 
	soapcpp2 -c -L -e $(RAW_H)

wsdl: $(RAW_H)
	echo wsdl done
$(APP_NAME):$(APP_OBJ) $(CS_CGI_LIB) $(CS_SERVICE_LIB)
	$(CC) -o $(APP_NAME) $(ZONE_CFLAGS)  $(APP_OBJ) $(ZONE_LIB) -lpthread

$(LJY_WSDL): $(RAW_WSDL)
	cp -f $(RAW_WSDL) $(LJY_WSDL)

$(CFG_XML): $(APP_NAME)
	./$(APP_NAME) --generate
	rm *.log

make_err:
	./makemmogerr

clean:
	$(RM) *.o  *.d
	$(RM) $(APP_NAME)
	$(RM) $(RAW_H)
	$(RM) soap*.c soap*.h
	$(RM) SignServiceSoap.*
	$(RM) *.tgz
	$(RM) -rf version_sign_service/
	

cleanlib:
	$(RM) $(CS_CGI_LIB)

DEPDIR = ~/service_bin/sign/
deploy:
	cp -f *.sh $(DEPDIR)/cfg/
	cp -f $(APP_NAME) $(DEPDIR)/bin/
	cp -f ../../cfg/db_meta/role_db_meta.tdr $(DEPDIR)/cfg/




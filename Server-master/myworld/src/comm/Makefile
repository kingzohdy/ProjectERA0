#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.24 2012/12/21 03:19:10 tjw Exp $
#  @author $Author: tjw $
#  @date $Date: 2012/12/21 03:19:10 $
#  @version $Revision: 1.24 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../mmog.mk

CS_LIB=$(MMOG_LIB)/libnet_cs.a
CS_OBJ=cs_net.o id_rsa_pub.o md5.o
COMM_LIB=$(MMOG_LIB)/libmycomm.a
COMM_OBJ=black_white.o proto_cs.o proto_ss.o tconnapi.o comm_misc.o op_log_meta.o cs_cgi.o dirreport.o taes.o sig_key.o file_lock.o comm_timer.o cgi_conf_desc.o
RSA_OBJ=id_rsa.o grav_rsa.o
RSA_LIB=$(MMOG_LIB)/librsa.a
MAP_OBJ=poly.o mapproc.o mapmask.o
MAP_LIB=$(MMOG_LIB)/libmap.a
CONFUSE_OBJ=confuse.o
CONFUSE=confuse
DIRTY_LIB=$(MMOG_LIB)/libdirty.a
DIRTY_OBJ=libdirty.o tlib_dirty_check.o dir_mm.o
PTGEN_LIB=$(MMOG_LIB)/libptgen.a
PTGEN_OBJ=ptgenimg.o
SCRIPT_LIB=$(MMOG_LIB)/libscript.a
SCRIPT_OBJ=og_script.o

COMM_CFLAGS = $(CFLAGS) $(MMOG_INC) -I/usr/local/mysql/include/mysql
COMM_IMPORT_LIBS = $(GAME_COMM_LIB)/libgamecomm.a -Wl,-Bstatic -lscew -lexpat -Wl,-Bdynamic  -lm  

.PHONY: all clean 

all: $(CS_LIB) $(COMM_LIB) $(RSA_LIB)  $(MAP_LIB) $(CONFUSE) $(DIRTY_LIB) $(PTGEN_LIB) $(SCRIPT_LIB) cs_cgi_export

%.o: %.c 
	$(CC) -c $(COMM_CFLAGS) $<

$(CS_LIB):$(CS_OBJ)
	rm -rf $(CS_LIB)
	ar rsv $(CS_LIB) $^

$(RSA_LIB):$(RSA_OBJ)
	rm -rf $(RSA_LIB)
	ar rsv $(RSA_LIB) $^
	
$(COMM_LIB):$(COMM_OBJ)
	rm -rf $(COMM_LIB)
	ar rsv $(COMM_LIB) $^

$(SCRIPT_LIB):$(SCRIPT_OBJ)
	rm -rf $(SCRIPT_LIB)
	ar rsv $(SCRIPT_LIB) $^	

$(MAP_LIB):$(MAP_OBJ)
	rm -rf $(MAP_LIB)
	ar rsv 	$(MAP_LIB) $^	

$(CONFUSE):$(CONFUSE_OBJ)
	$(CC) -o $(CONFUSE) $^
	./confuse proto_cs.tdr proto_cs.meta
	cp -f proto_cs.meta ../../myworld_robot/src/comm/

cs_cgi_export:comm_misc.o cs_cgi.o
	cp -f comm_misc.o ../cs_cgi/
	cp -f cs_cgi.o ../cs_cgi/
	cp -f cs_cgi.h ../cs_cgi/
	cp -f cs_cgi.c ../../tools/win32_cs_cgi_demos/cs_cgi_dll/
	cp -f cs_cgi.h ../../tools/win32_cs_cgi_demos/cs_cgi_dll/

$(DIRTY_LIB):$(DIRTY_OBJ)
	rm -rf $(DIRTY_LIB)
	ar rsv 	$(DIRTY_LIB) $^	

$(PTGEN_LIB):$(PTGEN_OBJ)
	rm -rf $(PTGEN_LIB)
	ar rsv 	$(PTGEN_LIB) $^
	
clean:
	$(RM) *.o  *.d
	$(RM) -rf $(CS_LIB) $(COMM_OBJ) $(MAP_LIB) $(DIRTY_LIB) $(PTGEN_LIB)

	


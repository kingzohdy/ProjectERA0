#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
#  @author $Author: jerry $
#  @date $Date: 2011/01/05 10:59:28 $
#  @version $Revision: 1.1.1.1 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

include ../mmog.mk

CS_LIB=$(MMOG_LIB)/libnet_cs.a
CS_OBJ=cs_net.o
COMM_LIB=$(MMOG_LIB)/libmycomm.a
COMM_OBJ=black_white.o proto_cs.o proto_ss.o tconnapi.o comm_misc.o

COMM_CFLAGS = $(CFLAGS) $(MMOG_INC)
COMM_IMPORT_LIBS = $(GAME_COMM_LIB)/libcomm.a $(GAME_COMM_LIB)/libpal.a $(GAME_COMM_LIB)/libtdr_dbms_xml.a -Wl,-Bstatic -lscew -lexpat -Wl,-Bdynamic  -lm  

.PHONY: all clean 

all: $(CS_LIB) $(COMM_LIB)

%.o: %.c 
	$(CC) -c $(COMM_CFLAGS) $<

$(CS_LIB):$(CS_OBJ)
	ar rsv $(CS_LIB) $^
	
$(COMM_LIB):$(COMM_OBJ)
	ar rsv $(COMM_LIB) $^			  
	
clean:
	$(RM) *.o  *.d
	$(RM) $(CS_LIB)

	


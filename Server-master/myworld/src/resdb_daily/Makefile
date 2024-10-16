
include ../mmog.mk

APP_NAME=resdb_daily

APP_CFILE=$(wildcard *.cc)
APP_OBJ=$(APP_CFILE:.cc=.o)
ZONE_INC=$(MMOG_INC) -I$(GAME_COMM_INC)/apps/

ZONE_CFLAGS=-Wno-deprecated $(CFLAGS) $(ZONE_INC)
ZONE_LIB=$(LDPATH) $(MMOG_LIB)/libdirty.a $(MMOG_LIB)/libptgen.a $(MMOG_LIB)/libnet_cs.a $(MMOG_LIB)/libmycomm.a $(MMOG_LIB)/libresloader.a   $(GAME_COMM_LIB)/libtormprotocolapi.a $(MMOG_LIB)/libscript.a  $(MMOG_LIB)/libmap.a $(GAME_COMM_LIB)/libgamecomm.a   \
   -Wl,-Bstatic -lscew -lexpat -lgd -lmysqlclient -lz -lcrypt -lnsl -lm -Wl,-Bdynamic

.PHONY: all clean

all: $(APP_NAME) 

-include $(APP_CFILE:.cc=.d)

%.d: %.cc
	$(CXX) -MM $(ZONE_CFLAGS) $< -o $@

%.o: %.cc %.d
	$(CXX) -c $(ZONE_CFLAGS) $(DEBUG) $<

$(APP_NAME): $(APP_OBJ)
	$(CXX) -o $(APP_NAME) $(ZONE_CFLAGS)  $^ $(ZONE_LIB) -lpthread

clean:
	$(RM) *.o  *.d
	$(RM) $(APP_NAME)

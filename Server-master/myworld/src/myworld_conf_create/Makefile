include ../mmog.mk

APP_NAME=myworld_config.so

APP_CFILE=$(wildcard *.c)
APP_OBJ=$(APP_CFILE:.c=.o)
#ZONE_INC = -I$(GAME_COMM_HOME)/services/tlog_service/

LIB = -Wl,-Bstatic $(GAME_COMM_HOME)/apps/tcm/lib/libtcmapi.a ${GAME_COMM_LIB}/libgamecomm.a  \
	-lscew -lexpat -Wl,-Bdynamic -lpthread
APP_FLAGES = $(CFLAGS) $(MMOG_INC) -I$(GAME_COMM_HOME)/apps/tcm/include  -I$(GAME_COMM_HOME)/services/tlog_service -I$(GAME_COMM_HOME)/include/tlog

.PHONY: all clean

all: $(APP_NAME)

-include $(APP_CFILE:.c=.d)

%.d: %.c
	$(CC) $(APP_FLAGES)  -MM $< -o $@ 

%.o: %.c %.d
	$(CC) $(APP_FLAGES)  -c  $<
	
$(APP_NAME): ${APP_OBJ}
	$(CC) ${APP_OBJ} -shared -o $(APP_NAME) $(APP_FLAGES)  $(LIB)
	cp $(APP_NAME) ../../cfg/tcm/
	
clean:
	$(RM) *.o *.d
	$(RM) $(APP_NAME)

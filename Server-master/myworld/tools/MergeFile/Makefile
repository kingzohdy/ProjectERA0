#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.2 2013/05/31 01:40:45 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2013/05/31 01:40:45 $
#  @version $Revision: 1.2 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#

APP_NAME=MergeFile

APP_CFILE=$(wildcard *.c)
APP_OBJ=$(APP_CFILE:.c=.o)
INC =-I../../comm_release/include
LIB = ../../comm_release/lib/libgamecomm.a

all: $(APP_NAME)

-include $(APP_CFILE:.c=.d)

%.o: %.c 
	gcc -c -g $(INC) $<
	
$(APP_NAME): $(APP_OBJ)
	$(CC) -o $(APP_NAME) -g $^ $(LIB) 

clean:
	$(RM) *.o 
	$(RM) $(APP_NAME)
	


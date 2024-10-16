MMOG_HOME = $(HOME)/src_deploy_tmp/myworld

GAME_COMM_HOME = $(MMOG_HOME)/comm_release
GAME_COMM_INC  = $(GAME_COMM_HOME)/include
GAME_COMM_LIB  = $(GAME_COMM_HOME)/lib
GAME_COMM_TOOLS = $(GAME_COMM_HOME)/tools

MMOG_SRC = $(MMOG_HOME)/src
MMOG_INC = -I$(GAME_COMM_INC) -I$(MMOG_SRC)/comm
MMOG_LIB = $(MMOG_SRC)/lib

LDPATH = -L$(GAME_COMM_LIB)/ -L$(MMOG_LIB)/ -L/usr/local/mysql/lib/mysql/

#C compiler flags
SHARED = -shared -z defs
CFLAGS= -g  

CFLAGS+= -Wall -Wextra -pipe -D_NEW_LIC -D_GNU_SOURCE -D_REENTRANT -fPIC

CC=gcc

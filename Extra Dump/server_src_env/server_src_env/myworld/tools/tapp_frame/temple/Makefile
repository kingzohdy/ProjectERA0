#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.2 2009/10/20 08:33:29 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2009/10/20 08:33:29 $
#  @version $Revision: 1.2 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#


include ../../../src/mmog.mk 

TARGETS = \
	SHELL \

all: $(TARGETS)

SHELL:
	@./conv.sh $(GAME_COMM_TOOLS) 
	@$(MAKE) -f Makefile.real

#------------------------------------------------------
clean:
	@$(MAKE) -f Makefile.real clean

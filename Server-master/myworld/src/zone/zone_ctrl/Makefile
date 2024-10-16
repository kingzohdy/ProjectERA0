#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.1.1.1 2009/07/23 04:22:48 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2009/07/23 04:22:48 $
#  @version $Revision: 1.1.1.1 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#


include ../../mmog.mk

TARGETS = \
	SHELL \

all: $(TARGETS)

SHELL:
	@./conv.sh $(GAME_COMM_TOOLS)
	@$(MAKE) -f Makefile.real

#------------------------------------------------------
clean:
	@$(MAKE) -f Makefile.real clean

#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.1 2010/06/17 06:39:52 niko Exp $
#  @author $Author: niko $
#  @date $Date: 2010/06/17 06:39:52 $
#  @version $Revision: 1.1 $
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

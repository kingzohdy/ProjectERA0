#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.1 2014/06/07 20:40:54 lzk Exp $
#  @author $Author: lzk $
#  @date $Date: 2014/06/07 20:40:54 $
#  @version $Revision: 1.1 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#


include ../../../myworld/src/mmog.mk 

TARGETS = \
	SHELL \

all: $(TARGETS)

SHELL:
	@./conv.sh $(GAME_COMM_TOOLS) 
	@$(MAKE) -f Makefile.real

#------------------------------------------------------
clean:
	@$(MAKE) -f Makefile.real clean

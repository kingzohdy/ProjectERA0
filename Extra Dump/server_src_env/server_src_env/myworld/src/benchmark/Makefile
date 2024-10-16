#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.1 2009/10/09 08:08:43 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2009/10/09 08:08:43 $
#  @version $Revision: 1.1 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#
CLEANTARGET = clean
SUBMODS = gd
SUBMODSCLEAN =$(patsubst %, %.$(CLEANTARGET), $(SUBMODS)) 

.PHONY: all $(CLEANTARGET) $(SUBMODS) 

all: $(SUBMODS)
$(CLEANTARGET): $(SUBMODSCLEAN)
	
$(SUBMODS):
	@echo "Begin to make sub module: '$@' ......"
	cd $@ && $(MAKE);
	@echo "Finish to  make sub module: '$@'" 

$(SUBMODSCLEAN): 
	@echo "Begin to clean sub module: '$(patsubst %.$(CLEANTARGET),%, $@)' ......"
	cd $(patsubst %.$(CLEANTARGET),%, $@)  && make $(CLEANTARGET);
	@echo "Finish to  clean sub module: '$(patsubst %.$(CLEANTARGET),%, $@)'" 

#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.9 2013/01/07 09:13:39 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2013/01/07 09:13:39 $
#  @version $Revision: 1.9 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#
CLEANTARGET = clean
SUBMODS = db_to_bin robot db_mem_xml unite_svr active_code account_dyn gamestat clean_world_log export_item cdkey wid_check 
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

#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.23 2013/03/12 02:36:20 kent Exp $
#  @author $Author: kent $
#  @date $Date: 2013/03/12 02:36:20 $
#  @version $Revision: 1.23 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#
CLEANTARGET = clean
SUBMODS = meta_xml comm dirty zone dir_svr myworld_conf_create  client_ex auth_svr version_svr bill_svr tlog_db aas_svr region_gate informer_svr cs_cgi account_cs_svr mud_svr recv_report service_svr world_scan cs_service \
acc_service \
bi_service \
chat_mon_service \
#
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

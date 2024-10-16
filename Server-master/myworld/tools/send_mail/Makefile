#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.1 2013/06/13 10:42:56 lzk Exp $
#  @author $Author: lzk $
#  @date $Date: 2013/06/13 10:42:56 $
#  @version $Revision: 1.1 $
#  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
#  @note Platform: Linux
#


include ../../src/mmog.mk 

TARGETS = \
	SHELL \

all: $(TARGETS)

SHELL:
	@$(MAKE) -f Makefile.real

#------------------------------------------------------
clean:
	@$(MAKE) -f Makefile.real clean

#install:
#	rm -f /usr/local/mmog/region/bin/service_svr
#	cp service_svr /usr/local/mmog/region/bin/

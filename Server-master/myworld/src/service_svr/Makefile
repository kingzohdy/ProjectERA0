#  @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.3 2012/07/18 10:07:21 tjw Exp $
#  @author $Author: tjw $
#  @date $Date: 2012/07/18 10:07:21 $
#  @version $Revision: 1.3 $
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

install:
	rm -f /usr/local/mmog/region/bin/service_svr
	cp service_svr /usr/local/mmog/region/bin/

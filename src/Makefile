#
#   Copyright 2013, Kyle Hayes
#   Author: Kyle Hayes
#
#   See the LICENSE file for licensing information.
#


ifeq ($(LST_ROOT),)
LST_ROOT = ./
endif

	
SUBDIRS = bootstrap vm
DEFINES = DEBUG=0

all: vm/lst

vm/lst:
	@for mdir in $(SUBDIRS); do \
		if test -d $$mdir; then \
			if test -f $$mdir/configure -a ! -f $$mdir/Makefile; then \
				cd $$mdir; ./configure; cd ..; \
			fi; \
			$(MAKE) -C $$mdir PREFIX=$(prefix) $(DEFINES); \
		fi; \
	done


clean:
	-@for mdir in $(SUBDIRS); do \
		$(MAKE) -C $$mdir clean; \
	done

distclean:
	-@for mdir in $(SUBDIRS); do \
		$(MAKE) -C $$mdir distclean; \
	done

	

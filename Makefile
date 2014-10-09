# OrMonitor MakeFile

MAKE = make
SRCDIR = src


ormonit :
	$(MAKE) -C $(SRCDIR)


.PHONY : clean
clean :
	$(MAKE) -C $(SRCDIR) clean

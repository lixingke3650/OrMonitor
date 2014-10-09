# OrMonitor MakeFile

CC = @gcc
OBJDIR = ../obj
SRCDIR = src
BINDIR = ../bin

LIB = -lpthread
OBJECTS =   $(OBJDIR)/monit.o \
			$(OBJDIR)/utility.o \
			$(OBJDIR)/logger.o \
			$(OBJDIR)/itimer.o \
			$(OBJDIR)/proc.o


ormonit : $(OBJECTS)
	$(CC) -o $(BINDIR)/ormonit $(OBJECTS) $(LIB)


$(OBJDIR)/monit.o   : utility.h monit.h logger.h itimer.h proc.h
	$(CC) -o $(OBJDIR)/monit.o -c monit.c

$(OBJDIR)/utility.o : utility.h
	$(CC) -o $(OBJDIR)/utility.o -c utility.c

$(OBJDIR)/logger.o  : utility.h logger.h
	$(CC) -o $(OBJDIR)/logger.o -c logger.c

$(OBJDIR)/itimer.o  : utility.h itimer.h logger.h
	$(CC) -o $(OBJDIR)/itimer.o -c itimer.c

$(OBJDIR)/proc.o    : utility.h logger.h proc.h
	$(CC) -o $(OBJDIR)/proc.o -c proc.c


.PHONY : clean
clean :
	-@rm $(BINDIR)/ormonit $(OBJECTS)

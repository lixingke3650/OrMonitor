# OrMonitor MakeFile

CC = gcc
OBJ = obj
OBJECTS = monit.o utility.o logger.o itimer.o proc.o

ormonit : $(OBJECTS)
	$(CC) -o ormonit $(OBJECTS)

monit.o   : utility.h monit.h logger.h itimer.h proc.h
utility.o : utility.h
logger.o  : utility.h logger.h
itimer.o  : utility.h itimer.h logger.h
proc.o    : utility.h logger.h proc.h

.PHONY : clean
clean :
	-rm ormonit $(OBJECTS)

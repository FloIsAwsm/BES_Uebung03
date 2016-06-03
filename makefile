<<<<<<< HEAD
##
## @file Makefile
## Beispiel 3
##
## 
## @author Florian Froestl <florian.froestl@technikum-wien.at>
## @author David Boisits <david.boisits@technikum-wien.at>
##
## @date 2016/04/16
##
## @version $Revision: 100 $
##
##
## 

##
## ------------------------------------------------------------- variables --
##

CC=gcc52
CFLAGS=-DDEBUG -Wall -pedantic -Werror -Wextra -Wstrict-prototypes -fno-common -g -O3 -std=gnu11
LIB=-lsem182
CP=cp
CD=cd
MV=mv
GREP=grep
DOXYGEN=doxygen

OBJSEND= sender.o
OBJEMPF= empfaenger.o
OBJSHM= SharedMemory.o
BIN= sender empfaenger

EXCLUDE_PATTERN=footrulewidth

##
## ----------------------------------------------------------------- rules --
##

%.o: %.c
	$(CC) $(CFLAGS) -c $<

##
## --------------------------------------------------------------- targets --
##

all: $(BIN)

sender: $(OBJSEND) $(OBJSHM)
	$(CC) -o $@ $^ $(LIB)

empfaenger: $(OBJEMPF) $(OBJSHM)
	$(CC) -o $@ $^ $(LIB)

clean:
	$(RM) *.o *~ $(BIN)

distclean: clean
	$(RM) -r doc

doc: html pdf

html: mkdoc
	$(DOXYGEN) doxygen.dcf

pdf: html
	$(CD) doc/pdf && \
	$(MV) refman.tex refman_save.tex && \
	$(GREP) -v $(EXCLUDE_PATTERN) refman_save.tex > refman.tex && \
	$(RM) refman_save.tex && \
	make && \
	$(MV) refman.pdf refman.save && \
	$(RM) *.pdf *.html *.tex *.aux *.sty *.log *.eps *.out *.ind *.idx \
	      *.ilg *.toc *.tps Makefile && \
	$(MV) refman.save refman.pdf

mkdoc:
	mkdir -p doc

##
## ---------------------------------------------------------- dependencies --
##
=======
##
## @file Makefile
## Beispiel 3
##
## 
## @author Florian Froestl <florian.froestl@technikum-wien.at>
## @author David Boisits <david.boisits@technikum-wien.at>
##
## @date 2016/04/16
##
## @version $Revision: 100 $
##
##
## 

##
## ------------------------------------------------------------- variables --
##

CC=gcc52
CFLAGS=-DDEBUG -Wall -pedantic -Werror -Wextra -Wstrict-prototypes -fno-common -g -O3 -std=gnu11
LIB=-lsem182
CP=cp
CD=cd
MV=mv
GREP=grep
DOXYGEN=doxygen

OBJSEND= sender.o
OBJEMPF= empfaenger.o
OBJSHM= SharedMemory.o
BIN= sender empfaenger

EXCLUDE_PATTERN=footrulewidth

##
## ----------------------------------------------------------------- rules --
##

%.o: %.c
	$(CC) $(CFLAGS) -c $<

##
## --------------------------------------------------------------- targets --
##

all: $(BIN)

sender: $(OBJSEND) $(OBJSHM)
	$(CC) -o $@ $^ $(LIB)

empfaenger: $(OBJEMPF) $(OBJSHM)
	$(CC) -o $@ $^ $(LIB)

clean:
	$(RM) *.o *~ $(BIN)

distclean: clean
	$(RM) -r doc

doc: html pdf

html: mkdoc
	$(DOXYGEN) doxygen.dcf

pdf: html
	$(CD) doc/pdf && \
	$(MV) refman.tex refman_save.tex && \
	$(GREP) -v $(EXCLUDE_PATTERN) refman_save.tex > refman.tex && \
	$(RM) refman_save.tex && \
	make && \
	$(MV) refman.pdf refman.save && \
	$(RM) *.pdf *.html *.tex *.aux *.sty *.log *.eps *.out *.ind *.idx \
	      *.ilg *.toc *.tps Makefile && \
	$(MV) refman.save refman.pdf

mkdoc:
	mkdir -p doc

##
## ---------------------------------------------------------- dependencies --
##
>>>>>>> 2985f36c5a3b591503a9a8e8f93faa922bf40823

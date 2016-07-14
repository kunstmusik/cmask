SHELL=		/bin/sh
CC=		g++
CFLAGS=	        -O
LIBS=		-lm
IFLAGS=		
NAME= 		cmask


SOURCES=	cmask.cpp \
		event.cpp \
		field.cpp \
		fileio.cpp \
		gen.cpp \
		mask.cpp \
		parser.cpp \
		quant.cpp \
		items.cpp \
		tables.cpp \
		utils.cpp

OBJS= 		cmask.o \
		event.o \
		field.o \
		fileio.o \
		gen.o \
		mask.o \
		parser.o \
		quant.o  \
		items.o	\
		tables.o \
		utils.o



.SUFFIXES:	.cpp
.cpp.o:	
		$(CC) $(CFLAGS) $(IFLAGS) -c $*.cpp

$(NAME): 	$(OBJS)
	$(CC) -v $(CFLAGS) $(IFLAGS) -o $(NAME) $(OBJS) $(LIBS)

cmask.o : 	cmask.cpp 	globals.h	
event.o : 	event.cpp 	event.h 	globals.h
field.o : 	field.cpp 	field.h 	globals.h
fileio.o :     	fileio.cpp 	fileio.h 	globals.h
gen.o : 	gen.cpp		gen.h		globals.h
mask.o : 	mask.cpp	mask.h		globals.h
parser.o : 	parser.cpp	parser.h	globals.h
quant.o : 	quant.cpp	quant.h		globals.h
items.o	:	items.cpp	items.h		globals.h
tables.o : 	tables.cpp	tables.h	globals.h
utils.o	: 	utils.cpp	utils.h		globals.h

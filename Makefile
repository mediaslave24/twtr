include config.mk

BSTRLIB = bstrlib
VPATH = .:${BSTRLIB}

HEADERS = bstrlib.h
SRC = twtr.c bstrlib.c
OBJ = ${SRC:.c=.o}

all: twtr

twtr: ${OBJ}

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<
${OBJ}: ${HEADERS}

clean:
	@echo cleaning
	@rm -f ${OBJ} twtr

.PHONY: all clean

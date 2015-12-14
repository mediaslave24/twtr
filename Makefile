include config.mk

HEADERS = util.h
SRC = twtr.c util.c
OBJ = ${SRC:.c=.o}

all: twtr

twtr: ${OBJ}
	@echo ${CC} -o $@ $^ ${LDFLAGS}
	@${CC} -o $@ $^ ${LDFLAGS} 

.c.o:
	@echo ${CC} -c ${CFLAGS} $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: ${HEADERS}

clean:
	@echo cleaning
	@rm -f ${OBJ} twtr

.PHONY: all clean

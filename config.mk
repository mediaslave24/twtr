# twtr version
VERSION = 0.1

# paths
INCLUDE = /usr/include
CURLINC = ${INCLUDE}/curl

# includes and libs
INCS = -Ibstrlib -I${CURLINC} # -I${FREETYPEINC}
LIBS = -loauth -lcurl #-L${X11LIB} -lX11 ${XINERAMALIBS} ${FREETYPELIBS}

# flags
CPPFLAGS = -D_BSD_SOURCE -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS}
CFLAGS   = -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# compiler and linker
CC = gcc -g

# twtr version
VERSION = 0.1

# paths
INCLUDE = /usr/include
CURLINC = ${INCLUDE}/curl

# includes and libs
INCS = -Ibstrlib -I${CURLINC}
LIBS = -loauth -lcurl

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -DVERSION=\"${VERSION}\"
CFLAGS   = -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# compiler and linker
CC = gcc -g

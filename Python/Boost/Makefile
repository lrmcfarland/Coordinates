# ===============================================================
# Filename:    Makefile
# Description: Makefile to run build, test commands
# Author:      L.R. McFarland
# Created:     2014 Nov 13
# ===============================================================

# Detect operating system flavor.
UNAME   := $(shell uname)

# ----------------
# ----- OS X -----
# ----------------

# .dylib is only for Darwin
ifeq ($(UNAME), Darwin)

OSFLAGS = env ARCHFLAGS="-arch x86_64"

endif

# -----------------
# ----- Linux -----
# -----------------

ifeq ($(UNAME), Linux)

CC = /usr/bin/g++
CFLAGS = "-g -W -Wall -fPIC -I. -std=c++11"
LDFLAGS = -L../../libCoords

OSFLAGS = CC=${CC} CFLAGS=${CFLAGS} LDFLAGS=${LDFLAGS}

endif


# -----------------
# ----- build -----
# -----------------

RM = rm -f

all: build

build: setup.py coords.cpp
	${OSFLAGS} python setup.py build

test: build test_angle.py test_Cartesian.py test_spherical.py
	./test_coords.sh -v

clean:
	-$(RM) -r build

clobber: clean
	-$(RM) *~

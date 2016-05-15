# ===============================================================
# Filename:    Makefile
# Description: makefile to run build, test commands
# Author:      L.R. McFarland
# Created:     2016 may 15
# ===============================================================

# Detect operating system flavor.
UNAME   := $(shell uname)

# VERBOSE="-v"

# ----------------
# ----- OS X -----
# ----------------

# .dylib is only for Darwin
ifeq ($(UNAME), Darwin)

CXX = g++
CXXFLAGS = -fPIC -I.
LDFLAGS  = -lpython -dynamiclib


# TODO meh
PYINCS = /System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7

endif

# -----------------
# ----- Linux -----
# -----------------

ifeq ($(UNAME), Linux)

# TODO needs development

CC = /usr/bin/g++
CFLAGS = "-g -W -Wall -fPIC -I. -std=c++11"
LDFLAGS = -L../../libCoords


endif


# -----------------
# ----- build -----
# -----------------

SOURCE = coords.i angle.h angle.cpp Cartesian.h Cartesian.cpp

RM = rm -f

all: build

build: coords_module


coords_module: coords_wrap.o angle.o Cartesian.o utils.o
	$(CXX) $(LDFLAGS) coords_wrap.o angle.o Cartesian.o utils.o -o _coords.so

coords_wrap.cxx: $(SOURCE)
	swig -c++ -python coords.i

coords_wrap.o: coords_wrap.cxx
	$(CXX) $(CXXFLAGS) -I$(PYINCS) -c coords_wrap.cxx



angle.o: angle.h angle.cpp
	$(CXX) $(CXXFLAGS) -c angle.cpp


Cartesian.o: Cartesian.h Cartesian.cpp
	$(CXX) $(CXXFLAGS) -c Cartesian.cpp


utils.o: utils.h utils.cpp
	$(CXX) $(CXXFLAGS) -c utils.cpp


test: coords_module
	python test_Cartesian.py -v


clean:
	-$(RM) coords_wrap.o coords_wrap.cxx
	-$(RM) angle.o
	-$(RM) Cartesian.o
	-$(RM) utils.o
	-$(RM) _coords.so coords.pyc coords.py


clobber: clean
	-$(RM) *~

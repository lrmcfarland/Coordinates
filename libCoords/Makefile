# ===============================================================
# Filename:    Makefile
# Description: makes libCoords and runs test scripts
# Author:      L.R. McFarland, lrm@starbug.com
# Created:     2014 Jun 19
# install_name now includes CURDIR for rpath error: unsafe use of relative rpath libCoords.1.dylib
# ===============================================================

# assumes googletest is a submodule of Coordinates and built first
GTEST_INC_DIR = ../googletest/googletest/include # TODO default if not set in env?
GTEST_LIB_DIR = ../googletest/googlemock/gtest # TODO default if not set in env?

# Detect operating system flavor.
UNAME   := $(shell uname)

# ----------------
# ----- OS X -----
# ----------------

# .dylib is only for Darwin
ifeq ($(UNAME), Darwin)

CXX      = clang++
CXXFLAGS = -g -W -Wall -fPIC -I. -std=c++11
LINK     = clang++

LDFLAGS  = -L. -lCoords

GTEST_LIBS = -L$(GTEST_LIB_DIR) -lgtest
GTEST_FLAGS = -std=c++11 -I$(GTEST_INC_DIR) -I. -g -c

DYLFLAGS  = -headerpad_max_install_names -single_module -dynamiclib -compatibility_version 1.0 -current_version 1.0.0 -install_name $(CURDIR)/libCoords.1.dylib

TARGET_D = libCoords.1.0.0.dylib
TARGET_D0 = libCoords.dylib
TARGET_D1 = libCoords.1.dylib
TARGET_D2 = libCoords.1.0.dylib

endif

# -----------------
# ----- Linux -----
# -----------------

ifeq ($(UNAME), Linux)

CXX      = g++
CXXFLAGS = -g -W -Wall -fPIC -I. -D BOOST_REGEX
LINK     = g++
LDFLAGS  = -L. -lCoords -lboost_regex

GTEST_LIBS = -lgtest -lpthread
GTEST_FLAGS = -std=c++11 -I. -g -c

endif

# -----------------
# ----- build -----
# -----------------

RM       = rm -f
LN       = ln -s
AR       = ar cq
RANLIB   = ranlib

# targets

INCLUDES = angle.h Cartesian.h datetime.h spherical.h utils.h
SOURCES = angle.cpp Cartesian.cpp datetime.cpp spherical.cpp utils.cpp
OBJECTS = angle.o Cartesian.o datetime.o spherical.o utils.o

TARGET_A = libCoords.a

# builds

all: staticlib $(TARGET_D)

staticlib: $(TARGET_A)

$(TARGET_A): $(OBJECTS) $(INCLUDES) $(SOURCES)
	-$(RM) $(TARGET_A)
	$(AR) $(TARGET_A) $(OBJECTS)
	$(RANLIB) $(TARGET_A)

$(TARGET_D): $(OBJECTS) $(INCLUDES) $(SOURCES)
	-$(RM) $(TARGET_D) $(TARGET_D0) $(TARGET_D1) $(TARGET_D2)
	$(LINK) $(DYLFLAGS) -o $(TARGET_D) $(OBJECTS)
	-$(LN) $(TARGET_D) $(TARGET_D0)
	-$(LN) $(TARGET_D) $(TARGET_D1)
	-$(LN) $(TARGET_D) $(TARGET_D2)


test: angle_unittest Cartesian_unittest datetime_unittest spherical_unittest
	./angle_unittest.sh
	./Cartesian_unittest.sh
	./datetime_unittest.sh
	./spherical_unittest.sh


angle_unittest: angle_unittest.o $(TARGET_A) $(TARGET_D)
	$(CXX) angle_unittest.o -o angle_unittest $(LDFLAGS) $(GTEST_LIBS)

# TODO: make rule for this?
angle_unittest.o: angle_unittest.cpp
	$(CXX) $(GTEST_FLAGS) angle_unittest.cpp


Cartesian_unittest: Cartesian_unittest.o $(TARGET_A) $(TARGET_D)
	$(CXX) Cartesian_unittest.o -o Cartesian_unittest $(LDFLAGS) $(GTEST_LIBS)

Cartesian_unittest.o: Cartesian_unittest.cpp
	$(CXX) $(GTEST_FLAGS) Cartesian_unittest.cpp


datetime_unittest: datetime_unittest.o $(TARGET_A) $(TARGET_D)
	$(CXX) datetime_unittest.o -o datetime_unittest $(LDFLAGS) $(GTEST_LIBS)

datetime_unittest.o: datetime_unittest.cpp
	$(CXX) $(GTEST_FLAGS) datetime_unittest.cpp


spherical_unittest: spherical_unittest.o $(TARGET_A) $(TARGET_D)
	$(CXX) spherical_unittest.o -o spherical_unittest $(LDFLAGS) $(GTEST_LIBS)

spherical_unittest.o: spherical_unittest.cpp
	$(CXX) $(GTEST_FLAGS) spherical_unittest.cpp


example1: example1.o $(TARGET_A) $(TARGET_D)
	$(CXX) example1.o -o example1 $(LDFLAGS)


mepsilon: mepsilon.c
	gcc mepsilon.c -o mepsilon


regex_test: regex_test.o
	$(CXX) regex_test.o -o regex_test $(LDFLAGS)


clean:
	-$(RM) angle_unittest
	-$(RM) angle_unittest.o
	-$(RM) Cartesian_unittest
	-$(RM) Cartesian_unittest.o
	-$(RM) datetime_unittest
	-$(RM) datetime_unittest.o
	-$(RM) spherical_unittest
	-$(RM) spherical_unittest.o
	-$(RM) mepsilon
	-$(RM) mepsilon.o
	-$(RM) regex_test
	-$(RM) regex_test.o
	-$(RM) example1
	-$(RM) example1.o
	-$(RM) $(OBJECTS)
	-$(RM) $(TARGET_D0) $(TARGET_D1) $(TARGET_D2)
	-$(RM) $(TARGET_D)
	-$(RM) $(TARGET_A)


clobber: clean
	-$(RM) *~

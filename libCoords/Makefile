# ===============================================================
# Filename:    Makefile
# Description: makes libCoords and runs test scripts
# Author:      L.R. McFarland, lrm@starbug.com
# Created:     2014 Jun 19
# ===============================================================

# where to find google's gtest code
# built as in README or TOOD: build gtest with "./configure; make" on OS X and Linux?

GTEST_DIR = /usr/local/gtest-1.7.0

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

GTEST_LIBS = -L$(GTEST_DIR) -lgtest

DYLFLAGS  = -headerpad_max_install_names -single_module -dynamiclib -compatibility_version 1.0 -current_version 1.0.0 -install_name libCoords.1.dylib

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
CXXFLAGS = -g -W -Wall -fPIC -I. -std=c++11
LINK     = g++

GTEST_LIBS = -L$(GTEST_DIR) -lgtest -lpthread

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
	$(CXX) angle_unittest.o -L. -lCoords $(GTEST_LIBS) -o angle_unittest

angle_unittest.o: angle_unittest.cpp
	$(CXX) -I$(GTEST_DIR)/include -I. -g -c angle_unittest.cpp

Cartesian_unittest: Cartesian_unittest.o $(TARGET_A) $(TARGET_D)
	$(CXX) Cartesian_unittest.o -L. -lCoords $(GTEST_LIBS) -o Cartesian_unittest

Cartesian_unittest.o: Cartesian_unittest.cpp
	$(CXX) -std=c++11 -I$(GTEST_DIR)/include -I. -g -c Cartesian_unittest.cpp

datetime_unittest: datetime_unittest.o $(TARGET_A) $(TARGET_D)
	$(CXX) datetime_unittest.o -L. -lCoords $(GTEST_LIBS) -o datetime_unittest

datetime_unittest.o: datetime_unittest.cpp
	$(CXX) -std=c++11 -I$(GTEST_DIR)/include -I. -g -c datetime_unittest.cpp

spherical_unittest: spherical_unittest.o $(TARGET_A) $(TARGET_D)
	$(CXX) spherical_unittest.o -L. -lCoords $(GTEST_LIBS) -o spherical_unittest

spherical_unittest.o: spherical_unittest.cpp
	$(CXX) -std=c++11 -I$(GTEST_DIR)/include -I . -g -c spherical_unittest.cpp


example1: example1.o $(TARGET_A) $(TARGET_D)
	$(CXX) example1.o -o example1 -L. -lCoords

mepsilon: mepsilon.c
	gcc mepsilon.c -o mepsilon

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
	-$(RM) example1
	-$(RM) example1.o
	-$(RM) $(OBJECTS)
	-$(RM) $(TARGET_D0) $(TARGET_D1) $(TARGET_D2)
	-$(RM) $(TARGET_D)
	-$(RM) $(TARGET_A)


clobber: clean
	-$(RM) *~

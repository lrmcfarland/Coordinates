# ===============================================================
# Filename:    Makefile
# Description: makes libAngles.so and test scripts
# Author:      L.R. McFarland, lrm@starbug.com
# Created:     2014 Jun 19
#
#              optional debugging (-g) and optimize for speed (-O)
#              linux support (the following is OS X/Darwin specific):
#                  -dynamiclib
#                  -fpic (position independent code) vs. -fPIC
#
# ===============================================================

# where to find google's gtest code
GTEST_DIR = /usr/local/gtest-1.7.0

# programs and flags
CXX      = g++
CXXFLAGS = -g -W -Wall -fPIC -I.

LINK     = g++
DYLFLAGS  = -headerpad_max_install_names -single_module -dynamiclib -compatibility_version 1.0 -current_version 1.0.0 -install_name libCoords.1.dylib

RM       = rm -f
LN       = ln -s
AR       = ar cq
RANLIB   = ranlib -s

# targets

INCLUDES = angle.h Cartesian.h spherical.h utils.h
SOURCES = angle.cpp Cartesian.cpp spherical.cpp utils.cpp
OBJECTS = angle.o Cartesian.o spherical.o utils.o

TARGET_A = libCoords.a

# Detect operating system flavor.
UNAME   := $(shell uname)

# .dylib is only for Darwin
ifeq ($(UNAME), Darwin)
TARGET_D = libCoords.1.0.0.dylib
TARGET_D0 = libCoords.dylib
TARGET_D1 = libCoords.1.dylib
TARGET_D2 = libCoords.1.0.dylib
endif

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


test: angle_unittest Cartesian_unittest spherical_unittest
	./angle_unittest.sh
	./Cartesian_unittest.sh
	./spherical_unittest.sh


angle_unittest: angle_unittest.o $(TARGET_D)
	g++ -L$(GTEST_DIR) -lgtest -L. -lCoords angle_unittest.o -o angle_unittest

angle_unittest.o: angle_unittest.cpp
	g++ -I$(GTEST_DIR)/include -I . -g -c angle_unittest.cpp

Cartesian_unittest: Cartesian_unittest.o $(TARGET_D)
	g++ -L$(GTEST_DIR) -lgtest -L. -lCoords Cartesian_unittest.o -o Cartesian_unittest

Cartesian_unittest.o: Cartesian_unittest.cpp
	g++ -I$(GTEST_DIR)/include -I . -g -c Cartesian_unittest.cpp

spherical_unittest: spherical_unittest.o $(TARGET_D)
	g++ -L$(GTEST_DIR) -lgtest -L. -lCoords spherical_unittest.o -o spherical_unittest

spherical_unittest.o: spherical_unittest.cpp
	g++ -I$(GTEST_DIR)/include -I . -g -c spherical_unittest.cpp


example1: example1.o $(TARGET_D)
	g++ example1.o -o example1 -L. -lCoords

mepsilon: mepsilon.c
	gcc mepsilon.c -o mepsilon

clean:
	-$(RM) angle_unittest
	-$(RM) angle_unittest.o
	-$(RM) Cartesian_unittest
	-$(RM) Cartesian_unittest.o
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

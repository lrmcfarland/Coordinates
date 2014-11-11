# Coordinates

## Features

This repo contains C++ classes with Python wrappers of
[Cartesian](http://en.wikipedia.org/wiki/Cartesian_coordinate_system)
and
[Spherical](http://en.wikipedia.org/wiki/Spherical_coordinate_system)
coordinate systems for use in physics applications. The objects are
[overloaded](http://en.wikipedia.org/wiki/Operator_overloading) to
support the basic arithmetic operators of + and -. Multiplying or
dividing by a double will scale the vector. Constructors for copy
and copy assign are provided along with conversion constructors
to transform Cartesian into Spherical coordinates and vice versa.

## Build

Each directory has its own Makefile with 'build', 'test', and 'clean'
targets, e.g.

```
$ pwd
/Users/.../Coordinates/libCoords
$ make clean
$ make test
```

libCoords must be built first. There is a build.sh script that runs
the Makefiles in all the directories in the necessary order and takes
the same make targets as above.

```
$ pwd
/Users/.../Coords/
$ ./build.sh clean
$ ./build.sh test
```

Each directory also has a suite of unit tests. libCoords uses
[gtest](https://code.google.com/p/googletest/). The Python
modules use Python native unittest.

To build the Boost wrappers you will, of course, need to install
[Boost](http://www.boost.org).

### [googletest](https://code.google.com/p/googletest/)

The C++ library uses [googletest](https://code.google.com/p/googletest/) to
run the unit tests. I have downloaded and built it in /usr/local by
following the instructions in the README

```
[root gtest-1.7.0]# export GTEST_DIR=/usr/local/gtest-1.7.0
[root gtest-1.7.0]# g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc

[root gtest-1.7.0]# ar -rv libgtest.a gtest-all.o
ar: creating archive libgtest.a
a - gtest-all.o
```

libCoords/Makefile sets its GTEST_DIR to /usr/local/gtest-1.7.0 and picks
up libgtest.a from there.


### [Boost](http://www.boost.org)

I have been wanting to use [homebrew](http://brew.sh) to install
boost, but some reason, I find it does not yet install
libboost_python.a by default or even with the --with-python and/or
--build-from-source options. So I built and installed it from the
source I downloaded from
[here](http://www.boost.org/users/history/version_1_56_0.html).

```
cd boost_1_56_0

./bootstrap.sh
./b2
sudo ./b2 install
```

The boost files are now in /usr/local/include/boost and
/usr/local/lib/libboost_*.  python/Boost/setup.py sets BOOST_ROOT to
point there.  brew doctor will notice and complain about this.


### OSX

I built this on my iMac using the g++ compiler that comes with
[Xcode](https://developer.apple.com/xcode/), but there is nothing special
that should cause a problem for other compilers.

### Linux

TODO

## Use

### C++

TODO

### Python

TODO


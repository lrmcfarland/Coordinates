# Coordinates

This repo holds my Coordinates library for implementing physics and astronomy applications.
It is implemented in C++ and has Python wrappers generated with
[Boost.python](https://wiki.python.org/moin/boost.python/GettingStarted),
[SWIG](http://www.swig.org/)
and
[extensions using just
Python](https://docs.python.org/3/extending/extending.html).
The Python extension makes this simple to incorporate into a python
web framework like [flask](http://flask.pocoo.org/) (See
[AAI](https://aai.starbug.com/)).
All objects are in the name space "Coords", e.g. Coords::Cartesian.


# To Install

I have pushed a python3 version of the library to the pypi repo

## Using pyenv

To use [pyenv](https://github.com/pyenv/pyenv) as a virtualenv

```
$ pyenv virtualenv 3.7.0 test-coords

$ pyenv activate test-coords

(test-coords) $ pip install --upgrade pip

(test-coords) $ pip install --index-url https://test.pypi.org/sample starbug.coords
```

TODO example wiht python3 -m

## Running an example

You should now be able to import the coords module and run this example

```
(test-coords) $ python
Python 3.7.0 (default, Jul  1 2018, 12:43:10)
[Clang 9.1.0 (clang-902.0.39.2)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> import starbug.coords as coords

>>> keplers = coords.spherical(6371, coords.angle(90) - coords.angle(37, 27, 13), coords.angle(-122, 10, 55))
>>> booksinc = coords.spherical(6371, coords.angle(90) - coords.angle(37, 23, 32.4852), coords.angle(-122, 4, 46.2252))
>>> str(keplers - booksinc)
'<spherical><r>11.3235</r><theta>61.4649</theta><phi>123.282</phi></spherical>'
```


# To Build

Clone the repo from here. This has googletest as a sub-module to
build the unit tests, but it is not necessary to build just the coords
library.
The [build.sh](build.sh) script will run

```
    git submodule update --init --recursive
```

## Build tools

### cmake

googletest will also need cmake installed

#### On OS X

```
    brew install cmake
```

### Boost

Boost is needed to build the Python Boost wrappers.

#### On OS X

```
    brew install boost --with-python
    brew install boost-python
    brew install boost-python3
```

### swig

#### On OS X

```
    brew install swig
```


# build.sh

The top level [build.sh](build.sh) script will build all libraries.
libCoords must be built first.
Python/Boost and Python/Manual are built next.
It will pass targets through to the Makefiles, like all (a.k.a. the
default), clean and test.

```
    $ pwd
    ~/Coordinates/
    $ ./build.sh clean
    $ ./build.sh test
```

Each directory has its own Makefile with 'build', 'test', and 'clean'
targets. build.sh simply runs the Makefile in each of the sub directories.

```
    $ pwd
    ~/Coordinates/libCoords
    $ make clean
    $ make test
```

Each part can be built by going into each directory in turn and
running the make command, but libCoords needs to be the first.
Details on this can be found in [libCoords](libCoords/README.md).
libCoords and the "manual" python extension can be built with
out additional packages.
A complete build of Coordinates uses gtest and Boost.

To build the Boost wrappers you will, of course, need to install
[Boost](http://www.boost.org) with python. Details on this can be found
in [Python/Boost](Python/Boost/README.md).


## OS X

I built this on my iMac using the [LLVM](http://llvm.org) compiler
that comes with [Xcode](https://developer.apple.com/xcode/).
After installing Xcode from Apple, I used [homebrew](http://brew.sh)
to install Boost.

## Linux

TODO 2015: CentOS and other Linux releases are using GCC 4.8
which has the prototype std::regex that throws regex_error on
my std::regex tests.
I experimented with upgrading to GCC 4.9, but had linking problems later on.
To work around this I use the Boost regex libraries.

# To Run

## C++

[example1.cpp](libCoords/example1.cpp) demonstrates the C++ interface.
Here I use the spherical coordinates with the radius of the earth (in
kilometers), the latitude (which needs to be converted to the theta
angle from the positive z axis), and longitude of two locations to
calculate the distance between them.

```

// ============================================================
// Filename:    example1.cpp
//
// Description: Example of using libCoords to calculate the distance
//              between two places on earth given the latitude and
//              longitude.
//
// Authors:     L.R. McFarland
// Created:     2014nov11
// ============================================================

#include <iostream>

#include <angle.h>
#include <Cartesian.h>
#include <spherical.h>

int main () {

  const double Re(6371); // radius of earth in km

  Coords::spherical keplers(Re, Coords::Latitude(37, 27, 13), Coords::angle(-122, 10, 55));
  Coords::spherical booksinc(Re, Coords::Latitude(37, 23, 32.4852), Coords::angle(-122, 4, 46.2252));

  Coords::Cartesian delta(keplers - booksinc);

  std::cout << "keplers " << keplers << " - " << std::endl
	    << "books inc " << booksinc << " = " << std::endl
	    << "distance " << delta.magnitude() << " km" << std::endl;

  return 0;
}


```

```

[libCoords (master)]$ make example1
g++ -g -W -Wall -fPIC -I.   -c -o example1.o example1.cpp
g++ example1.o -o example1 -L. -lCoords

[libCoords]$ ./example1
keplers <spherical><r>6371</r><theta>52.5464</theta><phi>-122.182</phi></spherical> -
books inc <spherical><r>6371</r><theta>52.6076</theta><phi>-122.08</phi></spherical> =
distance 11.3235 km

```


## Interactive Python

This example uses pylaunch.sh to set the library path to use the local library
and not import one from PYPI.



```
$ ./pylaunch.sh
# coords.so: ./build/lib.macosx-10.13-x86_64-3.7/coords.cpython-37m-darwin.so
# PYTHONPATH :./build/lib.macosx-10.13-x86_64-3.7
Python 3.7.0 (default, Jul  1 2018, 12:43:10)
[Clang 9.1.0 (clang-902.0.39.2)] on darwin
Type "help", "copyright", "credits" or "license" for more information.

>>> import coords

>>> keplers = coords.spherical(6371, coords.angle(90) - coords.angle(37, 27, 13), coords.angle(-122, 10, 55))
>>> booksinc = coords.spherical(6371, coords.angle(90) - coords.angle(37, 23, 32.4852), coords.angle(-122, 4, 46.2252))

>>> print keplers - booksinc
<spherical><r>11.3235</r><theta>61.4649</theta><phi>123.282</phi></spherical>

```

# Python Boost wrappers


## To Build

The build is done using make on the command line. There are targets
for build and test. See the Makefile for details. It will not build
../../libCoords. That must be done before this.

setup.py expects to find libCoords.dylib in ../../libCoords

coords.cpp contains the Boost wrappers for the three coordinate
classes, angle, Cartesian and spherical.

### [Boost](http://www.boost.org)

The Boost Python library must be installed before this build will
work.

I have been wanting to use [homebrew](http://brew.sh) to install
boost, but some reason (not using Boost Python?), I find it does not
yet install libboost_python.a by default or even with the
--with-python and/or --build-from-source options. So I built and
installed it from the source I downloaded from
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


## The Test Environment

The runtime environment expects ../../libCoords to be built.

On OS X, this needs to be on the DYLD_LIBRARY_PATH.
The PYTHONPATH must contain the boost coords.so.

setenv.sh will use COORDS_ROOT to find these or assume they are
relative to this directory.

pylaunch.sh will use setenv.sh to bring up a Python interpreter.

test_coords.sh will run the angle, Cartesian and spherical unit tests.


### To test a component

```

$ ./pylaunch.sh test_angle.py -v
...

$ ./pylaunch.sh test_Cartesian.py -v
...

$ ./pylaunch.sh test_spherical.py -v
../

```

### To test all components

```

$ ./test_coords.sh -v

```

## The Runtime Environment

You can use pylaunch.sh to set up an interactive session.

Here I use the spherical coordinates with the radius of the earth, the
latitude (which needs to be converted to the theta angle from the
positive z axis), and longitude of two locations to calculate the
distance between them.

```

$ ./pylaunch.sh 
# COORDS_ROOT not set. Using ../..
# coords.so: ../../python/Boost/build/lib.macosx-10.9-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../../libCoords
# PYTHONPATH :../../python/Boost/build/lib.macosx-10.9-intel-2.7

Python 2.7.5 (default, Mar  9 2014, 22:15:05) 
[GCC 4.2.1 Compatible Apple LLVM 5.0 (clang-500.0.68)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> import coords
>>> keplers = coords.spherical(6371, coords.angle(90) - coords.angle(37, 27, 13), coords.angle(-122, 10, 55))
>>> booksinc = coords.spherical(6371, coords.angle(90) - coords.angle(37, 23, 32.4852), coords.angle(-122, 4, 46.2252))
>>> print keplers - booksinc
<spherical><r>11.3235</r><theta>61.4649</theta><phi>123.282</phi></spherical>

```
# Python Boost wrappers

This directory contains the Boost generated Python wrappers for the C++
classes in ../../libCoords. coords.cpp contains the Boost macros that
are used to generate the wrappers. setup.py builds them.

There are some differences from the [Manual](../Manual) version that
I am looking into as I learn more about Boost.


## To Build

The build is done using make on the command line. There are targets
for clean and test. See the Makefile for details.

This Makefile will not build ../../libCoords. That must be done before this.

setup.py expects to find libCoords.dylib in ../../libCoords and Boost
to be in /usr/local. See setup.py for details.


### [Boost](http://www.boost.org)

The Boost Python library must be installed before this build will
work.



#### On OS X with brew

The easiest way I found to install and maintain boost on OS X is to
use [homebrew](http://brew.sh). Open the boost-python bottle. The
python wrappers are not in the "regular" boost bottle.

```

$ brew install boost-python
==> Installing boost-python dependency: boost
==> Downloading https://downloads.sf.net/project/machomebrew/Bottles/boost-1.56.0.mavericks.bottle.1.tar.gz
######################################################################## 100.0%
==> Pouring boost-1.56.0.mavericks.bottle.1.tar.gz
🍺  /usr/local/Cellar/boost/1.56.0: 10471 files, 438M
==> Installing boost-python
==> Downloading https://downloads.sourceforge.net/project/boost/boost/1.56.0/boost_1_56_0.tar.bz2
######################################################################## 100.0%
==> ./bootstrap.sh --prefix=/usr/local/Cellar/boost-python/1.56.0 --libdir=/usr/local/Cellar/boost-python/1.56.0/lib --with-librar
==> ./b2 --build-dir=build-python --stagedir=stage-python python=2.7 --prefix=/usr/local/Cellar/boost-python/1.56.0 --libdir=/usr/
🍺  /usr/local/Cellar/boost-python/1.56.0: 5 files, 11M, built in 3.1 minutes


```

#### On Ubuntu with apt-get


```
sudo apt-get install libboost-all-dev
```


#### from source

To built and install from the
[boost](http://www.boost.org/users/history/version_1_56_0.html)
source, unpack the tar-ball in /usr/local

```

cd /usr/local/boost_1_56_0

./bootstrap.sh
./b2
sudo ./b2 install
```

The boost files are now in /usr/local/include/boost and
/usr/local/lib/libboost_*.  python/Boost/setup.py sets BOOST_ROOT to
point there. On OS X brew doctor will notice and complain about this.


## The Test Environment

The runtime environment expects ../../libCoords to be built.

On OS X, this needs to be on the DYLD_LIBRARY_PATH.
On Linux, this needs to be on the LD_LIBRARY_PATH.

The PYTHONPATH must contain the boost coords.so.

setenv.sh will detect the platform using uname and set the appropriate
library path and use COORDS_ORIGIN environment variable to find
coords.so. If COORDS_ORIGIN is not set, it will assume it is relative
to this directory.

pylaunch.sh will use setenv.sh to bring up a Python interpreter in
this environment.

test_coords.sh will run the angle, Cartesian, datetime and spherical
unit tests.


### To test a component

Use pylaunch.sh to call setenv.sh. Select one of the associated python
unittest scripts, e.g. test_Cartesian.py, along with the usual command
line options like -v

```

$ ./pylaunch.sh test_angle.py -v

# COORDS_ROOT not set. Using ../..
# coords.so: ../../Python/Boost/build/lib.macosx-10.9-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../../libCoords
# PYTHONPATH :../../Python/Boost/build/lib.macosx-10.9-intel-2.7

test_accessors_1 (__main__.TestAngle)
Test value accessors 1 ... ok
test_accessors_1b (__main__.TestAngle)
Test value accessor exception ... skipped 'TODO Boost.Python.ArgumentError'
test_accessors_2 (__main__.TestAngle)
Test radians accessor negative ... ok
test_accessors_2a (__main__.TestAngle)
Test radians accessor exception ... skipped 'TODO Boost.Python.ArgumentError'
test_angle1_eq_angle1 (__main__.TestAngle)
Test richcompare operator==() ... ok
...
Test latitude += latitude ... ok
test_inplace_add_2 (__main__.TestLatitude)
Test latitude += angle ... ok
test_inplace_add_over_limit (__main__.TestLatitude)
Test latitude += latitude ... ok

----------------------------------------------------------------------
Ran 49 tests in 0.003s

OK (skipped=5)


```

### To run a single unit test

From the command line, pass in the UnitTest test harness and test name,
e.g. TestCartesian, test_default_constructor to the test script

```

$ ./pylaunch.sh test_Cartesian.py -v TestCartesian.test_default_constructor

# COORDS_ROOT not set. Using ../..
# coords.so: ../../Python/Boost/build/lib.macosx-10.9-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../../libCoords
# PYTHONPATH :../../Python/Boost/build/lib.macosx-10.9-intel-2.7

test_default_constructor (__main__.TestCartesian)
Test default constructor ... ok

----------------------------------------------------------------------
Ran 1 test in 0.000s

OK

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
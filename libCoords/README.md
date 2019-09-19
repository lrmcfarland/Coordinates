# Coordinates library

I created this library to support my projects in physics and astronomy.
I use C++ to implement classes for coordinates, Cartesian and spherical,
and date-times (customized for astronomical algorithms, e.g. Julian and
J2000 conventions).
I make use of C++'s support for creating conversion constructors to
make it easy to switch between Cartesian and spherical coordinate
systems as well as overloading methods like add and subtract.

Although I did overload the Cartesian multiply to be the vector dot
operation but, it is probably for the best I can not overload the
character "x" to be the cross product.
However there are dot() and cross() methods defined.

And I found overloading the output operator to produce XML (TODO JSON)
parse-able strings for constructors provided a powerful way to
serialize compound objects with the natural recursion of
each sub object knowing how to print itself.

The result is syntax that makes physics equations like

```
    F = m*a;

```

work correctly where <i>F</i> and <i>a</i> are vectors and <i>m</i> a scalar.
It also [commutes](https://en.wikipedia.org/wiki/Commutative_property).

All of these features are verified with unit testing.


# To install

Clone the repo from here. googletest is a sub-module so you will
also need to get that if you want to build the tests, but it
is not necessary to build just the coords library.

```
    git submodule update --init --recursive

```

The
[build.sh](https://github.com/lrmcfarland/Coordinates/blob/update-readmes/build.sh)
script will also do this for you.


# To Build

I support Linux and OS X builds with the
[Makefile](https://github.com/lrmcfarland/Coordinates/blob/update-readmes/libCoords/Makefile).
The basic targets are all, clean and test.  There are some blind spots
in the dependency chain, so I use make clean often to be sure
everything is rebuilt.


```
    $ cd libCoords/
    [libCoords]$ make clean
    ...
    [libCoords]$ make
    ...
    [libCoords]$ make test
    ...
```

## gtest

This uses the [googletest](https://github.com/google/googletest)
framework for the c++ unittests.  It is included as a git submodule
and built by the build.sh script. It is only needed to run the c++
unit tests in libCoords. The python unit tests will implicitly run
most of the same tests.


# To run

The
[setenv.sh](https://github.com/lrmcfarland/Coordinates/blob/update-readmes/libCoords/setenv.sh)
script will add the built coordinates library to your LD_LIBRARY_PATH
(Linux) or DYLD_LIBRARY_PATH (OS X) as appropriate.
It will use the default build location or the COORDS_ROOT environment
variable if it is set.


# To test


On the command line, use the shell wrapper to set up the environment
(setenv.sh) and run the tests. There is a wrapper script for each
of the four components: Cartesian, angle, date-time, spherical.

## To run the spherical unit tests

```
    $ ./spherical_unittest.sh

    # COORD_ORIGIN not set. Using ..
    # GTEST_DIR not set. Using /usr/local/gtest-1.7.0
    # DYLD_LIBRARY_PATH :../libCoords:/usr/local/gtest-1.7.0

    [==========] Running 43 tests from 2 test cases.
    [----------] Global test environment set-up.
    [----------] 42 tests from FixedSpherical
    [ RUN      ] FixedSpherical.Accessors
    [       OK ] FixedSpherical.Accessors (0 ms)
    [ RUN      ] FixedSpherical.Equivalence
    [       OK ] FixedSpherical.Equivalence (0 ms)
    ...

    [----------] Global test environment tear-down
    [==========] 43 tests from 2 test cases ran. (1 ms total)
    [  PASSED  ] 43 tests.

```

To run a single test, use the --gtest_filter option to pass the test
name through the shell wrapper

```
    $ ./Cartesian_unittest.sh --gtest_filter=FixedCartesian.ConstructFromString

    # COORD_ORIGIN not set. Using ..
    # GTEST_DIR not set. Using /usr/local/gtest-1.7.0
    # DYLD_LIBRARY_PATH :../libCoords:/usr/local/gtest-1.7.0

    Note: Google Test filter = FixedCartesian.ConstructFromString
    [==========] Running 1 test from 1 test case.
    [----------] Global test environment set-up.
    [----------] 1 test from FixedCartesian
    [ RUN      ] FixedCartesian.ConstructFromString
    [       OK ] FixedCartesian.ConstructFromString (0 ms)
    [----------] 1 test from FixedCartesian (0 ms total)

    [----------] Global test environment tear-down
    [==========] 1 test from 1 test case ran. (0 ms total)
    [  PASSED  ] 1 test.

```

# To debug

With gdb (or lldb on OS X) using the gtest filter, use the
full method name (in this example Coords::rotator::rotate) to set
the break point and --gtest_filter with the GTEST name
(XRotationTest.Positive90AboutZ) to run that test.

```
    [libCoords (spherical_atan2)]$ lldb Cartesian_unittest

    (lldb) target create "Cartesian_unittest"
    Current executable set to 'Cartesian_unittest' (x86_64).

    (lldb) b Coords::rotator::rotate
    Breakpoint 1: where = libCoords.1.dylib`Coords::rotator::rotate(Coords::Cartesian const&, Coords::angle const&) + 42 at Cartesian.cpp:235, address = 0x0000000000003f7a

    (lldb) run --gtest_filter=RotationTest.FirstDiagonal_1
    Process 31270 launched: '~/Coordinates/libCoords/Cartesian_unittest' (x86_64)
    Note: Google Test filter = RotationTest.FirstDiagonal_1
    [==========] Running 1 test from 1 test case.
    [----------] Global test environment set-up.
    [----------] 1 test from RotationTest
    [ RUN      ] RotationTest.FirstDiagonal_1
    Process 31270 stopped
    * thread #1: tid = 0x212ea0, 0x000000010013df7a libCoords.1.dylib`Coords::rotator::rotate(this=0x00007fff5fbff348, a_heading=0x00007fff5fbff330, an_angle=0x00007fff5fbff3e8) + 42 at Cartesian.cpp:235, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
	frame #0: 0x000000010013df7a libCoords.1.dylib`Coords::rotator::rotate(this=0x00007fff5fbff348, a_heading=0x00007fff5fbff330, an_angle=0x00007fff5fbff3e8) + 42 at Cartesian.cpp:235
       232						  const Coords::angle& an_angle) {
       233
       234	  // Quaternion-derived rotation matrix
    -> 235	  if (m_is_new_axis || m_current_angle != an_angle) {
       236
       237		double c(cos(an_angle.radians()));
       238		double s(sin(an_angle.radians()));

    (lldb) p a_heading
    (const Coords::Cartesian) $0 = (m_x = -1, m_y = -1, m_z = 1)

    (lldb) c

    Process 31270 resuming
    [       OK ] RotationTest.FirstDiagonal_1 (38877 ms)
    [----------] 1 test from RotationTest (38877 ms total)

    [----------] Global test environment tear-down
    [==========] 1 test from 1 test case ran. (38877 ms total)
    [  PASSED  ] 1 test.
    Process 31270 exited with status = 0 (0x00000000)

    (lldb) ^D

```

Setting break points

```


$ lldb datetime_unittest


(lldb) run --gtest_filter=DateTime.set_timezone_zulu_1

(lldb) b Coords::DateTime::toModifiedJulianDateAPC

(lldb) run --gtest_filter=DateTime.set_timezone_zulu_1

(lldb) b datetime_unittest.cpp:2411


```

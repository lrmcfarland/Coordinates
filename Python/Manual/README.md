# Python manually extended Python wrappers

This directory contains my "manually" [extended Python
wrappers](http://docs.python.org/extending/newtypes.html) for the C++
classes in ../../libCoords. coords.cpp contains the [Python/C
API](http://docs.python.org/c-api) calls that make up the
Python wrappers. setup.py builds them.

There are some differences from the [Boost](../Boost) version,
like not having a coords.Error exception but using RuntimeError
instead. The static unit vectors have also moved from coords.Ux
to coords.Cartesian.Ux. Similarly for Uy, Uz, and Uo.

## To Build

The build is done using make on the command line. There are targets
for clean and test. See the [Makefile](Makefile) for details.

This Makefile will not build ../../libCoords. That must be done before this.

setup.py expects to find libCoords.dylib in ../../libCoords


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
# coords.so: ../../Python/Manual/build/lib.macosx-10.9-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../../libCoords
# PYTHONPATH :../../Python/Manual/build/lib.macosx-10.9-intel-2.7

test_accessors_1 (__main__.TestAngle)
Test value accessors 1 ... ok
test_accessors_1b (__main__.TestAngle)
Test value accessor exception ... ok
test_accessors_2 (__main__.TestAngle)
Test radians accessor negative ... ok
test_accessors_2a (__main__.TestAngle)
Test radians accessor exception ... ok
test_angle1_eq_angle1 (__main__.TestAngle)
Test richcompare operator==() ... ok
...
test_inplace_plus_over_limit (__main__.TestLatitude)
Test latitude += latitude ... ok

----------------------------------------------------------------------
Ran 73 tests in 0.005s

OK (skipped=1)


```


### To run a single unit test

From the command line, pass in the UnitTest test harness and test name,
e.g. TestCartesian, test_default_constructor to the test script

```

$ ./pylaunch.sh test_Cartesian.py -v TestCartesian.test_default_constructor

# COORDS_ROOT not set. Using ../..
# coords.so: ../../Python/Manual/build/lib.macosx-10.9-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../../libCoords
# PYTHONPATH :../../Python/Manual/build/lib.macosx-10.9-intel-2.7

test_default_constructor (__main__.TestCartesian)
Test default constructor ... ok

----------------------------------------------------------------------
Ran 1 test in 0.001s

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

## Troubleshooting

I was getting a segmentation violation when running on python3 that
was not happening in python 2.7.  The problem was not using * magic

```
   -        self.assertRaises(coords.Error, lambda a: coords.datetime(a), (1962, 25, 66, 14, 30, 25))
   +        self.assertRaises(coords.Error, lambda a: coords.datetime(*a), (1962, 25, 66, 14, 30, 25))

```


The key to finding it was to use the "-X
[faulthandler](https://docs.python.org/3/library/faulthandler.html)"
option

```

(aai.starbug) [lrm@lrmz-iMac Manual (standard-timezone-v5)]$ . ./setenv.sh; python -q -X faulthandler ./test_datetime.py
# coords.so: ./build/lib.macosx-10.13-x86_64-3.7/coords.cpython-37m-darwin.so
# PYTHONPATH :./build/lib.macosx-10.13-x86_64-3.7:./build/lib.macosx-10.13-x86_64-3.7:./build/lib.macosx-10.13-x86_64-3.7:./build/lib.macosx-10.13-x86_64-3.7:./build/lib.macosx-10.13-x86_64-3.7
..................................Fatal Python error: Segmentation fault

Current thread 0x000000010a8395c0 (most recent call first):
  File "./test_datetime.py", line 156 in <lambda>
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/case.py", line 178 in handle
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/case.py", line 743 in assertRaises
  File "./test_datetime.py", line 156 in test_year_month_day_hour_minute_second_constructor_exception
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/case.py", line 615 in run
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/case.py", line 663 in __call__
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/suite.py", line 122 in run
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/suite.py", line 84 in __call__
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/suite.py", line 122 in run
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/suite.py", line 84 in __call__
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/runner.py", line 176 in run
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/main.py", line 271 in runTests
  File "/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/unittest/main.py", line 101 in __init__
  File "./test_datetime.py", line 362 in <module>
Segmentation fault: 11


```

I also used the method described [here](https://stackoverflow.com/questions/2663841/python-tracing-a-segmentation-fault)

Set the trace function in top of test code, test_datetime.py in this example.

```

import sys

def trace(frame, event, arg):
    print "%s, %s:%d" % (event, frame.f_code.co_filename, frame.f_lineno)
    return trace
cd ..
sys.settrace(trace)

```

Output will show

```


OK (skipped=1)
. ./setenv.sh; python ./test_datetime.py 
# coords.so: ./build/lib.macosx-10.14-x86_64-2.7/coords.so
# PYTHONPATH :./build/lib.macosx-10.14-x86_64-2.7:./build/lib.macosx-10.14-x86_64-2.7:./build/lib.macosx-10.14-x86_64-2.7:./build/lib.macosx-10.14-x86_64-2.7
call, ./test_datetime.py:31
line, ./test_datetime.py:31
line, ./test_datetime.py:33
line, ./test_datetime.py:46
line, ./test_datetime.py:53



line, /Users/rmcfarland/.pyenv/versions/2.7.16/lib/python2.7/unittest/case.py:318
line, /Users/rmcfarland/.pyenv/versions/2.7.16/lib/python2.7/unittest/case.py:319
line, /Users/rmcfarland/.pyenv/versions/2.7.16/lib/python2.7/unittest/case.py:320
call, ./test_datetime.py:33
line, ./test_datetime.py:37
line, ./test_datetime.py:39
return, ./test_datetime.py:39
line, /Users/rmcfarland/.pyenv/versions/2.7.16/lib/python2.7/unittest/case.py:328
line, /Users/rmcfarland/.pyenv/versions/2.7.16/lib/python2.7/unittest/case.py:329
call, ./test_datetime.py:229
line, ./test_datetime.py:231
/bin/sh: line 1:  4701 Segmentation fault: 11  python ./test_datetime.py



```


I also turned on printing with in coords.cpp using [this example](https://pythonextensionpatterns.readthedocs.io/en/latest/parsing_arguments.html).
In the code I could print the arg value to standard out. Debugging it old school.


```
ai-py27) [rmcfarland@VSN00249 Manual (polydatetime)]$ git diff
diff --git a/Python/Manual/coords.cpp b/Python/Manual/coords.cpp
index 03341c6..55a4f23 100644
--- a/Python/Manual/coords.cpp
+++ b/Python/Manual/coords.cpp
@@ -3683,7 +3683,26 @@ static int datetime_init(datetime* self, PyObject* args, PyObject* kwds) {
     return -1;
 
 
-  if (arg0) {
+  // TODO rm
+  PyObject_Print(args, stdout, 0); // TODO rm
+  fprintf(stdout, "aaaaaaa \n"); // TODO rm
+
+  if (arg0 == NULL) {
+
+    PyObject_Print(arg0, stdout, 0); // TODO rm
+    fprintf(stdout, "arg0 == NULL \n"); // TODO rm
+
+    // default needed for python 2.7 but not 3
+    Coords::DateTime a_datetime(year, month, day, hour, minute, second, timezone);
+    self->m_datetime = a_datetime;
+    return 0;
+  }
+
+
+  if (arg0 and !arg1) {
+
+    PyObject_Print(arg0, stdout, 0); // TODO rm
+    fprintf(stdout, "arg0 and !arg1 \n"); // TODO rm
 
```

The output looked like this

```

. ./setenv.sh; python ./test_datetime.py 
# coords.so: ./build/lib.macosx-10.14-x86_64-2.7/coords.so
# PYTHONPATH :./build/lib.macosx-10.14-x86_64-2.7:./build/lib.macosx-10.14-x86_64-2.7:./build/lib.macosx-10.14-x86_64-2.7:./build/lib.macosx-10.14-x86_64-2.7
('1962-07-10T07:30:00-05:00',)aaaaaaa 
'1962-07-10T07:30:00-05:00'ccccccc 
('1963-07-10T07:30:00-05:00',)aaaaaaa 
'1963-07-10T07:30:00-05:00'ccccccc 
.('1962-07-10T07:30:00+05:00',)aaaaaaa 
'1962-07-10T07:30:00+05:00'ccccccc 
('1963-07-10T07:30:00+05:00',)aaaaaaa 
'1963-07-10T07:30:00+05:00'ccccccc 
.('1962-07-10T07:30:00-06:00',)aaaaaaa 
'1962-07-10T07:30:00-06:00'ccccccc 
.('1962-07-10T07:30:00+05:00',)aaaaaaa 
'1962-07-10T07:30:00+05:00'ccccccc 
.()aaaaaaa 
<nil>bbbbbb 
.('1900-01-01T00:00:00-08:00',)aaaaaaa 
'1900-01-01T00:00:00-08:00'ccccccc 
.()aaaaaaa 
<nil>bbbbbb 
.('2017-10-05T07:30:00-05:00',)aaaaaaa 
'2017-10-05T07:30:00-05:00'ccccccc 
.('1999-12-31T08:30:00-0800',)aaaaaaa 
'1999-12-31T08:30:00-0800'ccccccc 
.('2019-11-10T07:30:00',)aaaaaaa 
'2019-11-10T07:30:00'ccccccc 
.('2019-11-10T07:30:00+0530',)aaaaaaa 
'2019-11-10T07:30:00+0530'ccccccc 
.(1962,)aaaaaaa 
1962ccccccc 
F(1962, 7, 10, 7, 30, 0, '5:30')aaaaaaa 
F(1962, 7)aaaaaaa 
/bin/sh: line 1: 22305 Segmentation fault: 11  python ./test_datetime.py
make: *** [test_datetime] Error 139


```

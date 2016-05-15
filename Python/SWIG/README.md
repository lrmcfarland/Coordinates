# Swig Python wrappers

This directory contains the [SWIG](http://swig.org) Python wrappers for the
coords objects. SWIG builds the wrappers from the [coords.i](coords.i)
interface definition.

This is proof of concept at this point and not all feature have been wrapped.


This has some differences from the manual and boost wrapped
versions. I have adapted the unit tests to have the same coverage
except where I needed to skip tests because of as yet unresolved
implementation differences.

The [Makefile](Makefile) is setup to wrap and build the _coords.so
library. This can simply be imported directly when the interpreter is
run in this directory or setting the PYTHONPATH otherwise.

```
    $ python
    Python 2.7.10 (default, Oct 23 2015, 19:19:21) 
    [GCC 4.2.1 Compatible Apple LLVM 7.0.0 (clang-700.0.59.5)] on darwin
    Type "help", "copyright", "credits" or "license" for more information.
    >>> import coords
    >>> dir(coords)
    ['Cartesian', 'Cartesian_swigregister', '__builtins__', '__doc__', '__file__', '__name__', '__package__', '_coords', '_newclass', '_object', '_swig_getattr', '_swig_property', '_swig_repr', '_swig_setattr', '_swig_setattr_nondynamic', 'angle', 'angle_swigregister']
    >>> foo = coords.Cartesian(1,2,3)
    >>> print foo
    <Cartesian><x>1</x><y>2</y><z>3</z></Cartesian>
    >>> repr(foo)
    '(1, 2, 3)'
    >>> 
```


# Install

## OS X

```
brew install swig
```



"""Creates python wrappers from boost macro

from http://docs.python.org/extending/building.html

ASSUMES: ../../libCoords exists and has boost installed (in
/usr/local/[include,lib] on OS X and in /usr/lib64 for Linux)

"""

import platform
import sys

from distutils.core import setup, Extension

name = 'coords'
version = '1.0'
description = 'coords package'

if platform.system() == 'Darwin':

    BOOST_ROOT = '/usr/local'
    include_dirs = ['../../libCoords']
    library_dirs = ['../../libCoords']
    libraries = ['Coords']
    sources = ['coords.cpp']

elif platform.system() == 'Linux':
    # TODO more Linux specific, e.g. CentOS vs. Ubuntu?

    include_dirs = ['../../libCoords']
    library_dirs = ['../../libCoords', '/usr/lib64']
    libraries = ['boost_regex', 'Coords']
    sources = ['coords.cpp']

else:
    print('unsupported platform:', platform.system())
    sys.exit(1)


coords_module = Extension(name,
                          include_dirs=include_dirs,
                          libraries=libraries,
                          library_dirs=library_dirs,
                          sources=sources)

setup (name=name,
       version=version,
       description=description,
       ext_modules=[coords_module])


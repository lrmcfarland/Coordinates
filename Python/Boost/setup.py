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

    # brew install boost python and python 3
    if sys.version_info.major == 3:
        boost_pylib = 'boost_python37'
    else:
        boost_pylib = 'boost_python27'

    BOOST_ROOT = '/usr/local'
    include_dirs = ['../../libCoords', BOOST_ROOT + '/include']
    library_dirs = ['../../libCoords', BOOST_ROOT + '/lib']
    libraries = [boost_pylib, 'Coords']
    sources = ['coords.cpp']

elif platform.system() == 'Linux':

    if sys.version_info.major == 3:
        boost_pylib = 'boost_python3' # TODO test this
    else:
        boost_pylib = 'boost_python'

    include_dirs = ['../../libCoords']
    library_dirs = ['../../libCoords', '/usr/lib64']
    libraries = [boost_pylib, 'boost_regex', 'Coords']
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

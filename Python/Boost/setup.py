"""Creates python wrappers from boost macro

from http://docs.python.org/extending/building.html

ASSUMES: ../../libCoords exists and has boost installed (in
/usr/local/[include,lib] on OS X and in /usr/lib64 for Linux)

"""

import platform
import setuptools
import sys

from distutils.core import setup, Extension

name = 'starbug.coords'
version = '1.0.0'
description = 'coords package'

with open("README.md", "r") as fh:
    long_description = fh.read()

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

setuptools.setup (name=name,
                  version=version,
                  author="Lincoln Randall McFarland",
                  author_email="lrm@starbug.com",
                  description=description,
                  long_description=long_description,
                  long_description_content_type="text/markdown",
                  url="https://github.com/lrmcfarland/Coordinates",
                  packages=setuptools.find_packages(),
                  classifiers=[
                      "Programming Language :: Python :: 3",
                      "License :: OSI Approved :: MIT License",
                      "Operating System :: OS Independent",
                      "Topic :: Scientific/Engineering :: Astronomy"
                  ],
                  ext_modules=[coords_module]
)

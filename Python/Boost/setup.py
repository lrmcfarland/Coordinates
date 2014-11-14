"""Creats python wrappers from boost macro

ASSUMES: ../../libCoords exists and /usr/local/[include,lib] has boost installed.
"""

from distutils.core import setup, Extension

name = 'coords'
version = '1.0'
BOOST_ROOT = '/usr/local'

include_dirs=['../../libCoords', BOOST_ROOT + '/include']


library_dirs=['../../libCoords', BOOST_ROOT + '/lib']

libraries = ['boost_python', 'coords']

sources = ['coords.cpp']

coords_module = Extension(name,
                         include_dirs=include_dirs,
                         libraries=libraries,
                         library_dirs=library_dirs,
                         sources=sources)

setup (name=name,
       version=version,
       description='coords package',
       ext_modules=[coords_module])

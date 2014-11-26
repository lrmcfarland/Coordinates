# builds python coords module
# from http://docs.python.org/extending/building.html

from distutils.core import setup, Extension

coords_module = Extension('coords',
                          include_dirs=['../../libCoords'], # TODO meh.
                          libraries=['Coords'],
                          library_dirs=['../../libCoords'], # TODO meh**2.
                          sources=['coords.cpp'])

setup (name='coords',
       version='1.0',
       description='coords package',
       ext_modules=[coords_module])


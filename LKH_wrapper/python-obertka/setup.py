from distutils.core import setup, Extension
setup(name='cvrp', version='1.1',  \
      ext_modules=[Extension('cvrp', ['vrp-main.c'])])
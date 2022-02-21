from distutils.core import setup, Extension
setup(name='helloworld', version='1.1',  \
      ext_modules=[Extension('helloworld', ['vrp-main.c'])])
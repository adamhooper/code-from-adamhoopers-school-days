#! /usr/bin/env python

from distutils.core import setup, Extension

module1 = Extension('solver',
		    include_dirs = ['/usr/include/glib-2.0',
		    		    '/usr/lib/glib-2.0/include'],
		    libraries = ['glib-2.0'],
		    sources = ['solver.c'])

setup (name = 'Solver',
       version = '1.0',
       description = 'My First Python Module',
       ext_modules = [module1])

#from distutils.core import setup, Extension
#
#ext = Extension('redis_packer', sources=['packer.c'])
#setup(name='redis_packer', version='0.1', description='Redis packer_command', ext_modules=[ext])
import os
from setuptools import setup, Extension, Feature
#speedups = Feature(
#    'optional C speed-enhancement module',
#    standard=True,
#    ext_modules = [
#        Extension('redis_packer._packer', ['redis_packer/_packer.c']),
#    ],
#)

readme = open(os.path.join(os.path.dirname(__file__), 'README.md')).read()

ext = Extension('redis_packer', sources=['_packer.c'])
setup(
    name='redis_packer',
    version='0.18',
    url='',
    license='MIT',
    author='Simon Zimmermann',
    author_email='simon@insmo.com',
    description='Implements a faster Redis pack_command',
    long_description=readme,
    zip_safe=False,
    ext_modules=[ext],
    #packages=['redis_packer'],
    #cmdclass={'build_ext': ve_build_ext},
    #features={'speedups': speedups}
)

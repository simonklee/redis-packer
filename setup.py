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

ext = Extension('redis_packer._packer', sources=['redis_packer/_packer.c'])
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
    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Environment :: Web Environment',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Topic :: Internet :: WWW/HTTP :: Dynamic Content',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Text Processing :: Markup :: HTML'
    ],
    packages=['redis_packer'],
    include_package_data=True,
    #cmdclass={'build_ext': ve_build_ext},
    ext_modules=[ext],
    #features={'speedups': speedups}
)

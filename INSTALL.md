# Mapnik Installation

Mapnik is cross platform and runs on Linux, Mac OSX, Solaris, *BSD, and Windows.

To configure and build Mapnik do:

```bash
    $ ./configure
    $ make
```

To trigger parallel compilation you can pass a JOBS value to make:

```bash
    $ JOBS=4 make
```

(Note that compiling Mapnik needs several GBytes of RAM. If you use parallel compiliation it needs more.)

To use a Python interpreter that is not named `python` for your build, do
something like the following instead:

```bash
    $ PYTHON=python2 ./configure
    $ make PYTHON=python2
```

NOTE: the above will not work on windows, rather see https://github.com/mapnik/mapnik/wiki/WindowsInstallation

Then to run the tests locally (without needing to install):

    make test-local

Install like:

    sudo make install

If you need to uninstall do:

    sudo make uninstall

For more details see the `Building` Section below.

Platform specific install guides at https://github.com/mapnik/mapnik/wiki/Mapnik-Installation

For troubleshooting help see https://github.com/mapnik/mapnik/wiki/InstallationTroubleshooting


## Depends

Build system dependencies are:

 * C++ compiler supporting `-std=c++11` (like >= g++ 4.8 or >= clang++ 3.4)
 * >= 2 GB RAM (> 5 GB for g++)
 * Python 2.4-2.7 
 * Scons (a copy is bundled)

Mapnik Core depends on:

 * Boost
    - >= 1.47 is required and >= 1.56 recommended
    - These libraries are used:
      - filesystem
      - system
      - thread (for python bindings only)
      - regex (optionally built with icu regex support)
      - program_options (optionally for mapnik command line programs)
 * libicuuc >= 4.0 (ideally >= 4.2) - International Components for Unicode
 * libz - Zlib compression
 * libfreetype - Freetype2 for font support (Install requires freetype-config)
 * libxml2 - XML parsing (Install requires xml2-config)
 * libharfbuzz - an OpenType text shaping engine (>=0.9.34 needed for CSS font-feature-settings support)

Mapnik Core optionally depends on:

 * libpng >= 1.2.x - PNG graphics (Default enabled, if found)
 * libjpeg - JPEG graphics (Default enabled, if found)
 * libtiff - TIFF graphics (Default enabled, if found)
 * libwebp - WEBP graphics  (Default enabled, if found)
 * libproj - PROJ.4 projection library (Default enabled, if found)

Mapnik Python bindings depend on:

 * Python 2.5-2.7 or >= 3.2 
 * Boost python
 * simplejson module if using <= 2.5

Note: Python 3.x is supported, see: https://github.com/mapnik/mapnik/wiki/Python3k

Additional optional dependencies:

 * Cairo >= 1.6.0 - Graphics library for output formats like PDF, PS, and SVG
    - pkg-config - Required for building with cairo support
    - pycairo - Python bindings for cairo
 * PostgreSQL (for PostGIS plugin support)
    - libpq - PostreSQL libraries
    - pg_config - PostgreSQL installation capabilities
 * libgdal - GDAL/OGR input (For gdal and ogr plugin support)
 * libsqlite3 - SQLite input (needs RTree support builtin) (sqlite plugin support)
 * libocci - Oracle input plugin support
 * libcurl - OSM input plugin support
 * librasterlite - Rasterlite input plugin support

Instructions for installing many of these dependencies on
various platforms can be found at the Mapnik Wiki:

https://github.com/mapnik/mapnik/wiki/Mapnik-Installation


## Building

The build system uses SCons, a pure python equivalent to autotools or cmake.

We provide a simple Makefile wrapper that can be used like:

    ./configure && make && make install

For help on what options are accepted do:

    ./configure --help

To interact with the local copy of scons directly you can do:

    python scons/scons.py configure

You can also use a globally installed scons:

    scons configure

If you want to clean your build do:

    make clean

If you experience odd configure errors, try cleaning the configure caches:

    make distclean

To install in a custom location do:

    ./configure PREFIX=/opt/mapnik

To pass custom CXXFLAGS or LDFLAGS do:

    ./configure CUSTOM_CXXFLAGS="-g -I/usr/include" CUSTOM_LDFLAGS="-L/usr/lib"

To pass custom paths to a dependency, like boost, do:

    ./configure BOOST_INCLUDES=/opt/boost/include BOOST_LIBS=/opt/boost/lib

To pass custom paths to a dependency, like icu, do:

    ./configure ICU_INCLUDES=/usr/local/include ICU_LIBS=/usr/local/include

For more details on usage see:

    https://github.com/mapnik/mapnik/wiki/UsingScons


## Testing Installation

You can run the Mapnik tests locally (without installing) like:

    make test-local # see the Makefile for how this works

Or you can install and test like:

    make install && make test

Many of the tests are written in python and you can run them individually like:

    make install
    python tests/python_tests/shapefile_test.py


## Learning Mapnik

### Help

Mapnik has an active community of talented users and developers making beautiful maps.

If you need help or want to participate starting points include:

- Sign up and post to the mailing list: http://mapnik.org/contact/
- Join and ask questions on the #mapnik channel on irc://irc.freenode.net/mapnik
- Add your help questions to https://github.com/mapnik/mapnik-support

### Cartographers

TileMill, which uses Mapnik internally, offers great step by step tutorials for
learning advanced map styling: http://mapbox.com/tilemill/docs/crashcourse/introduction/

### Programmers

Mapnik is great for building your own mapping applications. Visit
https://github.com/mapnik/mapnik/wiki/LearningMapnik for basic
tutorials on how to programmatically use Mapnik.

### Contributers

Read docs/contributing.markdown for resources for getting involved with Mapnik development.

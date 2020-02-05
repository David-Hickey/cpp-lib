# cpp-lib ![C/C++ CI](https://github.com/David-Hickey/cpp-lib/workflows/C/C++%20CI/badge.svg?branch=master)
Collection of libraries used by C++ projects in my PhD.

### Contents:
* `mathutils.hpp`: set of maths utilities like the dirac delta and levi-cevita symbol
* `arrayutils.hpp`: a set of nice arithmetic operations on a very thin custom aggregate class
* `tensorutils.hpp`: a set of utilities to make it easier to handle 2D tensors -- should probably generalise...
* `constants.hpp`: a set of Physical constants that will probably continue to grow as I need to use more of them
* `boundingbox.hpp`: a class representing a cuboid bound with useful helper methods
* `fluidutils.hpp`: an ever-growing set of handy hydrodynamics-related functions
* `config.hpp`: my nice wrapper around the less nice but very powerful `SimpleIni.h` library (`SimpleIni.h` is not mine)
* `argparse.hpp`: also not mine, but a handy utility to read CLI arguments which mirrors Python's `arseparse`

And last (but not least) an entire suite of tests to make sure everything works nicely. :)

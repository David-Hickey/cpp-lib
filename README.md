# cpp-lib
Collection of libraries used by C++ projects in my PhD.

### Contents:
* `mathutils.hpp`: set of maths utilities like the dirac delta and levi-cevita symbol
* `arrayutils.hpp`: a set of nice arithmetic operations on a very thin custom aggregate class
* `constants.hpp`: a set of Physical constants that will probably continue to grow as I need to use more of them
* `boundingbox.hpp`: a class representing a cuboid bound with useful helper methods
* `fluidutils.hpp`: an ever-growing set of handy hydrodynamics-related functions
* `config.hpp`: my nice wrapper around the less nice but very powerful `SimpleIni.h` library (`SimpleIni.h` is not mine)
* `argparse.hpp`: also not mine, but a handy utility to read CLI arguments which mirrors Python's `arseparse`

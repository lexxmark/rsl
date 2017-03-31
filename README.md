[![Build Status](https://travis-ci.org/lexxmark/rsl.svg?branch=master)](https://travis-ci.org/lexxmark/rsl)

# C++ Runtime Support Library
Runtime Support Library is a set of utility C++ classes.

## Supported platforms
* Microsoft Visual Studio 15 Update 3
* GCC 4.9

## How to use
This is header only library. Just add **rsl/include** directory to include paths of your project.

## How to build and run tests

1. make a directory to build
```
mkdir build
cd build
```

2. build tests
```
cmake "path_to_rsl/rsl/tests"
make

or

cmake -G "Visual Studio 14 Win64" "path_to_rsl/rsl/tests"
```
open **rsl_tests.sln** file and build solution

3. run tests
```
./rsl_tests
```

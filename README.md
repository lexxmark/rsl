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
## Dangling pointers
shared_ptr and unique_ptr classes solved memory managements issues in C++. But we still don't have any utility class to address dangling pointers problem. As many C++ researchers mentioned such class cannot be implemented with zero overhead principle. tsl::track::pointer<T> class is an attemp to make such class with the minimal overhead.

To catch pointer's dangling we need two classes:
1. **rsl::track::trackable** - a class that tracks lifetime of the object or resource.
2. **rsl::track::pointer** - a smart pointer class that will be got notified when corresponding trackable class is going to destroy.

The current implementation is **not threadsafe** and has the following memory overhead:
```
static_assert(sizeof(rsl::track::trackable) == sizeof(void*));
static_assert(sizeof(rsl::track::pointer<int>) == 4 * sizeof(void*));
```

rsl::track::trackable holds only one value - a pointer to a head of a chain of rsl::track::pointer instances.
Thus rsl::track::pointer class is a node of the two-directional list. It has pointers to previous and next nodes, pointer to the object it referencing to and one pointer used because rsl::track::pointer has virtual mathods to hide different dangling policies.

### How to use
To you track pointers for some object A you should have rsl::track::trackable instance that lives the same period of time as the object A. You could achieve this by making rsl::track::trackable part of A or combining A and rsl::track::trackable in larger object.
```C++
using namespace rsl::track;
struct A
{
    int value;
    trackable life_time;
};

A a;
pointer<A> p1(&a, a.life_time);
pointer<int> p2(&a.value, a.life_time);
```
```C++
using namespace rsl::track;
struct A
{
    int value;
};

std::pair<A, trackable> a;
pointer<A> p1(&a.first, a.second);
pointer<int> p2(&a.first.value, a.second);
```

Also rsl library has rsl::track::allocator class that make it easy to use track pointers with standard containers (see tests as examples).
```C++
rsl::track::vector<int> v{0, 1, 2};
pointer<int> p1(&v[1], v.get_allocator().get_trackable());
// compact version
auto p2 = get_ptr_at(v, 1)
```

cmake_minimum_required(VERSION 2.8.7)

project(rsl_tests CXX)

add_subdirectory(unittest-cpp)

include_directories(
    ../include
    ./unittest-cpp
)

add_definitions(-DRSL_ASSERT_ON_EXPECT)

if(MSVC) # has the support we need
    # remove unnecessary warnings about unchecked iterators
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_compile_options(/EHsc /W4)
else()
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -std=c++14 -O3 -Wall -Wextra -Wpedantic -Wno-missing-braces -Wconversion -Wsign-conversion -Wctor-dtor-privacy -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Woverloaded-virtual")
    elseif(COMPILER_SUPPORTS_CXX1Y)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -std=c++1y -O3 -Wall -Wextra -Wpedantic -Wno-missing-braces -Wconversion -Wsign-conversion -Wctor-dtor-privacy -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Woverloaded-virtual")
    elseif(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -std=c++11 -O3 -Wall -Wextra -Wpedantic -Wno-missing-braces -Wconversion -Wsign-conversion -Wctor-dtor-privacy -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Woverloaded-virtual")
    else()
      message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

add_executable(rsl_tests
	main.cpp
    track/pointer.cpp
    track/array.cpp
    track/deque.cpp
    track/forward_list.cpp
    track/list.cpp
    track/map.cpp
    track/set.cpp
    track/unique_ptr.cpp
    track/unordered_map.cpp
    track/unordered_set.cpp
    track/vector.cpp
)

target_link_libraries(rsl_tests UnitTest++)

add_test(rsl_tests rsl_tests)

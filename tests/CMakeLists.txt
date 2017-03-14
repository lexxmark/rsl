cmake_minimum_required(VERSION 2.8.7)

project(ref_ptr_tests CXX)

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
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -std=c++14 -O3 -Wall -Wextra -Wpedantic -Wno-missing-braces -Wconversion -Wsign-conversion -Wctor-dtor-privacy -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Woverloaded-virtual")
    elseif(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing -std=c++11 -O3 -Wall -Wextra -Wpedantic -Wno-missing-braces -Wconversion -Wsign-conversion -Wctor-dtor-privacy -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Woverloaded-virtual")
    else()
      message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

function(add_rsl_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} UnitTest++)
    add_test(
      ${name}
      ${name}
    )
endfunction()

add_rsl_test(ref_ptr_tests)
add_rsl_test(ref_unique_ptr_tests)

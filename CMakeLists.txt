cmake_minimum_required(VERSION 3.0)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_C_COMPILER /usr/bin/clang )
set( CMAKE_CXX_COMPILER /usr/bin/clang++ )

# add_compile_options( -O2 -std=c++17 )

project( basic )
add_executable(basic
  examples/basic.cpp
)
target_include_directories( basic PUBLIC
  ./include
)

project( calculator )
add_executable(calculator
  examples/calculator.cpp
)
target_include_directories( calculator PUBLIC
  ./include
)

set_target_properties(
  basic calculator
  PROPERTIES
  LANGUAGE CXX
  CXX_STANDARD 17
)
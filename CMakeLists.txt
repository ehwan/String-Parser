cmake_minimum_required(VERSION 3.0)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( LANGUAGE CXX )

add_compile_options( -O2 -std=c++17 )

project( basic )
add_executable(basic
  examples/basic.cpp
)

project( calculator )
add_executable(calculator
  examples/calculator.cpp
)

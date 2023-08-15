cmake_minimum_required(VERSION 3.27)

add_compile_options( -O2 -std=c++17 )

project( basic )
add_executable(basic
  basic.cpp
)

project( calculator )
add_executable(calculator
  calculator.cpp
)

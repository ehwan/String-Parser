cmake_minimum_required(VERSION 3.5)
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

project( compiler )
add_executable( compiler
  examples/compiler/node.cpp
  examples/compiler/instruction.cpp
  examples/compiler/tokenizer.cpp
  examples/compiler/compiler.cpp
  examples/compiler/program.cpp
  examples/compiler/main.cpp
)
target_include_directories( compiler PUBLIC
  ./include
)

set_target_properties(
  basic calculator compiler
  PROPERTIES
  LANGUAGE CXX
  CXX_STANDARD 17
)


find_package( GTest )
project( test CXX )
add_executable( test
  test/test.cpp
)
target_link_libraries( test PUBLIC GTest::gtest )
target_include_directories( test PUBLIC
  ./include
)
set_target_properties( test PROPERTIES
  LANGUAGE CXX
  CXX_STANDARD 17
)
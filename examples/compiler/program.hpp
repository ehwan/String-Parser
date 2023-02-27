#pragma once

#include "global.hpp"
#include <vector>
#include <stack>
#include <memory>
#include <iostream>

struct program_t
{
  int current_instruct;
  int main;
  std::vector<instruction_t*> instructions;
  std::stack<int> stack;
  std::unique_ptr<int[]> memory;

  program_t( int var_used )
  {
    memory.reset( new int[ var_used ] );
  }
  ~program_t();
  void run();
};

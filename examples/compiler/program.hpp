#pragma once

#include "global.hpp"
#include <vector>
#include <stack>
#include <iostream>

#include "instruction.hpp"

namespace eh { namespace compiler {

// virtual program structure emitted by compiler_t
class program_t
{
public:
  int current_instruct;
  int main;
  std::vector<std::unique_ptr<instruction_t>> instructions;
  std::stack<int> stack;

  label_map labels;
  variable_map variables;

  void run();
};

}}
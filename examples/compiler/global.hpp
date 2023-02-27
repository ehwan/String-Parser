#pragma once

#include <string>
#include <map>
#include <functional>
#include <memory>
#include <vector>

struct var_info
{
  int offset;
};

struct instruction_t;

struct program_t;
struct compiler_t;
struct node_t;
using node_ptr = std::unique_ptr<node_t>;
//using instruction_t = std::function<void(program_t*)>;
using label_info = int;
using variable_map = std::map<std::string,var_info>;
using label_map = std::map<std::string,label_info>;

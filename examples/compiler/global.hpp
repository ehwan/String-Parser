#pragma once

#include <map>
#include <string>

namespace eh { namespace compiler {

struct node_t;
class program_t;
struct instruction_t;

using variable_map = std::map< std::string, int >;
using label_map = std::map<std::string,int>;

}}
#pragma once

#include <parser.hpp>
#include "global.hpp"
#include "node.hpp"
#include "tokenizer.hpp"

namespace eh { namespace compiler {

class compiler_t
{
  using rule_t = eh::parser::rule<
    node_ptr,
    std::vector<token_t>::iterator
  >;

  struct
  {
    rule_t expr1;
    rule_t rhs;

    rule_t statements;

    rule_t program;
  } rules;


public:
  std::vector<token_t> tokens;
  std::unique_ptr<program_t> program;

  compiler_t();
  bool compile();
};

}}
#pragma once

#include "../../parser.hpp"
#include "global.hpp"

#include <map>
#include <string>
#include <vector>
#include <memory>


class compiler_t
{
  using token_type =
    eh::parser::token_t<int,std::vector<char>::iterator>;
  using rule_t = eh::parser::rule<
    node_ptr ,
    std::vector<token_type>::iterator
  >;
protected:

  struct
  {
    rule_t expr1;
    rule_t rhs;

    rule_t statements;

    rule_t program;
  } rules;

  variable_map variables;
  int var_used;

  label_map labels;

public:
  compiler_t();
  std::unique_ptr<program_t> compile(
      std::vector<token_type>::iterator b , 
      std::vector<token_type>::iterator e );


};


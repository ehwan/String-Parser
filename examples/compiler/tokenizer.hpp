#pragma once

#include <parser.hpp>
#include <vector>

namespace eh { namespace compiler {

struct token_t
{
  constexpr static int TOKEN_IGNORE = 0;
  constexpr static int TOKEN_NUM = 1001;
  constexpr static int TOKEN_IDENT = 1002;
  constexpr static int TOKEN_READ = 1003;
  constexpr static int TOKEN_PRINT = 1004;
  constexpr static int TOKEN_WHILE = 1005;
  constexpr static int TOKEN_IF = 1006;
  constexpr static int TOKEN_LEQ = 1007;
  constexpr static int TOKEN_GEQ = 1008;
  constexpr static int TOKEN_EQ = 1009;
  constexpr static int TOKEN_NEQ = 1010;
  constexpr static int TOKEN_LOGICAL_AND = 1011;
  constexpr static int TOKEN_LOGICAL_OR = 1012;
  constexpr static int TOKEN_RETURN = 1013;

  int type;
  std::string raw;

  bool operator == ( token_t const& rhs ) const
  {
    return type == rhs.type;
  }
  bool operator != ( token_t const& rhs ) const
  {
    return type != rhs.type;
  }
  bool operator ==( int t ) const
  {
    return type == t;
  }
  bool operator !=( int t ) const
  {
    return type != t;
  }

  operator int () const
  {
    return type;
  }
};

class tokenizer_t
{
public:
  std::vector<char> input;
  std::vector<token_t> tokens;

  bool parse();
};

}}
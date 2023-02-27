#pragma once

#include "../../parser.hpp"

#define TOKEN_IGNORE 0
#define TOKEN_NUM 1001
#define TOKEN_IDENT 1002
#define TOKEN_READ 1003
#define TOKEN_PRINT 1004
#define TOKEN_WHILE 1005
#define TOKEN_IF 1006
#define TOKEN_LEQ 1007
#define TOKEN_GEQ 1008
#define TOKEN_EQ 1009
#define TOKEN_NEQ 1010
#define TOKEN_LOGICAL_AND 1011
#define TOKEN_LOGICAL_OR 1012
#define TOKEN_RETURN 1013

template < typename I >
std::vector< eh::parser::token_t<int,I> > tokenize( I begin , I end )
{
using namespace eh::parser;
  auto c_comment =
    ("/*"_p >> *(any.unused() - "*/"_p) >> "*/"_p);
  auto cpp_comment =
    ("//"_p >> *(any.unused() - '\n'_p) >> '\n'_p.notconsume()).unused();
  auto white = c_comment | cpp_comment | " \n\t"_pone.unused();

  auto chars =
    ";{}()=+-*/%!<>,"_pone[(
        []( char x )->int
        {
          return x;
        } )];

  auto num = (+digit.unused()).attr( TOKEN_NUM );
  auto identity = ident.attr( TOKEN_IDENT );
  dictionary<int> dict;
  dict.add( std::string("while") , TOKEN_WHILE );
  dict.add( std::string("print") , TOKEN_PRINT );
  dict.add( std::string("read") , TOKEN_READ );
  dict.add( std::string("if") , TOKEN_IF );
  dict.add( std::string(">=") , TOKEN_LEQ );
  dict.add( std::string("<=") , TOKEN_GEQ );
  dict.add( std::string("==") , TOKEN_EQ );
  dict.add( std::string("!=") , TOKEN_NEQ );
  dict.add( std::string("&&") , TOKEN_LOGICAL_AND );
  dict.add( std::string("||") , TOKEN_LOGICAL_OR );
  dict.add( std::string("return") , TOKEN_RETURN );
  auto parser =
    dict |
    identity | num | chars;

  return eh::parser::tokenize( parser , white , begin , end );
}

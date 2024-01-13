#include "tokenizer.hpp"
#include <iostream>

namespace eh { namespace compiler {

bool tokenizer_t::parse()
{
  namespace ep = eh::parser;
  using namespace ep::literals;

  auto c_comment =
    ("/*"_p >> *(ep::any.unused() - "*/"_p) >> "*/"_p);
  auto cpp_comment =
    ("//"_p >> *(ep::any.unused() - '\n'_p) >> '\n'_p.notconsume()).unused();

  auto ignore = c_comment | cpp_comment | " \n\t"_pone.unused();

  auto chars =
    ";{}()=+-*/%!<>,"_pone[(
        []( char x )->int
        {
          return x;
        } )];

  auto num = (+ep::digit.unused()).attr( token_t::TOKEN_NUM );
  auto identifier = ep::ident.attr( token_t::TOKEN_IDENT );
  ep::dictionary<int> dict;
  dict.add( std::string("while") , token_t::TOKEN_WHILE );
  dict.add( std::string("print") , token_t::TOKEN_PRINT );
  dict.add( std::string("read") , token_t::TOKEN_READ );
  dict.add( std::string("if") , token_t::TOKEN_IF );
  dict.add( std::string(">=") , token_t::TOKEN_LEQ );
  dict.add( std::string("<=") , token_t::TOKEN_GEQ );
  dict.add( std::string("==") , token_t::TOKEN_EQ );
  dict.add( std::string("!=") , token_t::TOKEN_NEQ );
  dict.add( std::string("&&") , token_t::TOKEN_LOGICAL_AND );
  dict.add( std::string("||") , token_t::TOKEN_LOGICAL_OR );
  dict.add( std::string("return") , token_t::TOKEN_RETURN );

  auto token_id_parser =
    dict |
    identifier | num | chars;

  tokens.clear();

  auto begin = input.begin();
  while( begin != input.end() )
  {
    // skip whitespaces & comments
    while( ignore.parse( begin, input.end() ) )
    {
    }

    token_t token;

    auto token_parser = token_id_parser[(
      [&token]( int id )
      {
        token.type = id;
      }
    )].view()[(
      [&token]( decltype(input)::iterator begin, decltype(input)::iterator end )
      {
        token.raw.assign( begin, end );
      }
    )];

    auto parse_result = token_parser.parse( begin, input.end() );
    if( parse_result.is_valid() )
    {
      tokens.push_back( std::move(token) );
    }else {
      return false;
    }
  }
  return true;
}

}}
#include "../parser.hpp"

#include <string>
#include <iostream>

namespace ep {
  using namespace eh::parser;
};

// parsing variable-name pattern
// [a-zA-Z_][a-zA-Z0-9_]*
void variable_name_parse()
{
  auto small_alpha_parser = ep::range( 'a', 'z' );
  auto big_alpha_parser = ep::range( 'A', 'Z' );
  auto digit_parser = ep::range( '0', '9' );
  auto underscore_parser = ep::one( '_' );

  auto variable_parser =
    ep::seq(
        ep::or_(small_alpha_parser, big_alpha_parser, underscore_parser),
        ep::repeat(
          ep::or_(small_alpha_parser, big_alpha_parser, digit_parser, underscore_parser),
          0,
          99999
        )
    );

  // this is same as variable_parser
  auto variable_parser2 =
    ( small_alpha_parser | big_alpha_parser | underscore_parser )
    >>
    *( small_alpha_parser | big_alpha_parser | digit_parser | underscore_parser )
    ;

  std::string str = "foo123 var456";
  auto begin = str.begin();

  // return optional<AttributeType>
  auto ret = variable_parser.parse( begin, str.end() );
  // begin points where parsing ends
  std::string parsed_name( str.begin(), begin );

  ++begin;
  auto mid = begin;
  auto ret2 = variable_parser2.parse( begin, str.end() );
  std::string parsed_name2( mid, begin );

  std::cout << parsed_name << std::endl;
  std::cout << parsed_name2 << std::endl;
}
void dictionary()
{
  eh::parser::dictionary<int,char> dict;
  dict.add( std::string("hello") , 0 );
  dict.add( std::string("hell") , 2 );
  dict.add( std::string("world") , 1 );
  dict.add( std::string("hellowo") , 4 );

  std::string str = "helloworld";
  auto b = str.begin();
  auto ret = dict.parse( b , str.end() );
  if( ret )
  {
    std::cout << ret.get();
  }else
  {
    std::cout << "fail";
  }
}
void tokenize()
{
  // tokeninze string
  std::string str = "   11 1aa    a2 22b     bb    ";

  // white spaces parser; will be ignored when parsing
  auto white = ' '_p;

  // return 0 for alphabet, 1 for digit
  auto parser = 
    eh::parser::alpha[( [](int x){return 0;})] |
    eh::parser::digit[( [](int x){return 1;})];

  auto tokens = eh::parser::tokenize( parser , white , str.begin() , str.end() );
  for( auto i : tokens )
  {
    if( i.data == 0 )
    {
      std::cout << "alpha " << *i.begin << std::endl;
    }else if( i.data == 1 )
    {
      std::cout << "digit " << *i.begin << std::endl;
    }else
    {
      std::cout << "unknown\n";
    }
  }
}

void comment_parser()
{
  // commenting out
  auto printstring = []( std::string &str ){ std::cout << str << '\n'; };

  // C-Style comment
  auto comment_c = 
    ("/*"_p).unused() >>
    (*( eh::parser::any - "*/"_p )) >>
    ("*/"_p).unused()
    ;

  // Cpp-Style comment
  auto comment_cpp =
    "//"_p >>
    *( eh::parser::any - '\n'_p ) >>
    '\n'_p.notconsume()
    ;

  auto comment_out =
    *(
    comment_c.unused() |
    comment_cpp.unused() |
    eh::parser::any
    );
  std::string str =
    "hello world\n"
    "this is // hiding comment\n"
    "and /*this is hiding too*/ this is\n"
    "multi line\n";
  auto b = str.begin();

  // return optional< vector<char> >
  auto ret = comment_out.parse( b , str.end() );
  for( auto i : ret.get() )
  {
    std::cout << i;
  }
}

int main()
{
  //variable_name_parse();
  //tokenize();
  comment_parser();
}

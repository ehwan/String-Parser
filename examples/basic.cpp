#include "../parser.hpp"

#include <string>
#include <iostream>

int main()
{
#if 0
  // commenting out
  auto printstring = []( std::string &str ){ std::cout << str << '\n'; };
  auto comment_c = 
    ("/*"_p).unused() >>
    (*( eh::parser::any - "*/"_p )).string()[printstring] >>
    ("*/"_p).unused()
    ;

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
  auto ret = comment_out.parse( b , str.end() );
  for( auto i : ret.get() )
  {
    std::cout << i;
  }
#endif

#if 0
  // tokeninze
  std::string str = "   11 1aa    a2 22b     bb    ";
  auto white = ' '_p;
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
  auto b = str.begin();
  auto aa = (*('_'_p.unused())).parse( b , str.end() );
#endif

#if 0
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

#endif

  std::string str = "abcabc";
  auto b = str.begin();

  auto ap = 'a'_p.unused()[(
      []()
      {
      std::cout << "a!\n";
      } )];

  auto p = ( ap >> 'b'_p >> 'x'_p ) |
    (ap >> 'x'_p) |
    ( eh::parser::repeat( ap , 3 , -1 ) ) |
    (ap >> 'b'_p >> 'c'_p);
  auto ret = p.parse( b , str.end() );
}

#include <iostream>
#include <string>

#include "../parser.hpp"

struct accum_t
{
  int data;
  accum_t()
  {
    data = 0;
  }
  void operator()( int x )
  {
    data = data*10 + x;
  }
  int operator()() const
  {
    return data;
  }
};
using rule_t = eh::parser::rule<int,std::string::iterator>;

int main()
{
  auto num =
    eh::parser::action(
    eh::parser::repeat( eh::parser::digit0, 1, 10000 ),
    []( auto &v )
    {
    int x = 0;
    for( int i : v )
    {
    x = x *10 + i-'0';
    }
    return x;
    } );
  rule_t expr;
  rule_t expr0 = num;
  rule_t expr1;
  expr1 =
    ( expr0.ref() >> '*'_p >> expr1.ref() )[(
        []( int a , char x , int b )->int
        {
          return a * b;
        })] | expr0.ref();
  rule_t expr2;
  expr2 =
    ( expr1.ref() >> '+'_p >> expr2.ref() )[(
        []( int a , char x , int b )->int
        {
          return a + b;
        })] | expr1.ref();

  expr = expr2;

  std::string str;
  while( std::cin >> str )
  {
  auto b = str.begin();
  auto ret = expr.parse( b , str.end() );
  std::cout << ret.get() << std::endl;
  }
}

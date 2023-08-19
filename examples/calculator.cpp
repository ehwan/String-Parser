#include <ios>
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
    eh::parser::action(
      expr0.ref() 
      >> eh::parser::repeat(
        (('*'_p).unused() >> expr0.ref()),
        0,
        100000
      ),
      []( int e1, std::vector<int> &rights )
      {
        for( int i : rights )
        {
          e1 *= i;
        }
        return e1;
      }
    );
  rule_t expr2;
  expr2 =
    eh::parser::action(
      expr1.ref() 
      >> eh::parser::repeat(
        (('+'_p).unused() >> expr1.ref()),
        0,
        100000
      ),
      []( int e1, std::vector<int> &rights )
      {
        for( int i : rights )
        {
          e1 += i;
        }
        return e1;
      }
    );

  expr = expr2;

  std::string str = "1+2*3";
  auto begin = str.begin();
  auto result = expr.parse( begin, str.end() );
  std::cout << "Input String : " << str << "\n";
  std::cout << "Parse Result : " << std::boolalpha << result.is_valid() << "\n";
  std::cout << "Parse Data : " << result.get() << "\n";
}

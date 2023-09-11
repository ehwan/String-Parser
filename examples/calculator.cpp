#include <iostream>
#include <string>

#include "parser.hpp"

namespace ep = eh::parser;

int main()
{
  // REGEX : [0-9]
  // Pattern Match and return its digit as integer type
  auto digit_parser = 
    ep::action(
      ep::range( '0', '9' ),

      // captured Attribute of range() is decltype(*iterator) = char
      // this functor will be called every range() pattern matched
      []( char captured_character )
      {
        // return char->digit
        // this will be new Attribute of this parser
        return (int)(captured_character - '0');
      }
    );

  // make action() using [] operator
  auto digit_parser2 =
    ep::range('0', '9')[ digit_parser.functor ];
  

  // REGEX : [0-9]+
  auto integer_parser =
    ep::action(
      ep::repeat( digit_parser, 1, 12 ),

      // captured Attribute of repeat() is vector< child Attribute > = vector<int>
      []( std::vector<int> &captured_digits )
      {
        int result = 0;
        for( int d : captured_digits )
        {
          result = result * 10 + d;
        }
        // return accumulated integer;
        // this will be new Attribute of this parser
        return result;
      }
    );

  // REGEX : integer_parser ( ('+'|'-') integer_parser )*
  auto plus_minus_expression_noaction =
    ep::seq( // Attribute = tuple< Attr of child > = tuple< int, vector< tuple<char,int> > >
      integer_parser, // Attribute = int
      ep::repeat( // Attribute = vector< Attr of child > = vector< tuple<char,int> >
        ep::seq( // Attr = tuple< Attrs of child > = tuple< char, int >
          ep::or_(  // char ( only if all child's Attr are same )
            ep::one('-'), // char
            ep::one('+') // char
          ),
          integer_parser // int
        ),
        0, 10000000
      )
    );

  // or using literals and operator
  using namespace ep::literals;
  auto plus_minus_expression_noaction2 =
    integer_parser
    >>
    *(
       ('-'_p | '+'_p) >> integer_parser
    );
      
  auto plus_minus_expression =
    ep::action(
      plus_minus_expression_noaction,
      // captured tuple will be automatically unpacked
      []( int first_captured_integer, std::vector< std::tuple<char,int> > const& rhs_captured )
      {
        int result = first_captured_integer;

        // process right-hand-side operations
        for( auto tup : rhs_captured )
        {
          char operation = std::get<0>( tup );
          int rhs_integer = std::get<1>( tup );

          if( operation == '+' ){ result += rhs_integer; }
          else if( operation == '-' ){ result -= rhs_integer; }
        }

        // return calculated integer;
        // this will be new Attribute of this parser
        return result;
      }
    );

    std::string input_string = "1+22-333+4444-55555+666666 unknown string";

    auto begin = input_string.begin();
    auto parse_result = plus_minus_expression.parse( begin, input_string.end() );

    std::cout << "Input String : " << input_string << "\n";
    std::cout << "Parse Result : " << std::boolalpha << parse_result.is_valid() << "\n";
    std::cout << "Parsed Data : " << parse_result.get() << " = " << 1+22-333+4444-55555+666666 << "\n";

    std::cout << "Iterator 'begin' point at : " << std::string( begin, input_string.end() ) << "\n";
}
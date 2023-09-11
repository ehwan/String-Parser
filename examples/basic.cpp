#include "../parser.hpp"

#include <string>
#include <iostream>

namespace ep {
  using namespace eh::parser;
};

int main()
{
  /*
    Every Parser Object have parse( begin&:iterator, end:iterator ) function
    that performs pattern-mathing scheme
  */

  /*
    range( min_, max_ ) returns a Parser Object.
    It's parse() function will consume one iterator
    and returns if a character is in range [min_, max_]

    if the pattern doesn't match, iterator will not move
  */
  auto one_small_alphabet = ep::range( 'a', 'z' );
  auto one_big_alphabet = ep::range( 'A', 'Z' );
  auto one_digit = ep::range( '0', '9' );

  std::string my_string = "abcdefg 123456";

  auto begin = my_string.begin();
  /*
    pattern matched, 'begin' will now point my_string[1]
  */
  one_small_alphabet.parse( begin, my_string.end() );

  /*
    pattern doesn't match, 'begin' will not move
  */
  one_big_alphabet.parse( begin, my_string.end() );

  /*
  parse() function returns an parse_result_t<Attr> value
  which contains the result of pattern matching
  and the parsed data from input stream.

  for range() parser, the character it consumed will be returned as its Attr
  */
  auto parse_result = one_small_alphabet.parse( begin, my_string.end() );
  std::cout << "Parse Result : " << std::boolalpha << parse_result.is_valid() << "\n";
  std::cout << "Parsed Character : " << parse_result.get() << "\n";
  
  // at this point, 'begin' points my_string[2]

  /*
  action( Parser, Functor ) is special parser wrapper.
  It performs same pattern-matching as its child parser,
  but call a functor() every pattern matching is successfully done.

  functor() could take child parser's Attr as its argument,
  and the returned value willbe its new Attr
  */
  auto action_parser = ep::action(
    one_small_alphabet,
    []( int ch )
    {
      std::cout << "Small Alphabet Parsing Successfully Done!\n";

      // now 'ch*2' is its new Attr
      return ch*2;
    } );

  // pattern matched, functor() will be called
  // and its Attr would be 'c'*2
  auto action_result = action_parser.parse( begin, my_string.end() );
  std::cout << "New Attr : " << action_result.get() << "\n";


  /* 
  There are more special Parser Wrapper for advaced Parser creation.

  or_( parser1, parser2, ... , parserN )
  will test every N parsers until one of them is successfully matched

  seq( parser1, parser2, ..., parserN )
  will test every N parsers sequentially.

  repeat( parser, min_, max_ )
  will test 'parser' X times where X is in range [min_,max_]
  */

  auto one_alphabet_parser = ep::or_( one_small_alphabet, one_big_alphabet );

  // this is same as one_alphabet_parser
  auto one_alphabet_parser2 = one_small_alphabet | one_big_alphabet;

  auto one_alphabet_and_digit = ep::seq( one_alphabet_parser, one_digit );
  auto one_alphabet_and_digit2 = one_alphabet_parser >> one_digit;

  // [a-zA-Z]*
  auto alphabet_star = ep::repeat( one_alphabet_parser, 0, 9999999 );

  // [a-zA-Z]+
  auto alphabet_plus = ep::repeat( one_alphabet_parser, 1, 9999999 );
}

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

  // return parse_result_t<AttributeType>
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
  // dictionary will return the longest-matched-string's attribute
  eh::parser::dictionary<int,char> dict;
  dict.add( std::string("hello") , 0 );
  dict.add( std::string("hell") , 2 );
  dict.add( std::string("world") , 1 );
  dict.add( std::string("hellowo") , 4 );
  dict.add( "dictword1", 10 );
  dict.add( std::string_view("string_view only for c++17"), 10000 );

  std::string str = "helloworld";
  auto b = str.begin();

  // parse_result< Attribute Type >
  ep::parse_result_t<int> ret = dict.parse( b , str.end() );
  if( ret ) // ret.is_valid()
  {
    std::cout << ret.get();
  }else
  {
    std::cout << "fail";
  }
}

void comment_parser()
{
  // commenting out
  auto printstring = []( std::string &str ){ std::cout << str << '\n'; };

  // using string literals
  using namespace ep::literals;


  // _p for complete-matching
  // C-Style comment
  auto comment_c = 
    // between "/*" and "*/"
    ("/*"_p).unused() >>

    // any character but "*/"
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

  // return parse_result_t< vector<char> >
  auto ret = comment_out.parse( b , str.end() );
  for( auto i : ret.get() )
  {
    std::cout << i;
  }
}
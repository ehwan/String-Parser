# String Parser
Header only template-based string parser and pattern matcher.

## Examples
see 'examples/basic.cpp' for basic tutorial

Every Parser Object have parse( begin&:iterator, end:iterator ) function
that performs pattern-mathing scheme

range( min_, max_ ) returns a Parser Object.
It's parse() function will consume one iterator
and returns if a character is in range [min_, max_]

if the pattern doesn't match, iterator will not move

'''cpp
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
'''

parse() function returns an 'optional<Attr>' value
which contains the result of pattern matching
and the parsed data from input stream.

for range() parser, the character it consumed will be returned as its Attr

'''cpp
  auto parse_result = one_small_alphabet.parse( begin, my_string.end() );
  std::cout << "Parse Result : " << std::boolalpha << parse_result.is_valid() << "\n";
  std::cout << "Parsed Character : " << parse_result.get() << "\n";
  
  // at this point, 'begin' points my_string[2]
'''

action( Parser, Functor ) is special parser wrapper.
It performs same pattern-matching as its child parser,
but call a functor() every pattern matching is successfully done.

functor() could take child parser's Attr as its argument,
and the returned value willbe its new Attr

'''cpp
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
'''


There are more special Parser Wrapper for advaced Parser creation.

or_( parser1, parser2, ... , parserN )
will test every N parsers until one of them is successfully matched

seq( parser1, parser2, ..., parserN )
will test every N parsers sequentially.

repeat( parser, min_, max_ )
will test 'parser' X times where X is in range [min_,max_]

'''cpp
  auto one_alphabet_parser = ep::or_( one_small_alphabet, one_big_alphabet );

  // this is same as one_alphabet_parser
  auto one_alphabet_parser2 = one_small_alphabet | one_big_alphabet;

  auto one_alphabet_and_digit = ep::seq( one_alphabet_parser, one_digit );
  auto one_alphabet_and_digit2 = one_alphabet_parser >> one_digit;

  // [a-zA-Z]*
  auto alphabet_star = ep::repeat( one_alphabet_parser, 0, 9999999 );

  // [a-zA-Z]+
  auto alphabet_plus = ep::repeat( one_alphabet_parser, 1, 9999999 );
'''
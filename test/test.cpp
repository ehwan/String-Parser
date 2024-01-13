#include "rules/dictionary.hpp"
#include "rules/stringmatch.hpp"
#include "unused.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <parser.hpp>
#include <string>

namespace ep = eh::parser;
using namespace ep::literals;

// parser that always return same parse result;
// does not consume iterator
TEST( ParserTest, Always )
{
  std::string src = "abcdefg";

  auto begin = src.begin();
  auto res = ep::true_.parse( begin, src.end() );
  EXPECT_EQ( res.is_valid(), true );
  EXPECT_EQ( begin, src.begin() );

  res = ep::false_.parse( begin, src.end() );
  EXPECT_EQ( res.is_valid(), false );
  EXPECT_EQ( begin, src.begin() );
}

TEST( ParserTest, One )
{
  std::string src = "abc";

  auto begin = src.begin();
  auto res = ep::one( 'a' ).parse( begin, src.end() );
  EXPECT_EQ( res.is_valid(), true );
  EXPECT_EQ( begin, src.begin()+1 );
  EXPECT_EQ( res.get(), 'a' );

  res = ep::one( 'a' ).parse( begin, src.end() );
  EXPECT_EQ( res.is_valid(), false );
  EXPECT_EQ( begin, src.begin()+1 );

  res = ep::one( 'b' ).parse( begin, src.end() );
  EXPECT_EQ( res.is_valid(), true );
  EXPECT_EQ( begin, src.begin()+2 );
  EXPECT_EQ( res.get(), 'b' );

  res = ep::one( 'c' ).parse( begin, src.end() );
  EXPECT_EQ( res.is_valid(), true );
  EXPECT_EQ( begin, src.begin()+3 );
  EXPECT_EQ( res.get(), 'c' );

  res = ep::one( 'c' ).parse( begin, src.end() );
  EXPECT_EQ( res.is_valid(), false );
  EXPECT_EQ( begin, src.begin()+3 );
}

TEST( ParserTest, Range )
{
  std::string src = "abcd1234";

  auto a_to_d = ep::range( 'a', 'd' );
  auto digit = ep::range( '1', '9' );

  auto begin = src.begin();
  auto res = a_to_d.parse( begin, src.end() );
  EXPECT_EQ( res.is_valid(), true );
  EXPECT_EQ( begin, src.begin()+1 );
  EXPECT_EQ( res.get(), 'a' );

  while( 1 )
  {
    res = a_to_d.parse( begin, src.end() );
    if( res.is_valid() == false ){ break; }
  }
  EXPECT_EQ( begin, src.begin()+4 );

  res = digit.parse( begin, src.end() );
  EXPECT_TRUE( res.is_valid() );
  EXPECT_EQ( begin, src.begin()+5 );
  EXPECT_EQ( res.get(), '1' );

  while( 1 )
  {
    res = digit.parse( begin, src.end() );
    if( res.is_valid() == false ){ break; }
  }
  EXPECT_EQ( begin, src.end() );
}

// check one character is in set
TEST( ParserTest, CharacterSet )
{
  auto even_digits = ep::set_( std::set<char>{ '2', '4', '6', '8', '0' } );
  auto odd_digits = "13579"_pone;

  std::string src = "01";

  auto begin = src.begin();
  auto res = odd_digits.parse( begin, src.end() );
  EXPECT_FALSE( res );
  EXPECT_EQ( begin, src.begin() );

  res = even_digits.parse( begin, src.end() );
  EXPECT_TRUE( res );
  EXPECT_EQ( begin, src.begin()+1 );
  EXPECT_EQ( res.get(), '0' );

  res = even_digits.parse( begin, src.end() );
  EXPECT_FALSE( res );
  EXPECT_EQ( begin, src.begin()+1 );

  res = odd_digits.parse( begin, src.end() );
  EXPECT_TRUE( res );
  EXPECT_EQ( begin, src.begin()+2 );
  EXPECT_EQ( res.get(), '1' );

  // end of string
  res = even_digits.parse( begin, src.end() );
  EXPECT_FALSE( res );
  EXPECT_EQ( begin, src.end() );

  res = odd_digits.parse( begin, src.end() );
  EXPECT_FALSE( res );
  EXPECT_EQ( begin, src.end() );
}

// check single string match
TEST( ParserTest, StringMatch )
{
  std::string src = "prefixprefi";
  auto prefix_correct = ep::string_match( std::string("prefix") );
  auto prefix_incorrect = "prefux"_p;

  auto begin = src.begin();
  auto res = prefix_incorrect.parse( begin, src.end() );
  EXPECT_FALSE( res );
  EXPECT_EQ( begin, src.begin() );

  res = prefix_correct.parse( begin, src.end() );
  EXPECT_TRUE( res );
  EXPECT_EQ( begin, src.begin()+6 );

  res = prefix_incorrect.parse( begin, src.end() );
  EXPECT_FALSE( res );
  EXPECT_EQ( begin, src.begin()+6 );

  res = prefix_correct.parse( begin, src.end() );
  EXPECT_FALSE( res );
  EXPECT_EQ( begin, src.begin()+6 );
}

// check longest string match in dictionary
TEST( ParserTest, Dictionary )
{
  // attribute type & character type
  ep::dictionary<int,char> dict;

  dict.add( "hello", 0 );
  dict.add( "hellow", 1 );
  dict.add( "helloworld", 2 );

  dict.add( "bye", 3 );
  dict.add( "byebye", 4 );

  std::string src = "helloworldbyebyeworld";
  auto begin = src.begin();

  auto res = dict.parse( begin, src.end() );
  EXPECT_TRUE( res );
  EXPECT_EQ( res.get(), 2 );
  EXPECT_EQ( begin, src.begin()+10 );

  res = dict.parse( begin, src.end() );
  EXPECT_TRUE( res );
  EXPECT_EQ( *res, 4 );
  EXPECT_EQ( begin, src.begin()+10+6 );

  res = dict.parse( begin, src.end() );
  EXPECT_FALSE( res );
  EXPECT_EQ( begin, src.begin()+10+6 );
}

TEST( ParserTest, Action )
{
  std::string src = "12345";

  auto begin = src.begin();

  // action takes one character and return nothing
  int old_attr;
  auto parse_digit0 = ep::digit[(
    [&old_attr]( int ch )
    {
      old_attr = ch;
    } )];

  auto res = parse_digit0.parse( begin, src.end() );
  EXPECT_TRUE( res );
  EXPECT_EQ( typeid(res.get()), typeid(ep::unused_t) );
  EXPECT_EQ( begin, src.begin()+1 );
  EXPECT_EQ( old_attr, '1' );

  // action takes one character and return new value
  auto parse_digit1 = ep::digit[(
    []( int ch ) -> long long
    {
      return ch * 2;
    } )];
  auto res2 = parse_digit1.parse( begin, src.end() );
  EXPECT_TRUE( res2 );
  EXPECT_EQ( typeid(res2.get()), typeid(long long) );
  EXPECT_EQ( res2.get(), '2'*2 );
  EXPECT_EQ( begin, src.begin()+2 );


  // action takes nothing (for unused attribute parser) and return new value
  auto parse_digit2 = ep::digit.unused()[(
    []() -> float
    {
      return 1.0f;
    }
  )];
  auto res3 = parse_digit2.parse( begin, src.end() );
  EXPECT_TRUE( res3 );
  EXPECT_EQ( typeid(res3.get()), typeid(float) );
  EXPECT_EQ( begin, src.begin()+3 );
  EXPECT_LE( std::abs(res3.get()-1.0f), 1e-6f );

  auto parse_two_digit =
    (ep::digit >> ep::digit)[(
      []( int a, int b )
      {
        // reversed
        return std::pair<int,int>(b, a);
      } )];
  auto res4 = parse_two_digit.parse( begin, src.end() );
  EXPECT_TRUE( res4 );
  EXPECT_EQ( typeid(res4.get()), typeid(std::pair<int,int>) );
  EXPECT_EQ( res4.get().first, '5' );
  EXPECT_EQ( res4.get().second, '4' );
  EXPECT_EQ( begin, src.begin()+5 );

  auto res5 = parse_digit2.parse( begin, src.end() );
  EXPECT_FALSE( res5 );
  EXPECT_EQ( begin, src.end() );
}

TEST( ParserTest, Sequence )
{
  std::string src = "0123456789012";
  auto begin = src.begin();

  //                         int          int
  // new attribute will be tuple<int,int>
  auto two_digit_parser = ep::digit >> ep::digit;

  auto res = two_digit_parser.parse( begin, src.end() );
  EXPECT_TRUE( res );
  EXPECT_EQ( begin, src.begin()+2 );
  EXPECT_EQ( std::get<0>(res.get()), '0' );
  EXPECT_EQ( std::get<1>(res.get()), '1' );

  auto three_digit_parser = 
    ep::seq(
      ep::digit, ep::digit, ep::digit
    );
  auto res2 = three_digit_parser.parse( begin, src.end() );
  EXPECT_TRUE( res2 );
  EXPECT_EQ( begin, src.begin()+5 );
  EXPECT_EQ( std::get<0>(res2.get()), '2' );
  EXPECT_EQ( std::get<1>(res2.get()), '3' );
  EXPECT_EQ( std::get<2>(res2.get()), '4' );

  // tuple and tuple will be merge to new tuple
  auto five_digit_parser =
    two_digit_parser >> three_digit_parser;
  auto res5 = five_digit_parser.parse( begin, src.end() );
  EXPECT_TRUE( res5 );
  EXPECT_EQ( begin, src.begin()+10 );
  EXPECT_EQ( std::get<0>(res5.get()), '5' );
  EXPECT_EQ( std::get<1>(res5.get()), '6' );
  EXPECT_EQ( std::get<2>(res5.get()), '7' );
  EXPECT_EQ( std::get<3>(res5.get()), '8' );
  EXPECT_EQ( std::get<4>(res5.get()), '9' );

  // parse failed; only 3 characters left
  res5 = five_digit_parser.parse( begin, src.end() );
  EXPECT_FALSE( res5 );
  EXPECT_EQ( begin, src.begin()+10 );

  res2 = three_digit_parser.parse( begin, src.end() );
  EXPECT_TRUE( res2 );
  EXPECT_EQ( begin, src.end() );
  EXPECT_EQ( std::get<0>(res2.get()), '0' );
  EXPECT_EQ( std::get<1>(res2.get()), '1' );
  EXPECT_EQ( std::get<2>(res2.get()), '2' );


  begin = src.begin();

  // unused and int attribute -> int
  {
    auto unused_and_int = ep::digit.unused() >> ep::digit;
    auto res0 = unused_and_int.parse( begin, src.end() );
    EXPECT_TRUE( res0 );
    EXPECT_EQ( res0.get(), '1' );
  }

  // int and tuple -> tuple< int, *unpacked tuple >
  {
    auto int_and_tuple = ep::digit >> two_digit_parser;
    auto res0 = int_and_tuple.parse( begin, src.end() );
    EXPECT_TRUE( res0 );
    EXPECT_EQ( std::get<0>(res0.get()), '2' );
    EXPECT_EQ( std::get<1>(res0.get()), '3' );
    EXPECT_EQ( std::get<2>(res0.get()), '4' );
  }
}

int main( int argc, char** argv )
{
  testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
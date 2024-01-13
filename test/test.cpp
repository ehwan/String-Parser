#include "rules/dictionary.hpp"
#include "rules/stringmatch.hpp"
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

int main( int argc, char** argv )
{
  testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
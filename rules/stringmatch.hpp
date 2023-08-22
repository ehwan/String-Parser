#pragma once

#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// match one string
template < typename CharType >
struct string_match_t
  : base_t<string_match_t<CharType>>
{
  std::vector<CharType> string_;

  string_match_t( std::vector<CharType> const& string__ )
    : string_(string__)
  {
  }
  template < typename I >
  optional<unused_t> parse( I& begin , I end ) const
  {
    if( std::distance(begin,end) < string_.size() ){ return {false}; }

    bool ret = std::equal(string_.begin(), string_.end(), begin );
    if( ret )
    {
      std::advance( begin , string_.size() );
    }
    return {ret};
  }
};

}}}

namespace eh { namespace parser {

template < typename CharType >
rules::string_match_t<CharType> string_match( std::vector<CharType> const& string_ )
{
  return {string_};
}
template < typename I >
rules::string_match_t<typename std::iterator_traits<I>::value_type>
string_match( I begin, I end )
{
  return string_match( std::vector< typename std::iterator_traits<I>::value_type >( begin, end ) );
}

inline rules::string_match_t<char> string( char const *str )
{
  return string_match( str, str+std::strlen(str) );
}


}}

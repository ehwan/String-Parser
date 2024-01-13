#pragma once

#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// match one string
// Container sould have member method string_.begin(), string_.end()
template < typename Container >
struct string_match_t
  : base_t<string_match_t<Container>>
{
  Container string_;

  string_match_t( Container string__ )
    : string_(std::move(string__))
  {
  }
  template < typename I >
  parse_result_t<unused_t> parse( I& begin , I end ) const
  {
    I begin0 = begin;
    for( auto i=string_.begin(); i!=string_.end(); ++i )
    {
      if( begin == end || *begin != *i )
      {
        begin = begin0;
        return {false};
      }
      ++begin;
    }
    return {true};
  }
};

}}}

namespace eh { namespace parser {

template < typename CharType >
rules::string_match_t<std::vector<CharType>> string_match( std::vector<CharType> const& string_ )
{
  return {string_};
}
template < typename CharType >
rules::string_match_t<std::basic_string<CharType>> string_match( std::basic_string<CharType> const& string_ )
{
  return {string_};
}
template < typename I >
auto
string_match( I begin, I end )
{
  return string_match( std::vector< typename std::iterator_traits<I>::value_type >( begin, end ) );
}

#if __cplusplus >= 201700
template < typename CharType >
rules::string_match_t< std::basic_string_view<CharType> >
string_match( std::basic_string_view<CharType> string_view )
{
  return {string_view};
}
#endif


}}

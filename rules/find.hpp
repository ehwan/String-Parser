#pragma once

#include <iterator>
#include <algorithm>
#include <set>
#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

template < typename CharType >
struct find_t
  : base_t<find_t<CharType>>
{
  std::set<CharType> set_;

  find_t( std::set<CharType> s )
    : set_( std::move(s) )
  {
  }

  template < typename I >
  optional<typename std::iterator_traits<I>::value_type>
  parse( I& begin , I end ) const
  {
    if( begin != end && set_.find(*begin) != set_.end() )
    {
      return { true , *begin++ };
    }
    return {false};
  }
};

}}}

namespace eh { namespace parser {

template < typename C , typename I >
struct attribute< rules::find_t<C> , I >
{
  using type = typename std::iterator_traits<I>::value_type;
};

template < typename CharType >
rules::find_t<CharType> find( std::set<CharType> set )
{
  return { std::move(set) };
}


}}

#pragma once

#include <iterator>
#include <algorithm>
#include <set>
#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// find one character in set
template < typename CharType >
struct character_set_t
  : base_t<character_set_t<CharType>>
{
  std::set<CharType> set_;

  character_set_t( std::set<CharType> const& s )
    : set_(s)
  {
  }

  template < typename I >
  parse_result_t<typename std::iterator_traits<I>::value_type>
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
struct attribute< rules::character_set_t<C> , I >
{
  using type = typename std::iterator_traits<I>::value_type;
};

template < typename CharType >
rules::character_set_t<CharType> set_( std::set<CharType> const& set )
{
  return { set };
}
template < typename I >
rules::character_set_t< typename std::iterator_traits<I>::value_type >
set_( I begin, I end )
{
  return set_( std::set< typename std::iterator_traits<I>::value_type >(begin, end) );
}



}}

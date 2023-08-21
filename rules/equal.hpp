#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// match one string
template < typename Container >
struct equal_t 
  : base_t<equal_t<Container>>
{
  Container c;
  size_t size;

  equal_t( Container c_ )
    : c( std::move(c_) )
  {
    size = std::distance( std::begin(c) , std::end(c) );
  }
  template < typename I >
  optional<unused_t> parse( I& begin , I end ) const
  {
    if( std::distance(begin,end) < size ){ return {false}; }

    bool ret = std::equal(std::begin(c),std::end(c),begin);
    if( ret )
    {
      std::advance( begin , size );
    }
    return {ret};
  }
};

}}}

namespace eh { namespace parser {

template < typename Container >
rules::equal_t<Container> equal( Container c )
{
  return { std::move(c) };
}
inline rules::equal_t<std::string> string( char const *str )
{
  return { str };
}


}}

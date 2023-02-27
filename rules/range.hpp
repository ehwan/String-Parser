#pragma once

#include <iterator>
#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

template < typename T >
struct range_t
  : base_t<range_t<T>>
{
  T a , b;
  range_t( T a_ , T b_ )
    : a(std::move(a_)) , b(std::move(b_))
  {
  }
  template < typename I >
  optional<typename std::iterator_traits<I>::value_type>
  parse( I& begin , I end ) const
  {
    if( begin != end && a <= *begin && *begin <= b )
    {
      return {true , *begin++};
    }
    return {false};
  }


};

}}}

namespace eh { namespace parser {

template < typename T , typename I >
struct attribute< rules::range_t<T> , I >
{
  using type = typename std::iterator_traits<I>::value_type;
};
template < typename T >
rules::range_t<T> range( T a , T b )
{
  return { a , b };
}

}}

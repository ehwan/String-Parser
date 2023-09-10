#pragma once

#include <iterator>
#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// test if one character is in range [a,b] *Note* b is in-range
template < typename T >
struct range_t
  : base_t<range_t<T>>
{
  T min_ , max_;
  range_t( T min__ , T max__ )
    : min_(std::move(min__)) , max_(std::move(max__))
  {
  }
  template < typename I >
  parse_result_t<typename std::iterator_traits<I>::value_type>
  parse( I& begin , I end ) const
  {
    if( begin != end && min_ <= *begin && *begin <= max_ )
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
rules::range_t<T> range( T min_ , T max_ )
{
  return { min_ , max_ };
}

}}

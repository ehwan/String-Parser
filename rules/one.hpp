#pragma once

#include <iterator>
#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// test check( ch ) where ch is *begin ( one character )
template < typename F >
struct one_t
  : base_t<one_t<F>>
{
  F check;
  one_t( F c )
    : check( std::move(c) )
  {
  }
  template < typename I >
  parse_result_t<typename std::iterator_traits<I>::value_type>
  parse( I &begin , I end ) const
  {
    if( begin != end && check( *begin ) )
    {
      return { true , *begin++ };
    }
    return {false};
  }
};

template < typename F >
one_t<F> make_one( F f )
{
  return { f };
}

template < typename T >
struct single_check
{
  T data;

  single_check( T d )
    : data( std::move(d) )
  {
  }

  template < typename T2 >
  bool operator()( T2 c ) const
  {
    return c == data;
  }
};
struct any_check
{
  template < typename T >
  bool operator()( T t ) const
  {
    return true;
  }
};

}}}

namespace eh { namespace parser {

template < typename T , typename I >
struct attribute< rules::one_t<T> , I >
{
  using type = typename std::iterator_traits<I>::value_type;
};

// match one character's value is equal to one
template < typename T >
auto one( T value )
{
  return rules::make_one( rules::single_check<T>(value) );
}

}}

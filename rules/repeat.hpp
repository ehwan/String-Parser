#pragma once

#include <vector>
#include <type_traits>

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// for vector attribute specialization
template < typename T >
struct vector_attr;

template <>
struct vector_attr<unused_t>
{
  using type = unused_t;

  type data;
  void add( type t ){}
};
template < typename T >
struct vector_attr
{
  using type = std::vector<T>;

  type data;
  void add( T &&t )
  {
    data.emplace_back( std::move(t) );
  }
};

// repeat Parser N times
// where N \in [min_,max_]
template < typename P >
struct repeat_t 
  : base_t<repeat_t<P>>
{
  P p;
  unsigned int min_;
  unsigned int max_;

  repeat_t( P p_ , unsigned int mi_ , unsigned int ma_ )
    : p(std::move(p_)) ,
      min_(mi_) , max_(ma_)
  {
  }

  template < typename I >
  optional<
    typename vector_attr< typename attribute<P,I>::type >::type
  >
  parse( I &begin , I end ) const
  {
    vector_attr< typename attribute<P,I>::type > vec;
    unsigned int i = 0;
    I begin_ = begin;
    while( i < max_ )
    {
      auto r = p.parse(begin,end);
      if( r.is_valid() == false )
      {
        break;
      }
      vec.add( std::move(r.get()) );
      ++i;
    }

    if( min_ <= i )
    {
      return {true,std::move(vec.data)};
    }
    begin = begin_;
    return {false};
  }

};

}}}

namespace eh { namespace parser {

template < typename P , typename I >
struct attribute<rules::repeat_t<P>,I>
{
  using type = typename rules::vector_attr<
    typename attribute<P,I>::type
  >::type;
};

template < typename P >
rules::repeat_t<P>
repeat( P p , unsigned int min_ , unsigned int max_ )
{
  return { p , min_ , max_ };
}
template < typename P >
rules::repeat_t<P>
repeat( P p , unsigned int x )
{
  return { p , x , x };
}

}}

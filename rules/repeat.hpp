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
template < typename Parser >
struct repeat_t 
  : base_t<repeat_t<Parser>>
{
  Parser parser;
  unsigned int min_;
  unsigned int max_;

  repeat_t( Parser parser_ , unsigned int min__ , unsigned int max__ )
    : parser(std::move(parser_)) ,
      min_(min__) , max_(max__)
  {
  }

  template < typename Iterator >
  optional<
    typename vector_attr< typename attribute<Parser,Iterator>::type >::type
  >
  parse( Iterator &begin , Iterator end ) const
  {
    vector_attr< typename attribute<Parser,Iterator>::type > vec;
    unsigned int i = 0;
    Iterator begin_ = begin;
    while( i < max_ )
    {
      auto r = parser.parse(begin,end);
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

template < typename Parser >
rules::repeat_t<Parser>
repeat( Parser parser , unsigned int min_ , unsigned int max_ )
{
  return { parser , min_ , max_ };
}
template < typename Parser >
rules::repeat_t<Parser>
repeat( Parser parser , unsigned int count )
{
  return { parser , count , count };
}

}}

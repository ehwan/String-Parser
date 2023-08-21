#pragma once

#include <utility>
#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// return matched iterator range as tuple< begin, end >
template < typename P >
struct itpair_t
  : base_t<itpair_t<P>>
{
  P p;

  itpair_t( P p_ )
    : p(std::move(p_))
  {
  }

  template < typename I >
  optional< std::tuple<I,I> >
  parse( I &begin , I end ) const
  {
    I begin_ = begin;
    if( p.parse(begin,end) )
    {
      return { true , std::tuple<I,I>(begin_,begin) };
    }
    return { false };
  }
};

}}}

namespace eh { namespace parser {

template < typename P , typename I >
struct attribute<rules::itpair_t<P>,I>
{
  using type = std::tuple<I,I>;
};

template < typename P >
rules::itpair_t<P>
itpair( P p )
{
  return { p };
}

}}

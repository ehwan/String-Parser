#pragma once

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// Test with parser, but do not consume iterator
template < typename P >
struct notconsume_t
  : base_t<notconsume_t<P>>
{
  P p;

  notconsume_t( P p_ )
    : p(static_cast<P&&>(p_))
  {
  }

  template < typename I >
  parse_result_t< typename attribute<P,I>::type >
  parse( I& begin , I end ) const
  {
    I begin_ = begin;
    return p.parse(begin_,end);
  }
};

}}}

namespace eh { namespace parser {

template < typename P , typename I >
struct attribute< rules::notconsume_t<P> , I >
{
  using type = typename attribute<P,I>::type;
};

template < typename P >
rules::notconsume_t<P> notconsume( P p )
{
  return { static_cast<P&&>(p) };
}

}}

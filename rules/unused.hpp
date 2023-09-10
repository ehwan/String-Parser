#pragma once

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// make attribute unused_t
template < typename P >
struct attrunused_t
  : base_t<attrunused_t<P>>
{
  P p;
  attrunused_t( P p_ )
    : p( static_cast<P&&>(p_) )
  {
  }
  template < typename I >
  parse_result_t<unused_t> parse( I& begin , I end ) const
  {
    return { p.parse(begin,end).is_valid() };
  }

};

}}}

namespace eh { namespace parser {

template < typename P , typename I >
struct attribute< rules::attrunused_t<P> , I >
{
  using type = unused_t;
};
template < typename P >
rules::attrunused_t<P> unused( P p )
{
  return { p };
}

}}

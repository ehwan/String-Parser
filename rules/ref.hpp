#pragma once

#include "../base.hpp"
#include <memory>

namespace eh { namespace parser { namespace rules {

// reference wrapper for Parser Object
template < typename P >
struct ref_t
  : base_t<ref_t<P>>
{
  P const *p;

  ref_t( P const *p_ )
    : p(p_)
  {
  }

  template < typename I >
  auto parse( I &begin , I end ) const
  {
    return p->parse(begin,end);
  }

};

}}}

namespace eh { namespace parser {

template < typename P , typename I >
struct attribute< rules::ref_t<P> , I >
{
  using type = typename attribute<P,I>::type;
};
template < typename P >
rules::ref_t<P> ref_( P const& p )
{
  return { std::addressof(p) };
}

}}

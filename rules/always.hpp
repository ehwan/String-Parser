#pragma once

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// always return 'd' as parse result
// 'always-true-parser' can be used with 'or_' 
// to make 'optional' parser ( '?' for REGEX )
struct always_t
  : base_t<always_t>
{
  bool d;
  always_t( bool x )
    : d(x)
  {
  }
  template < typename I >
  optional<unused_t>
  parse( I& begin , I end ) const
  {
    return {d};
  }
};

}}}

namespace eh { namespace parser {

template < typename I >
struct attribute< rules::always_t , I >
{
  using type = unused_t;
};
using always_t = rules::always_t;
inline rules::always_t always( bool x )
{
  return { x };
}

}}

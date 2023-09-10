#pragma once

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// always return 'd' as parse result
// 'always-true-parser' can be used with 'or_' 
// to make 'optional' parser ( '?' for REGEX )
struct always_t
  : base_t<always_t>
{
  bool parse_result;

  always_t( bool parse_result_ )
    : parse_result(parse_result_)
  {
  }
  template < typename Iterator >
  parse_result_t<unused_t>
  parse( Iterator& begin , Iterator end ) const
  {
    return {parse_result};
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
inline rules::always_t always( bool parse_result )
{
  return { parse_result };
}

}}

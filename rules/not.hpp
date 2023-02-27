#pragma once

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

template < typename PA , typename PB >
struct not_t
  : base_t<not_t<PA,PB>>
{
  PA pa;
  PB pb;

  not_t( PA pa_ , PB pb_ )
    : pa( static_cast<PA&&>(pa_) ),
      pb( static_cast<PB&&>(pb_) )
  {
  }

  template < typename I >
  optional< typename attribute<PA,I>::type >
  parse( I& begin , I end ) const
  {
    I begin_ = begin;
    I beginb = begin;
    if( pb.parse(beginb,end).is_valid() == false )
    {
      if( auto r = pa.parse(begin,end) )
      {
        return r;
      }
    }
    return {false};
  }
};

}}}

namespace eh { namespace parser {

template < typename PA , typename PB , typename I >
struct attribute< rules::not_t<PA,PB> , I >
{
  using type = typename attribute<PA,I>::type;
};

template < typename PA , typename PB >
rules::not_t<PA,PB> not_( PA pa , PB pb )
{
  return { static_cast<PA&&>(pa) , static_cast<PB&&>(pb) };
}

}}

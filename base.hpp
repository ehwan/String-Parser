#pragma once

#include <type_traits>
#include "optional.hpp"

namespace eh { namespace parser { namespace rules {

struct always_t;

template < typename P , typename F >
struct action_t;

template < typename PA , typename PB >
struct or_t;

template < typename PA , typename PB >
struct not_t;

template < typename P >
struct repeat_t;

template < typename PA , typename PB >
struct seq_t;

template < typename P >
struct attrunused_t;

template < typename P >
struct notconsume_t;

template < typename P >
struct itpair_t;

template < typename P >
struct ref_t;


template < typename CRTP >
struct base_t
{
  using this_type = base_t<CRTP>;
  using child_type = CRTP;

  child_type& as_child()
  {
    return static_cast<child_type&>(*this);
  }
  child_type const& as_child() const
  {
    return static_cast<child_type const&>(*this);
  }

  repeat_t<CRTP> operator* () const;
  repeat_t<CRTP> operator+ () const;

  template < typename F >
  decltype(auto)
  operator[] ( F &&f ) const;

  template < typename P >
  decltype(auto)
  operator| ( P p ) const;

  template < typename P >
  decltype(auto)
  operator>> ( P p ) const;

  template < typename P >
  decltype(auto)
  operator- ( P p ) const;

  template < typename T >
  decltype(auto) operator()( T x ) const;
  template < typename T >
  decltype(auto) attr( T x ) const;
  decltype(auto) operator()() const;
  decltype(auto) pair() const;
  ref_t<CRTP> ref() const;

  attrunused_t<CRTP> unused() const;
  notconsume_t<CRTP> notconsume() const;

  template < typename T >
  decltype(auto) construct() const;
};

}}}

namespace eh { namespace parser {

template < typename Parser , typename Iterator >
struct attribute
{
  static auto invoke( Parser *p , Iterator *i )
  {
    Iterator i_ = *i;
    return p->parse( i_ , i_ );
  }
  using type = typename decltype(invoke(nullptr,nullptr))::type;
};

}}

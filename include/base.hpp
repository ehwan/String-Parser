#pragma once

#include <type_traits>
#include "parse_result.hpp"

#if __cplusplus >= 201500
#include <string_view>
#endif


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


// base CRTP Parser Object class
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

  // REGEX '*' operator
  // repeat 0 or more times
  repeat_t<CRTP> operator* () const;

  // REGEX '+' operator
  // repeat 1 or more times
  repeat_t<CRTP> operator+ () const;

  // operator for action( this, f )
  template < typename Functor >
  decltype(auto)
  operator[] ( Functor &&functor ) const;

  // operator for or_( *this, parser )
  template < typename Parser >
  decltype(auto)
  operator| ( Parser parser ) const;

  // operator for seq( *this, p )
  template < typename Parser >
  decltype(auto)
  operator>> ( Parser parser ) const;

  // operator for not_( *this, p )
  template < typename Parser >
  decltype(auto)
  operator- ( Parser parser ) const;

  // same as attr()
  // make its attrubute as new_attr
  template < typename NewAttr >
  decltype(auto) operator()( NewAttr new_attr ) const;

  // make its attribute as x
  template < typename NewAttr >
  decltype(auto) attr( NewAttr new_attr ) const;

  // same as unused()
  // make its attribute as unused_t
  decltype(auto) operator()() const;

// Its attribute will be tuple (begin,end) that parse() consumed
  decltype(auto) pair() const;

// Its attribute will be tuple (begin,end) that parse() consumed
  decltype(auto) view() const;

  // return itself as reference
  // same usage as std::ref
  ref_t<CRTP> ref() const;

  // make its attribute as unused_t
  attrunused_t<CRTP> unused() const;

  // make parse() does not consume iterator
  notconsume_t<CRTP> notconsume() const;

  // call constructor of T as functor
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

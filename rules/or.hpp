#pragma once

#include <type_traits>
#include <utility>

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

template < typename A , typename B >
struct or_merge;

template <>
struct or_merge<unused_t,unused_t>
{
  using type = unused_t;

  static type merge( unused_t )
  {
    return {};
  }
};
template < typename A >
struct or_merge<A,unused_t>
{
  using type = A;

  static type merge( A a )
  {
    return a;
  }
  static type merge( unused_t )
  {
    return {};
  }
};
template < typename B >
struct or_merge<unused_t,B>
{
  using type = B;

  static type merge( unused_t )
  {
    return {};
  }
  static type merge( B b )
  {
    return b;
  }
};
template < typename A , typename B >
struct or_merge
{
  using type = unused_t;

  static type merge( A a )
  {
    return {};
  }
  static type merge( B b )
  {
    return {};
  }
};
template < typename A >
struct or_merge<A,A>
{
  using type = A;

  static type merge( A a )
  {
    return a;
  }
};

template < typename A , typename B >
struct or_t
  : base_t<or_t<A,B>>
{
  int cached = -1;
  A a;
  B b;

  or_t( A a_ , B b_ )
    : a(std::move(a_)) , b(std::move(b_))
  {
  }

  template < typename I >
  optional< typename or_merge<
    typename attribute<A,I>::type , typename attribute<B,I>::type
  >::type >
  parse( I &begin , I end ) const
  {
    if( auto r1 = a.parse(begin,end) )
    {
      return { true , or_merge<
        typename attribute<A,I>::type , typename attribute<B,I>::type
      >::merge( std::move(r1.get()) ) };
    }
    if( auto r2 = b.parse(begin,end) )
    {
      return { true , or_merge<
        typename attribute<A,I>::type , typename attribute<B,I>::type
      >::merge( std::move(r2.get()) ) };
    }
    return {false};
  }
};

}}}

namespace eh { namespace parser {

template < typename A , typename B , typename I >
struct attribute< rules::or_t<A,B> , I >
{
  using type = typename rules::or_merge<
    typename attribute<A,I>::type , typename attribute<B,I>::type
  >::type;
};

template < typename A , typename B >
rules::or_t<A,B> or_( A a , B b )
{
  return { a , b };
}
template < typename A , typename B , typename C , typename ... Ps >
auto or_( A a , B b , C c , Ps ... ps )
{
  return or_( a ,
      or_( b , c , ps... )
  );
}

}}

#pragma once

#include <tuple>
#include <type_traits>

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {


// for specialization of tuple Attributes
template < typename A , typename B >
struct tuple_merge;

// both Attributes are unused_t -> result Attribute is unused_t
template <>
struct tuple_merge<unused_t,unused_t>
{
  using type = unused_t;

  static type merge( unused_t , unused_t )
  {
    return {};
  }
};

// one of Attributes is unused_t -> result Attribute is the normal one
template < typename B >
struct tuple_merge<unused_t,B>
{
  using type = B;

  static type merge( unused_t , B b )
  {
    return b;
  }
};
template < typename A >
struct tuple_merge<A,unused_t>
{
  using type = A;

  static type merge( A a , unused_t )
  {
    return a;
  }
};
template < typename ... Bs >
struct tuple_merge<unused_t,std::tuple<Bs...>>
{
  using type = std::tuple<Bs...>;

  static type merge( unused_t , std::tuple<Bs...> b )
  {
    return b;
  }
};
template < typename ... As >
struct tuple_merge<std::tuple<As...>,unused_t>
{
  using type = std::tuple<As...>;

  static type merge( std::tuple<As...> a , unused_t )
  {
    return a;
  }
};


// two non-unused_t and non-tuple data
// concat them as tuple
template < typename A , typename B >
struct tuple_merge
{
  using type = std::tuple<A,B>;

  static type merge( A a , B b )
  {
    return std::make_tuple( std::move(a) , std::move(b) );
  }
};

// one normal type and one tuple
// concat to tuple
template < typename A , typename ... Bs >
struct tuple_merge<A,std::tuple<Bs...>>
{
  using type = std::tuple<A,Bs...>;

  static type merge( A a , std::tuple<Bs...> b )
  {
    return std::tuple_cat( std::make_tuple(std::move(a)) , std::move(b) );
  }
};

// one normal type and one tuple
// concat to tuple
template < typename ... As , typename B >
struct tuple_merge<std::tuple<As...>,B>
{
  using type = std::tuple<As...,B>;

  static type merge( std::tuple<As...> a , B b )
  {
    return std::tuple_cat(
        std::move(a) , std::make_tuple(std::move(b))
    );
  }
};


// tuple and tuple
// concat tuples
template < typename ... As , typename ... Bs >
struct tuple_merge<std::tuple<As...>,std::tuple<Bs...>>
{
  using type = std::tuple<As...,Bs...>;

  static type merge( std::tuple<As...> a , std::tuple<Bs...> b )
  {
    return std::tuple_cat( std::move(a) , std::move(b) );
  }
};

template < typename PA , typename PB >
struct seq_t
  : base_t<seq_t<PA,PB>>
{
  PA pa;
  PB pb;

  seq_t( PA a , PB b )
    : pa(std::move(a)) , pb(std::move(b))
  {
  }

  template < typename I , typename PA_ = PA , typename PB_ = PB >
  optional<
    typename tuple_merge<
      typename attribute<PA_,I>::type , typename attribute<PB_,I>::type
    >::type
  >
  parse( I &begin , I end ) const
  {
    I begin_ = begin;
    if( auto r1 = pa.parse(begin,end) )
    {
      if( auto r2 = pb.parse(begin,end) )
      {
        return { true ,
          tuple_merge<
          typename attribute<PA_,I>::type , typename attribute<PB_,I>::type
        >::merge( std::move(r1.get()) , std::move(r2.get()) )
        };
      }
      begin = begin_;
    }
    return {false};
  }

};

}}}

namespace eh { namespace parser {

template < typename PA , typename PB , typename I >
struct attribute<rules::seq_t<PA,PB>,I>
{
  using type = typename rules::tuple_merge<
    typename attribute<PA,I>::type , typename attribute<PB,I>::type
  >::type;
};

template < typename PA , typename PB >
rules::seq_t<PA,PB>
seq( PA pa , PB pb )
{
  return { pa , pb };
}
template < typename PA , typename PB , typename PC , typename ... Ps >
auto seq( PA pa , PB pb , PC pc , Ps ... ps )
{
  return seq( pa ,
      seq( pb , pc , ps... )
  );
}

}}

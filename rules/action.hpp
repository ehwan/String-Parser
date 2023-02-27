#pragma once

#include <type_traits>
#include <functional>
#include <utility>
#include <tuple>

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

template < typename F , typename T >
struct action_invoke;


template < typename F >
struct action_invoke<F,unused_t>
{
  // F()
  using result_type =
    typename std::decay<typename std::result_of<F()>::type>::type;

  template < typename Result >
  struct shell;

  template <>
  struct shell<void>
  {
    using type = unused_t;

    static type invoke( F f )
    {
      f();
      return {};
    }
  };
  template < typename Res >
  struct shell
  {
    using type = Res;

    static type invoke( F f )
    {
      return f();
    }
  };


  using type = typename shell<result_type>::type;
  static type invoke( F f , unused_t )
  {
    return shell<result_type>::invoke( f );
  }
};
template < typename F , typename T >
struct action_invoke
{
  // F( t )
  using result_type =
    typename std::decay<typename std::result_of<F(T&)>::type>::type;
  
  template < typename Result >
  struct shell;

  template <>
  struct shell<void>
  {
    using type = unused_t;

    static type invoke( F f , T &t )
    {
      f( t );
      return {};
    }
  };
  template < typename Res >
  struct shell
  {
    using type = Res;

    static type invoke( F f , T &t )
    {
      return f( t );
    }
  };


  using type = typename shell<result_type>::type;
  static type invoke( F f , T &t )
  {
    return shell<result_type>::invoke( f , t );
  }
};
template < typename F , typename ... Ts >
struct action_invoke<F,std::tuple<Ts...>>
{
  using tuple_type = std::tuple<Ts...>;
  // F( Ts... )
  using result_type =
    typename std::decay<typename std::result_of<F(Ts&...)>::type>::type;
  
  template < typename Result >
  struct shell;

  template <>
  struct shell<void>
  {
    using type = unused_t;

    template < size_t ... Is >
    static type invoke( F f , tuple_type &t ,
        std::index_sequence<Is...> )
    {
      f( std::get<Is>(t)... );
      return {};
    }
  };
  template < typename Res >
  struct shell
  {
    using type = Res;

    template < size_t ... Is >
    static type invoke( F f , tuple_type &t ,
        std::index_sequence<Is...> )
    {
      return f( std::get<Is>(t)... );
    }
  };


  using type = typename shell<result_type>::type;
  static type invoke( F f , tuple_type &t )
  {
    return shell<result_type>::invoke( f , t ,
        std::index_sequence_for<Ts...>{} );
  }
};

template < typename P , typename F >
struct action_t
  : base_t<action_t<P,F>>
{
  P p;
  F f;

  action_t( P p_ , F f_ )
    : p(std::move(p_)) , f(std::move(f_))
  {
  }

  template < typename I >
  optional<
    typename action_invoke<F,typename attribute<P,I>::type>::type
  >
  parse( I &begin , I end ) const
  {
    if( auto ret = p.parse(begin,end) )
    {
      return { true ,
        action_invoke<F,typename attribute<P,I>::type>::invoke(f,ret.get()) };
    }
    return { false };
  }
};

}}}

namespace eh { namespace parser {

template < typename P , typename F , typename I >
struct attribute<rules::action_t<P,F>,I>
{
  using type = typename rules::action_invoke<
    F , typename attribute<P,I>::type
  >::type;
};

template < typename P , typename F >
rules::action_t<P,typename std::decay<F>::type>
action( P p , F &&f )
{
  return { p , static_cast<F&&>(f) };
}


}}

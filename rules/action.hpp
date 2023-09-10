#pragma once

#include <type_traits>
#include <functional>
#include <utility>
#include <tuple>

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// for specialization on invoking action
// with unused_t, tuple<...>, normal data
template < typename F , typename T >
struct action_invoke;


// for unused_t
template < typename F >
struct action_invoke<F,unused_t>
{
  // action does not take any arguments

  // 'result_of' will be deprecated in c++17
#if __cplusplus < 201500
  using result_type =
    typename std::decay<typename std::result_of<F()>::type>::type;
#else
  using result_type =
    typename std::decay< typename std::invoke_result<F>::type >::type;
#endif

  // template specialized helper class for ( void or T ) returned type
  template < typename Result >
  struct shell;

  // if return_type of action is void, make it unused_t
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

// for normal data
template < typename F , typename T >
struct action_invoke
{
  // F( t ), action takes only one data as argument

  // 'result_of' will be deprecated in c++17
#if __cplusplus < 201500
  using result_type =
    typename std::decay<typename std::result_of<F(T&)>::type>::type;
#else
  using result_type =
    typename std::decay< typename std::invoke_result<F,T&>::type >::type;
#endif

  
  // template specialized helper class for ( void or T ) returned type
  template < typename Result >
  struct shell;

  // if return_type of action is void, make it unused_t
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

// for tuple
template < typename F , typename ... Ts >
struct action_invoke<F,std::tuple<Ts...>>
{
  using tuple_type = std::tuple<Ts...>;
  // F( Ts... ), action takes unpacked tuple as arguments

#if __cplusplus < 201500
  using result_type =
    typename std::decay<typename std::result_of<F(Ts&...)>::type>::type;
#else
  using result_type =
    typename std::decay< typename std::invoke_result<F,Ts&...>::type >::type;
#endif
  
  template < typename Result >
  struct shell;

  // if return_type of action is void, make it unused_t
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

// Action Wrapper
// if Parse test success, Functor() will be called
template < typename Parser , typename Functor >
struct action_t
  : base_t<action_t<Parser,Functor>>
{
  Parser parser;
  Functor functor;

  action_t( Parser parser__ , Functor functor__ )
    : parser(std::move(parser__)) , functor(std::move(functor__))
  {
  }

  template < typename Iterator >
  parse_result_t<
    typename action_invoke<Functor,typename attribute<Parser,Iterator>::type>::type
  >
  parse( Iterator &begin , Iterator end ) const
  {
    if( auto ret = parser.parse(begin,end) )
    {
      return { true ,
        action_invoke<Functor,typename attribute<Parser,Iterator>::type>::invoke(functor,ret.get()) };
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

template < typename Parser , typename Functor >
rules::action_t<Parser,typename std::decay<Functor>::type>
action( Parser parser , Functor &&functor )
{
  return { parser , static_cast<Functor&&>(functor) };
}


}}

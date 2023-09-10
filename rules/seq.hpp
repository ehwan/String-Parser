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

// test Parser A, then Parser B
template < typename ParserA , typename ParserB >
struct seq_t
  : base_t<seq_t<ParserA,ParserB>>
{
  ParserA parsera;
  ParserB parserb;

  seq_t( ParserA parsera_ , ParserB parserb_ )
    : parsera(std::move(parsera_)) , parserb(std::move(parserb_))
  {
  }

  template < typename Iterator , typename PA_ = ParserA , typename PB_ = ParserB >
  parse_result_t<
    typename tuple_merge<
      typename attribute<PA_,Iterator>::type , typename attribute<PB_,Iterator>::type
    >::type
  >
  parse( Iterator &begin , Iterator end ) const
  {
    Iterator begin_ = begin;
    if( auto r1 = parsera.parse(begin,end) )
    {
      if( auto r2 = parserb.parse(begin,end) )
      {
        return { true ,
          tuple_merge<
          typename attribute<PA_,Iterator>::type , typename attribute<PB_,Iterator>::type
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

template < typename ParserA , typename ParserB >
rules::seq_t<ParserA,ParserB>
seq( ParserA parser_a , ParserB parser_b )
{
  return { parser_a , parser_b };
}
template < typename ParserA , typename ParserB , typename ParserC , typename ... Parsers >
auto seq( ParserA parser_a , ParserB parser_b , ParserC parser_c , Parsers ... parsers )
{
  return seq( parser_a ,
      seq( parser_b , parser_c , parsers... )
  );
}

}}

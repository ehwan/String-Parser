#pragma once

#include <type_traits>
#include <utility>

#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// Attribute specialization for Or Attributes
// Possible usage of Variant, but not now
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

// test if A success first,
// then test B ( if A failed )
template < typename ParserA , typename ParserB >
struct or_t
  : base_t<or_t<ParserA,ParserB>>
{
  ParserA parsera;
  ParserB parserb;

  or_t( ParserA parsera_ , ParserB parserb_ )
    : parsera(std::move(parsera_)) , parserb(std::move(parserb_))
  {
  }

  template < typename Iterator >
  parse_result_t< typename or_merge<
    typename attribute<ParserA,Iterator>::type , typename attribute<ParserB,Iterator>::type
  >::type >
  parse( Iterator &begin , Iterator end ) const
  {
    if( auto r1 = parsera.parse(begin,end) )
    {
      return { true , or_merge<
        typename attribute<ParserA,Iterator>::type , typename attribute<ParserB,Iterator>::type
      >::merge( std::move(r1.get()) ) };
    }
    if( auto r2 = parserb.parse(begin,end) )
    {
      return { true , or_merge<
        typename attribute<ParserA,Iterator>::type , typename attribute<ParserB,Iterator>::type
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

template < typename ParserA , typename ParserB >
rules::or_t<ParserA,ParserB> or_( ParserA parser_a , ParserB parser_b )
{
  return { parser_a , parser_b };
}
template < typename ParserA , typename ParserB , typename ParserC , typename ... Parsers >
auto or_( ParserA parser_a , ParserB parser_b , ParserC parser_c , Parsers ... parsers )
{
  return or_( parser_a ,
      or_( parser_b , parser_c , parsers... )
  );
}

}}

#pragma once

#include <vector>
#include "base.hpp"

namespace eh { namespace parser {

template < typename Attr , typename Iterator >
struct token_t
{
  using value_type = Attr;
  using this_type = token_t<Attr,Iterator>;

  Iterator begin, end;
  Attr data;

  operator Attr() const
  {
    return data;
  }
  bool operator == ( this_type const& rhs ) const
  {
    return data == rhs.type;
  }
  bool operator == ( Attr t ) const
  {
    return data == t;
  }
  bool operator != ( this_type const& rhs ) const
  {
    return data != rhs.type;
  }
  bool operator != ( Attr t ) const
  {
    return data != t;
  }
};

template <
  typename Parser ,
  typename White ,
  typename Iterator >
auto
tokenize( Parser p , White w , Iterator begin , Iterator end )
{
  // attribute of p is Type
  // attribute of w is free
  using attr_type = typename attribute<Parser,Iterator>::type;
  using token_type = token_t<attr_type,Iterator>;

  struct 
  {
    std::vector<token_type> vec;

    unused_t operator()( attr_type t )
    {
      vec.emplace_back();
      vec.back().data = t;
      return {};
    }
    unused_t operator()( Iterator begin , Iterator end )
    {
      vec.back().begin = begin;
      vec.back().end = end;
      return {};
    }
  } token_set;

  auto paction = p[ std::ref(token_set) ].pair()[ std::ref(token_set) ];
  do
  {
    // skip white spaces
    while( w.parse(begin,end) ){}
  } while( paction.parse(begin,end) );

  return token_set.vec;
}

}}

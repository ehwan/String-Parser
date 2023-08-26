#pragma once

#include <utility>
#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// return matched iterator range as tuple< begin, end >
template < typename Parser >
struct itpair_t
  : base_t<itpair_t<Parser>>
{
  Parser parser;

  itpair_t( Parser parser_ )
    : parser(std::move(parser_))
  {
  }

  template < typename Iterator >
  optional< std::tuple<Iterator,Iterator> >
  parse( Iterator &begin , Iterator end ) const
  {
    Iterator begin_ = begin;
    if( parser.parse(begin,end) )
    {
      return { true , std::tuple<Iterator,Iterator>(begin_,begin) };
    }
    return { false };
  }
};

}}}

namespace eh { namespace parser {

template < typename P , typename I >
struct attribute<rules::itpair_t<P>,I>
{
  using type = std::tuple<I,I>;
};

template < typename Parser >
rules::itpair_t<Parser>
itpair( Parser parser )
{
  return { parser };
}

}}

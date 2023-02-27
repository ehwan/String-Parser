#pragma once

#include <functional>

#include "rule.hpp"
#include "rules/one.hpp"
#include "rules/range.hpp"
#include "rules/seq.hpp"
#include "rules/action.hpp"
#include "rules/itpair.hpp"
#include "rules/or.hpp"
#include "rules/repeat.hpp"
#include "rules/equal.hpp"
#include "rules/ref.hpp"
#include "rules/unused.hpp"
#include "rules/always.hpp"
#include "rules/not.hpp"
#include "rules/notconsume.hpp"
#include "rules/find.hpp"
#include "rules/dictionary.hpp"

#include "token.hpp"

/*
 * iterator usage :
 *    ++i
 *    i++
 *    *i
 *    i2 = i
 *    i != end
 *    i == end
 *    distance( i , i2 )
 *    equal( i , i2 , b )
 *    advance( i , n )
 */

namespace eh { namespace parser { namespace suffix {
  inline auto operator"" _p( char x )
  {
    return one( x );
  }
  inline rules::equal_t<std::string> operator"" _p( char const *str , size_t l )
  {
    return { std::string(str,l) };
  }
  inline rules::find_t<char> operator"" _pone( char const *str , size_t l )
  {
    std::set<char> set{ str , str + l };
    return { std::move(set) };
  }
}}}

using namespace eh::parser::suffix;

namespace eh { namespace parser {

template < typename CRTP >
rules::repeat_t<CRTP> rules::base_t<CRTP>::operator* () const
{
  return { as_child() , 0 , (unsigned)-1 };
}
template < typename CRTP >
rules::repeat_t<CRTP> rules::base_t<CRTP>::operator+ () const
{
  return { as_child() , 1 , (unsigned)-1 };
}
template < typename CRTP >
template < typename P >
decltype(auto)
rules::base_t<CRTP>::operator| ( P p ) const
{
  return or_( as_child(), p );
}
template < typename CRTP >
template < typename P >
decltype(auto)
rules::base_t<CRTP>::operator>> ( P p ) const
{
  return seq( as_child(), p );
}
template < typename CRTP >
template < typename P >
decltype(auto)
rules::base_t<CRTP>::operator- ( P p ) const
{
  return not_( as_child(), p );
}
template < typename CRTP >
rules::attrunused_t<CRTP> rules::base_t<CRTP>::unused() const
{
  return { as_child() };
}
template < typename CRTP >
rules::notconsume_t<CRTP> rules::base_t<CRTP>::notconsume() const
{
  return { as_child() };
}
template < typename CRTP >
template < typename F >
decltype(auto)
rules::base_t<CRTP>::operator[]( F &&f ) const
{
  return action( as_child(), static_cast<F&&>(f) );
}
template < typename CRTP >
template < typename T >
decltype(auto)
rules::base_t<CRTP>::operator()( T x ) const
{
  struct return_this_t
  {
    T x;

    T operator()() const
    {
      return x;
    }
  };

  return action( unused() , return_this_t{x} );
}
template < typename CRTP >
template < typename T >
decltype(auto)
rules::base_t<CRTP>::attr( T x ) const
{
  struct return_this_t
  {
    T x;

    T operator()() const
    {
      return x;
    }
  };

  return action( unused() , return_this_t{x} );
}
template < typename CRTP >
decltype(auto)
rules::base_t<CRTP>::operator()() const
{
  return unused();
}
template < typename CRTP >
decltype(auto)
rules::base_t<CRTP>::pair() const
{
  return itpair( unused() );
}
template < typename CRTP >
rules::ref_t<CRTP>
rules::base_t<CRTP>::ref() const
{
  return { std::addressof(as_child()) };
}
template < typename T >
struct constructor
{
  T operator()() const
  {
    return T();
  }
  template < typename T2 >
  T operator()( T2 &&t ) const
  {
    return T(t);
  }
};
template < typename CRTP >
template < typename T >
decltype(auto)
rules::base_t<CRTP>::construct() const
{
  return action( as_child(), constructor<T>() );
}

const auto any = rules::make_one( rules::any_check{} );
const auto lower = range( 'a' , 'z' );
const auto upper = range( 'A' , 'Z' );
const auto alpha = or_( lower , upper );
const auto alpha_ = or_( one('_') , alpha );
const auto digit = range( '0' , '9' );
const auto digit0 = range( '1' , '9' );
const auto ident =
  alpha_.unused()
  >> *(alpha_.unused() | digit.unused());
const auto true_ = rules::always_t{true};
const auto false_ = rules::always_t{false};

}}

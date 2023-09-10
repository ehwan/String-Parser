#pragma once

#include "unused.hpp"

namespace eh { namespace parser {

// return type of parse() function
template < typename Attribute >
struct parse_result_t
{
  using type = Attribute;

  bool parse_result;
  Attribute data;

  parse_result_t( bool parse_result_ , Attribute data_ )
    : parse_result( parse_result_ ), data( static_cast<Attribute&&>(data_) )
  {
  }

  // default construct Attribute
  parse_result_t( bool parse_result_ )
    : parse_result(parse_result_)
  {
  }

  operator bool() const { return parse_result; }
  bool is_valid() const{ return parse_result; }
  Attribute& get()
  {
    return data;
  }
  Attribute const& get() const
  {
    return data;
  }
  Attribute& operator*()
  {
    return data;
  }
  Attribute const& operator*() const
  {
    return data;
  }
};
template <>
struct parse_result_t<unused_t>
{
  using type = unused_t;

  bool parse_result;

  parse_result_t( bool parse_result_ , unused_t )
    : parse_result(parse_result_)
  {
  }
  parse_result_t( bool parse_result_ )
    : parse_result(parse_result_)
  {
  }
  operator bool() const{ return parse_result; }
  bool is_valid() const{ return parse_result; }
  unused_t get() const
  {
    return {};
  }
  unused_t operator*() const
  {
    return {};
  }
};

}}

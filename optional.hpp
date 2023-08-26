#pragma once

#include "unused.hpp"

namespace eh { namespace parser {

// return type of parse() function
template < typename Attribute >
struct optional
{
  using type = Attribute;

  bool parse_result;
  Attribute data;

  optional( bool parse_result_ , Attribute data_ )
    : parse_result( parse_result_ ), data( static_cast<Attribute&&>(data_) )
  {
  }

  // default construct Attribute
  optional( bool parse_result_ )
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
};
template <>
struct optional<unused_t>
{
  using type = unused_t;

  bool parse_result;

  optional( bool parse_result_ , unused_t )
    : parse_result(parse_result_)
  {
  }
  optional( bool parse_result_ )
    : parse_result(parse_result_)
  {
  }
  operator bool() const{ return parse_result; }
  bool is_valid() const{ return parse_result; }
  unused_t get() const
  {
    return {};
  }
};

}}

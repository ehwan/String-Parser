#pragma once

#include "unused.hpp"

namespace eh { namespace parser {

template < typename T >
struct optional
{
  using type = T;
  bool b;
  T data;

  optional( bool b_ , T d )
    : b(b_), data(static_cast<T&&>(d))
  {
  }
  optional( bool b_ )
    : b(b_)
  {
  }

  operator bool() const { return b; }
  bool is_valid() const{ return b; }
  T& get()
  {
    return data;
  }
  T const& get() const
  {
    return data;
  }
};
template <>
struct optional<unused_t>
{
  using type = unused_t;
  bool b;

  optional( bool b_ , unused_t )
    : b(b_)
  {
  }
  optional( bool b_ )
    : b(b_)
  {
  }
  operator bool() const{ return b; }
  bool is_valid() const{ return b; }
  unused_t get() const
  {
    return {};
  }
};

}}

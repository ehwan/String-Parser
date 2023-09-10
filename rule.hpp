#pragma once

#include <memory>
#include "base.hpp"
#include "rules/seq.hpp"

namespace eh { namespace parser {

// virtual class version of template PARSER
template < typename Attribute, typename Iterator >
class rule
  : public rules::base_t<rule<Attribute,Iterator>>
{
  struct base_t
  {
    virtual ~base_t(){}
    virtual base_t* clone() const = 0;
    virtual parse_result_t<Attribute> parse( Iterator &begin , Iterator end ) const = 0;
  };

  template < typename P >
  struct child_t
    : base_t
  {
    using this_type = child_t<P>;

    P p;

    child_t( P p_ )
      : p( static_cast<P&&>(p_) )
    {
    }
    ~child_t(){}

    base_t* clone() const
    {
      return new this_type(p);
    }
    parse_result_t<Attribute> parse( Iterator &begin , Iterator end ) const
    {
      return p.parse( begin , end );
    }
  };

protected:
  std::unique_ptr<base_t> parser_;

public:
  rule()
  {
  }
  rule( rule &&rhs )
    : parser_( std::move(rhs.parser_) )
  {
  }
  rule( rule const& rhs )
    : parser_( rhs.parser_->clone() )
  {
  }
  template < typename Parser >
  rule( Parser p )
    : parser_( new child_t<Parser>(std::move(p)) )
  {
  }
  rule& operator = ( rule &&rhs )
  {
    parser_ = std::move( rhs.parser_ );
    return *this;
  }
  rule& operator = ( rule const& rhs )
  {
    parser_.reset( rhs.parser_->clone() );
    return *this;
  }
  template < typename Parser >
  rule& operator = ( Parser p )
  {
    parser_.reset( new child_t<Parser>(std::move(p)) );
    return *this;
  }

  parse_result_t<Attribute>
  parse( Iterator &begin , Iterator end ) const
  {
    return parser_->parse( begin , end );
  }
};

template < typename R , typename I , typename I2 >
struct attribute<rule<R,I>,I2>
{
  using type = R;
};

}}

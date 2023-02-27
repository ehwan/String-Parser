#pragma once

/*
 * solution1
 * for all n in nodes
 *  find in map -> hash( begin , begin+n.size )
 * ->unordered map
 *
 * solution2 
 */

#include <map>
#include <iterator>
#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

template < typename Attr , typename CharType >
struct dictionary_t
  : base_t<dictionary_t<Attr,CharType>>
{
  struct node_t
  {
    bool hasattr;
    Attr attr;
    std::map<CharType,node_t> map;

    node_t()
    {
      hasattr = false;
    }
    void set_attr( Attr attr_ )
    {
      hasattr = true;
      attr = std::move(attr_);
    }
    Attr get_attr() const
    {
      return attr;
    }
    bool has_attr() const
    {
      return hasattr;
    }
    node_t const* find( CharType c ) const
    {
      auto ret = map.find( c );
      return ret != map.end() ? &ret->second : nullptr;
    }
  };

  node_t root;

  /*
   * be careful when adding ""(char[]) strings to container
   * term zero '\0' will be also added to parser
   */
  template < typename Container >
  void add( Container const& c , Attr attr )
  {
    auto end = std::end(c);
    node_t *node = &root;
    for( auto i=std::begin(c); i!=end; ++i )
    {
      node = &node->map[*i];
    }
    node->set_attr( std::move(attr) );
  }
  template < typename I >
  optional<Attr>
  parse( I &begin , I end ) const
  {
    I begin_ = begin;
    I last_tail_it = begin;

    node_t const* node = &root;
    node_t const* last_tail = nullptr;
    while( 1 )
    {
      if( begin == end ){ break; }
      node = node->find( *begin++ );
      if( node == nullptr ){ break; }
      if( node->has_attr() )
      {
        last_tail = node;
        last_tail_it = begin;
      }
    }
    if( last_tail )
    {
      begin = last_tail_it;
      return { true, last_tail->get_attr() };
    }
    begin = begin_;
    return {false};
  }

};

}}}

namespace eh { namespace parser {

template < typename Attr , typename CharType , typename I >
struct attribute< rules::dictionary_t<Attr,CharType>,I>
{
  using type = Attr;
};

template < typename Attr , typename CharType=char >
using dictionary = rules::dictionary_t<Attr,CharType>;

}}

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
#include <vector>
#include <string>
#include <cstring>
#include "../base.hpp"

namespace eh { namespace parser { namespace rules {

// contains multiple 'words'
// test if input contains on of 'words'
// longest matching string using trie
template < typename Attr , typename CharType >
struct dictionary_t
  : base_t<dictionary_t<Attr,CharType>>
{
  // Trie
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

  template < typename I >
  void add( I begin, I end, Attr const& attr )
  {
    node_t *node = &root;
    for( auto i=begin; i!=end; ++i )
    {
      node = &node->map[*i];
    }
    node->set_attr( std::move(attr) );
  }
  void add( std::vector<CharType> const& str, Attr const& attr )
  {
    add( str.begin(), str.end(), attr );
  }
  void add( std::basic_string<CharType> const& str, Attr const& attr )
  {
    add( str.begin(), str.end(), attr );
  }
  void add( CharType const* c_str, Attr const& attr )
  {
    add( c_str, c_str+std::strlen(c_str), attr );
  }

  template < typename I >
  parse_result_t<Attr>
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

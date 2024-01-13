#pragma once

#include "global.hpp"
#include <memory>

namespace eh { namespace compiler {

using node_ptr = std::unique_ptr<node_t>;

struct node_t
{
  constexpr static int NODE_STATEMENTS = 1;
  constexpr static int NODE_NUM = 2;
  constexpr static int NODE_VAR = 3;
  constexpr static int NODE_PRINT = 4;
  constexpr static int NODE_READ = 5;
  constexpr static int NODE_ASSIGN = 6;
  constexpr static int NODE_BINARYOP = 7;
  constexpr static int NODE_UNARYOP = 8;
  constexpr static int NODE_WHILE = 9;
  constexpr static int NODE_IF = 10;
  constexpr static int NODE_FUNCTION = 11;
  constexpr static int NODE_LABEL = 12;
  constexpr static int NODE_GOTO = 13;
  constexpr static int NODE_RETURN = 14;

  int type;
  std::unique_ptr<node_t> left, right;

  node_t( int t , node_ptr l=nullptr, node_ptr r=nullptr )
    : type(t) ,
      left( std::move(l) ) , right( std::move(r) )
  {
  }
  virtual ~node_t(){}
  virtual void emit( program_t* ) = 0;
};

struct node_statements
  : node_t
{
  node_statements( node_ptr l , node_ptr r )
    : node_t( NODE_STATEMENTS , std::move(l) , std::move(r) )
  {
  }
  void emit( program_t *p )
  {
    if( left ){ left->emit( p ); }
    if( right ){ right->emit( p ); }
  }
};

// constant numbers
struct node_num
  : node_t
{
  int data;

  node_num( int d )
    : node_t(NODE_NUM) ,
      data( d )
  {
  }

  void emit( program_t *p );
};

// variable
struct node_var
  : node_t
{
  variable_map::iterator data;

  node_var( variable_map::iterator i )
    : node_t(NODE_VAR) ,
      data( i )
  {
  }
  void emit( program_t *p );
};

// print statement
struct node_print
  : node_t
{
  node_print( node_ptr l )
    : node_t(NODE_PRINT,std::move(l))
  {
  }
  void emit( program_t *p );
};

// read statement
struct node_read
  : node_t
{
  variable_map::iterator data;
  node_read( variable_map::iterator it )
    : node_t(NODE_READ) ,
      data( it )
  {
  }
  void emit( program_t *p );
};

// assign value to variable
struct node_assign
  : node_t
{
  variable_map::iterator data;
  node_assign( variable_map::iterator it , node_ptr l )
    : node_t(NODE_ASSIGN,std::move(l)) ,
      data( it )
  {
  }

  void emit( program_t *p );
};

// unary operations;
// ++, --, ...
struct node_unaryop
  : node_t
{
  int op;
  node_unaryop( node_ptr l , int t )
    : node_t(NODE_UNARYOP,std::move(l))
  {
    op = t;
  }

  void emit( program_t *p );
};

// binary operations;
// a + b, a - b, ...
struct node_binaryop
  : node_t
{
  int op;
  node_binaryop( node_ptr l , node_ptr r , int t )
    : node_t(NODE_BINARYOP,std::move(l),std::move(r))
  {
    op = t;
  }
  void emit( program_t *p );
};


// jump labels
struct node_label
  : node_t
{
  // named label
  label_map::iterator data;
  // is return statement?
  bool return_;

  node_label( label_map::iterator d , node_ptr a , bool ret )
    : node_t( NODE_LABEL , std::move(a) ) ,
      data( d ) ,
      return_( ret )
  {
  }
  void emit( program_t *p );
};

// goto statement
struct node_goto
  : node_t
{
  label_map::iterator data;
  bool push;

  node_goto( label_map::iterator it , bool pushto )
    : node_t(NODE_GOTO) ,
      data(it) ,
      push(pushto)
  {
  }
  void emit( program_t *p );
};

// return statement
struct node_return
  : node_t
{
  node_return()
    : node_t( NODE_RETURN )
  {
  }
  void emit( program_t *p );

};

/*
struct node_while
  : node_t
{
  node_while( node_ptr l , node_ptr r )
    : node_t(NODE_WHILE,l,r)
  {
  }
  void emit( program_t *p );
};
struct node_if
  : node_t
{
  node_if( node_ptr l , node_ptr r )
    : node_t(NODE_IF,l,r)
  {
  }
  void emit( program_t *p );
};
*/

}}



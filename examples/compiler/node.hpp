#pragma once

#include <memory>
#include "global.hpp"

#define NODE_STATEMENTS 1
#define NODE_NUM 2
#define NODE_VAR 3
#define NODE_PRINT 4
#define NODE_READ 5
#define NODE_ASSIGN 6
#define NODE_BINARYOP 7
#define NODE_UNARYOP 8
#define NODE_WHILE 9
#define NODE_IF 10
#define NODE_FUNCTION 11
#define NODE_LABEL 12
#define NODE_GOTO 13
#define NODE_RETURN 14

struct node_t
{
  int type;
  node_ptr left, right;

  node_t( int t , node_ptr l=nullptr , node_ptr r=nullptr )
    : type(t) ,
      left( std::move(l) ) , right( std::move(r) )
  {
  }
  virtual ~node_t(){}
  virtual void emit( program_t * ) = 0;
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
struct node_print
  : node_t
{
  node_print( node_ptr l )
    : node_t(NODE_PRINT,std::move(l))
  {
  }
  void emit( program_t *p );
};
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
struct node_label
  : node_t
{
  label_map::iterator data;
  bool return_;
  node_label( label_map::iterator d , node_ptr a , bool ret )
    : node_t( NODE_LABEL , std::move(a) ) ,
      data( d ) ,
      return_( ret )
  {
  }
  void emit( program_t *p );
};
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

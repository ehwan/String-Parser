#pragma once

#include "global.hpp"

namespace eh { namespace compiler {

// default instruction; noop
struct instruction_t
{
  virtual ~instruction_t(){}
  virtual void run( program_t *p ){}
};

// push constant data to stack
struct instruction_pushstack_t
  : instruction_t
{
  int x;

  instruction_pushstack_t( int x_ )
    : instruction_t() ,
      x( x_ )
  {
  }

  void run( program_t *p );
};

// print top element of stack and pop
struct instruction_print_t
  : instruction_t
{
  void run( program_t *p );
};

// push pointing variable's value to stack
struct instruction_var_t
  : instruction_t
{
  variable_map::iterator x;

  instruction_var_t( decltype(x) x_ )
    : instruction_t() ,
      x( x_ )
  {
  }
  void run( program_t *p );
};

// read from stdin to named variable
struct instruction_read_t
  : instruction_t
{
  variable_map::iterator x;
  instruction_read_t( decltype(x) x_ )
    : instruction_t() ,
      x( x_ )
  {
  }
  void run( program_t *p );
};

// assign stack's value to variable and pop
struct instruction_assign_t
  : instruction_t
{
  variable_map::iterator x;
  instruction_assign_t( decltype(x) x_ )
    : instruction_t() ,
      x( x_ )
  {
  }
  void run( program_t *p );
};

// perform unary operation to stack's top value
struct instruction_unary_t
  : instruction_t
{
  int op;
  instruction_unary_t( int op_ )
    : instruction_t() ,
      op( op_ )
  {
  }
  void run( program_t *p );
};

// perform binary operation to stack's top-most values
struct instruction_binary_t
  : instruction_t
{
  int op;
  instruction_binary_t( int op_ )
    : instruction_t() ,
      op( op_ )
  {
  }
  void run( program_t *p );
};

// return from function
struct instruction_return_t
  : instruction_t
{
  void run( program_t *p );
};

// goto named label
struct instruction_goto_t
  : instruction_t
{
  label_map::iterator data;
  bool pushtostack;

  instruction_goto_t( decltype(data) d , bool p )
    : instruction_t() ,
      data(d) ,
      pushtostack(p)
  {
  }

  void run( program_t *p );
};

}}
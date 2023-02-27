#pragma once

#include "global.hpp"

struct instruction_t
{
  virtual ~instruction_t(){}
  virtual void run( program_t *p ){}
};

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
struct instruction_print_t
  : instruction_t
{
  void run( program_t *p );
};
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
struct instruction_return_t
  : instruction_t
{
  void run( program_t *p );
};

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

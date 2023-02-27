#include "node.hpp"
#include "program.hpp"
#include "lexer.hpp"
#include "instruction.hpp"
#include <iostream>

void node_num::emit( program_t *p )
{
  p->instructions.push_back(
      new instruction_pushstack_t{data}
    );
}
void node_print::emit( program_t *p )
{
  left->emit( p );
  p->instructions.push_back(
      new instruction_print_t()
    );
}
void node_var::emit( program_t *p )
{
  p->instructions.push_back(
      new instruction_var_t{ data }
    );
}
void node_read::emit( program_t *p )
{
  p->instructions.push_back(
      new instruction_read_t{data}
    );
}
void node_assign::emit( program_t *p )
{
  left->emit( p );
  p->instructions.push_back(
      new instruction_assign_t{data}
    );
}

void node_unaryop::emit( program_t *p )
{
  left->emit( p );
  p->instructions.push_back(
      new instruction_unary_t{op}
    );
}
void node_binaryop::emit( program_t *p )
{
  right->emit( p );
  left->emit( p );
  p->instructions.push_back(
      new instruction_binary_t{op}
    );
}
void node_label::emit( program_t *p )
{
  data->second = p->instructions.size();
  p->instructions.push_back( new instruction_t{} );
  if( left ){ left->emit(p); }
  if( return_ ){ p->instructions.push_back( new instruction_return_t{} ); }
}

void node_goto::emit( program_t *p )
{
  p->instructions.push_back( new instruction_goto_t{data,push} );
}
void node_return::emit( program_t *p )
{
  p->instructions.push_back( new instruction_return_t{} );
}

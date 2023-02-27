#include "instruction.hpp"
#include "program.hpp"
#include "lexer.hpp"

void instruction_pushstack_t::run( program_t *p )
{
  p->stack.push( x );
}
void instruction_print_t::run( program_t *p )
{
  std::cout << p->stack.top() << std::endl;
  p->stack.pop();
}
void instruction_var_t::run( program_t *p )
{
  p->stack.push( p->memory[ x->second.offset ] );
}
void instruction_read_t::run( program_t *p )
{
  std::cin >> p->memory[x->second.offset];
}
void instruction_assign_t::run( program_t *p )
{
  p->memory[x->second.offset] = p->stack.top();
  p->stack.pop();
}
void instruction_unary_t::run( program_t *p )
{
  int &x = p->stack.top();
  switch( op )
  {
  case '-':
    x = -x;
    break;
  case '!':
    x = !x;
    break;
  }
}
void instruction_binary_t::run( program_t *p )
{
  int b = p->stack.top();
  p->stack.pop();
  int &a = p->stack.top();
  switch( op )
  {
  case '*':
    a *= b;
    break;
  case '/':
    a /= b;
    break;
  case '%':
    a %= b;
    break;
  case '+':
    a += b;
    break;
  case '-':
    a -= b;
    break;
  case '>':
    a = ( a > b );
    break;
  case '<':
    a = ( a < b );
    break;
  case TOKEN_LEQ:
    a = ( a <= b );
    break;
  case TOKEN_GEQ:
    a = ( a >= b );
    break;
  case TOKEN_EQ:
    a = ( a == b );
    break;
  case TOKEN_NEQ:
    a = ( a != b );
    break;
  case TOKEN_LOGICAL_AND:
    a = ( a && b );
    break;
  case TOKEN_LOGICAL_OR:
    a = ( a || b );
    break;
  }
}
void instruction_return_t::run( program_t *p )
{
  p->current_instruct = p->stack.top();
  p->stack.pop();
}
void instruction_goto_t::run( program_t *p )
{
  if( pushtostack )
  {
    p->stack.push( p->current_instruct );
  }
  if( data->second == -1 )
  {
    std::cout << "no label " << data->first << std::endl;
    std::exit( 0 );
  }
  p->current_instruct = data->second;
}

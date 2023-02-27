#include "program.hpp"
#include "instruction.hpp"

void program_t::run()
{
  stack.push( instructions.size()-1 );
  current_instruct = main;
  while( current_instruct < instructions.size() )
  {
    instructions[ current_instruct ]->run( this );
    ++current_instruct;
  }
}
program_t::~program_t()
{
  for( auto &i : instructions )
  {
    delete i;
  }
}

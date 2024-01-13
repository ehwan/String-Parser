#include "program.hpp"
#include "instruction.hpp"

namespace eh { namespace compiler {

void program_t::run()
{
  stack.push( instructions.size()-1 );
  auto main_it = labels.find( "main" );
  if( main_it == labels.end() )
  {
    std::cout << "No function main()\n";
    return;
  }
  current_instruct = main_it->second;
  while( current_instruct < instructions.size() )
  {
    instructions[ current_instruct ]->run( this );
    ++current_instruct;
  }
}

}}
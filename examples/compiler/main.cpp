#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include "lexer.hpp"
#include "compiler.hpp"
#include "program.hpp"
#include "node.hpp"

std::vector<char> read_from( int f )
{
  constexpr int size = 1024;
  std::vector<char> ret;
  ret.reserve( size );
  char buf[size];
  int r;

  while( (r = read(f,buf,size)) )
  {
    ret.insert( ret.end() , buf+0 , buf+r );
  }
  return ret;
}
int main( int argc , const char** argv )
{
  /*
  if( argc < 2 )
  {
    std::cout << "arguments error\n";
    return 0;
  }
  int f = open( argv[1] , O_RDONLY );
  */
  std::vector<char> stream = read_from(0);

  auto tokens = tokenize( stream.begin() , stream.end() );

  compiler_t compiler;
  auto program = compiler.compile( tokens.begin() , tokens.end() );
  program->run();

  return 0;
}

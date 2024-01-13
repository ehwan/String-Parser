#include <iostream>
#include <sys/fcntl.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include "program.hpp"
#include "node.hpp"
#include "tokenizer.hpp"
#include "compiler.hpp"

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
  if( argc < 2 )
  {
    std::cout << "arguments error\n";
    std::cout << "USAGE:\n";
    std::cout << argv[0] << " {file_name}\n";
    return 0;
  }
  eh::compiler::tokenizer_t tokenizer;
  int file_desc = open( argv[1], O_RDONLY );
  tokenizer.input = read_from( file_desc );
  close( file_desc );

  std::cout << "Start Tokenizing...\n";
  bool tokenize_result = tokenizer.parse();
  std::cout << "Tokenizing Result: ";
  std::cout << std::boolalpha << tokenize_result << "\n";
  for( auto &t : tokenizer.tokens )
  {
    std::cout << t.raw << ": " << t.type << "\n";
  }

  std::cout << "Tokenizing End...\n";

  std::cout << "Compiling Start...\n";
  eh::compiler::compiler_t compiler;
  compiler.tokens = std::move( tokenizer.tokens );
  auto compile_result = compiler.compile();
  std::cout << "Compiling End... : " << std::boolalpha << compile_result << "\n";

  std::cout << "~~~~~~~~~~~~~~~~~~~~~ Program Result ~~~~~~~~~~~~~~~~~~~~~\n";
  compiler.program->run();

  return 0;
}

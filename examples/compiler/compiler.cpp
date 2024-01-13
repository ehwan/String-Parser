#include "compiler.hpp"
#include "tokenizer.hpp"
#include "program.hpp"
#include "node.hpp"
#include <iostream>

namespace eh { namespace compiler {

compiler_t::compiler_t()
{
  namespace ep = eh::parser;
  using namespace ep::literals;

  using namespace eh::parser;
  auto num = one(token_t::TOKEN_NUM)[(
      []( token_t &t )->node_ptr
      {
        return node_ptr(new node_num( std::stoi( t.raw ) ) );
      } )];
  auto var = one(token_t::TOKEN_IDENT)[(
      [this]( token_t &t )->variable_map::iterator
      {
        auto it = program->variables.find( t.raw );
        if( it == program->variables.end() )
        {
          it = program->variables.emplace( t.raw, 0 ).first;
        }
        return it;
      } )];

  auto exprvar = var[(
      []( variable_map::iterator it )->node_ptr
      {
        return node_ptr( new node_var(it) );
      } )];
  auto expr0 = num | exprvar | ('('_p.unused() >> rules.rhs.ref() >> ')'_p.unused());


  std::set<int> expr1op = { '+' , '-' , '!' };
  rules.expr1 = 
    (ep::set_(expr1op) >> rules.expr1.ref())[(
        []( token_t &token, node_ptr &n )->node_ptr
        {
          return node_ptr( new node_unaryop(std::move(n),token.type) );
        } )]
    | expr0;

  struct binary_parser_t
    : eh::parser::rules::base_t<binary_parser_t>
  {
    struct reducer_t
    {
      node_ptr data;
      eh::parser::unused_t operator()( node_ptr &a )
      {
        data = std::move(a);
        return {};
      }
      eh::parser::unused_t operator()( token_t &t , node_ptr &b )
      {
        node_ptr n{
          new node_binaryop( std::move(data) , std::move(b) , t.type )
        };
        data = std::move( n );
        return {};
      }
      node_ptr operator()()
      {
        return std::move(data);
      }
    };
    using it_type = std::vector<token_t>::iterator;

    rule_t num_parser;
    ep::rule<token_t,std::vector<token_t>::iterator> op_parser;
    binary_parser_t( rule_t num_ , decltype(op_parser) op_ )
      : num_parser(std::move(num_)) ,
        op_parser(std::move(op_))
    {
    }

    auto parse( it_type &begin , it_type end ) const
    {
      reducer_t reducer{};
      auto parser = num_parser[ std::ref(reducer) ] >> *( (op_parser >> num_parser)[std::ref(reducer)] );
      return parser[ std::ref(reducer) ].parse( begin, end );
    }
  };

  auto expr2op = '*'_p | '/'_p | '%'_p;
  auto expr2 = binary_parser_t{ rules.expr1 , expr2op };

  auto expr3op = '+'_p | '-'_p;
  auto expr3 = binary_parser_t{ expr2 , expr3op };

  auto expr4op = '>'_p | '<'_p | one(token_t::TOKEN_LEQ) | one(token_t::TOKEN_GEQ);
  auto expr4 = binary_parser_t{ expr3 , expr4op };

  auto expr5op = one(token_t::TOKEN_EQ) | one(token_t::TOKEN_NEQ);
  auto expr5 = binary_parser_t{ expr4 , expr5op };

  auto expr6op = one(token_t::TOKEN_LOGICAL_AND);
  auto expr6 = binary_parser_t{ expr5 , expr6op };

  auto expr7op = one(token_t::TOKEN_LOGICAL_OR);
  auto expr7 = binary_parser_t{ expr6 , expr7op };

  auto rhs = expr7;
  rules.rhs = rhs;


  auto label = one(token_t::TOKEN_IDENT)[(
      [this]( token_t &t )->label_map::iterator
      {
        auto it = program->labels.find( t.raw );
        if( it == program->labels.end() )
        {
          it = program->labels.emplace( t.raw, -1 ).first;
        }
        return it;
      } )];

  auto curly_statements =
    '{'_p.unused() >> rules.statements.ref() >> '}'_p.unused();

  auto print = (one(token_t::TOKEN_PRINT).unused() >> rhs >> ';'_p.unused())[(
      []( node_ptr &n )->node_ptr
      {
        return node_ptr(new node_print(std::move(n)));
      } )];
  auto read = (one(token_t::TOKEN_READ).unused() >> var >> ';'_p.unused())[(
      []( variable_map::iterator it )->node_ptr
      {
        return node_ptr(new node_read(it));
      } )];
  auto assign = (var >> '='_p.unused() >> rhs >> ';'_p.unused())[(
      []( variable_map::iterator it  , node_ptr &b )->node_ptr
      {
        return node_ptr(new node_assign(it,std::move(b)));
      } )];

  auto funccall = (label >> '('_p.unused() >> ')'_p.unused() >> 
      ';'_p.unused())[(
      []( label_map::iterator it )->node_ptr
      {
        return node_ptr( new node_goto(it,true) );
      } )];
  auto return_ = (one(token_t::TOKEN_RETURN).unused() >> ';'_p.unused())[(
      []()->node_ptr
      {
        return node_ptr( new node_return() );
      } )];
  auto funcdecl = (label >> '('_p.unused() >> ')'_p.unused() >>
      '{'_p.unused() >> rules.statements.ref() >> '}'_p.unused() )[(
      []( label_map::iterator it , node_ptr &n )->node_ptr
      {
        return node_ptr( 
            new node_label( it , std::move(n) , true ) );
      } )];

  auto statement = 
    print 
    | read
    | assign
    | funccall
    | return_
    | curly_statements;

  rules.statements =
    (statement >> (rules.statements.ref()|true_.construct<node_ptr>()))
    [(
        []( node_ptr &a , node_ptr &b )->node_ptr
        {
          if( b )
          {
            return node_ptr( new node_statements(std::move(a),std::move(b)));
          }else
          {
            return std::move(a);
          }
        } )];

  rules.program =
    (funcdecl >> (rules.program.ref()|true_.construct<node_ptr>()))
    [(
        []( node_ptr &a , node_ptr &b )->node_ptr
        {
          return node_ptr( new node_statements(std::move(a),std::move(b)) );
        } )];
}

bool compiler_t::compile()
{
  program.reset( new program_t );
  auto begin = tokens.begin();
  auto ret = rules.program.parse(begin, tokens.end());
  if( ret.is_valid() == false )
  {
    std::cout << "syntax error\n";
    return false;
  }
  ret.get()->emit( program.get() );

  return true;
}

}}
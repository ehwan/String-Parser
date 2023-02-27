#include "compiler.hpp"
#include "lexer.hpp"
#include "program.hpp"
#include "node.hpp"
#include <iostream>

compiler_t::compiler_t()
{
  var_used = 0;
  using namespace eh::parser;
  auto num = one(TOKEN_NUM)[(
      []( token_type t )->node_ptr
      {
        return node_ptr(new node_num( std::stoi(std::string(t.begin,t.end)) ));
      } )];
  auto var = one(TOKEN_IDENT)[(
      [this]( token_type t )->variable_map::iterator
      {
      std::cout << "Var1\n";
        std::string name{ t.begin , t.end };
        auto it = variables.find( name );
        if( it == variables.end() )
        {
          it = variables.emplace( name , var_info{var_used++} ).first;
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
    (find(expr1op) >> rules.expr1.ref())[(
        []( token_type t , node_ptr &n )->node_ptr
        {
          return node_ptr( new node_unaryop(std::move(n),t.data) );
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
      eh::parser::unused_t operator()( token_type t , node_ptr &b )
      {
        node_ptr n{
          new node_binaryop( std::move(data) , std::move(b) , t.data )
        };
        data = std::move( n );
        return {};
      }
      node_ptr operator()()
      {
        return std::move(data);
      }
    };
    using it_type = std::vector<token_type>::iterator;

    rule_t num;
    eh::parser::rule<token_type,it_type> op;
    binary_parser_t( rule_t num_ , decltype(op) op_ )
      : num(std::move(num_)) ,
        op(std::move(op_))
    {
    }

    eh::parser::optional<node_ptr> parse( it_type &begin , it_type end ) const
    {
      reducer_t reducer{};
      auto parser = num[ std::ref(reducer) ] >> *( (op >> num)[std::ref(reducer)] );
      return parser[ std::ref(reducer) ].parse( begin , end );
    }
  };

  auto expr2op = '*'_p | '/'_p | '%'_p;
  auto expr2 = binary_parser_t{ rules.expr1 , expr2op };

  auto expr3op = '+'_p | '-'_p;
  auto expr3 = binary_parser_t{ expr2 , expr3op };

  auto expr4op = '>'_p | '<'_p | one(TOKEN_LEQ) | one(TOKEN_GEQ);
  auto expr4 = binary_parser_t{ expr3 , expr4op };

  auto expr5op = one(TOKEN_EQ) | one(TOKEN_NEQ);
  auto expr5 = binary_parser_t{ expr4 , expr5op };

  auto expr6op = one(TOKEN_LOGICAL_AND);
  auto expr6 = binary_parser_t{ expr5 , expr6op };

  auto expr7op = one(TOKEN_LOGICAL_OR);
  auto expr7 = binary_parser_t{ expr6 , expr7op };

  auto rhs = expr7;
  rules.rhs = rhs;


  auto label = one(TOKEN_IDENT)[(
      [this]( token_type t )->label_map::iterator
      {
        std::string name{t.begin,t.end};
        auto it = labels.find( name );
        if( it == labels.end() )
        {
          it = labels.emplace( name , -1 ).first;
        }
        return it;
      } )];

  auto curly_statements =
    '{'_p.unused() >> rules.statements.ref() >> '}'_p.unused();

  auto print = (one(TOKEN_PRINT).unused() >> rhs >> ';'_p.unused())[(
      []( node_ptr &n )->node_ptr
      {
        return node_ptr(new node_print(std::move(n)));
      } )];
  auto read = (one(TOKEN_READ).unused() >> var >> ';'_p.unused())[(
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
  auto return_ = (one(TOKEN_RETURN).unused() >> ';'_p.unused())[(
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

std::unique_ptr<program_t> compiler_t::compile( 
    std::vector<token_type>::iterator b ,
    std::vector<token_type>::iterator e )
{
  var_used = 0;
  auto ret = rules.program.parse(b,e);
  if( ret.is_valid() == false )
  {
    std::cout << "syntax error\n";
    std::exit( 0 );
  }
  program_t *p = new program_t{ var_used };
  ret.get()->emit( p );

  auto mainit = labels.find( "main" );
  if( mainit == labels.end() )
  {
    std::cout << "no function main\n";
    std::exit(0);
  }
  p->main = mainit->second;
  if( p->main == -1 )
  {
    std::cout << "no function main decl\n";
    std::exit(0);
  }
  return std::unique_ptr<program_t>(p);
}

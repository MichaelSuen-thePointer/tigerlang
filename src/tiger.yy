%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {tiger}
%define parser_class_name {TigerParser}

%code requires {

namespace tiger
{
    class TigerScanner;
}

#ifdef YY_NULLPTR
#undef YY_NULLPTR
#endif
#define YY_NULLPTR nullptr
}

%parse-param { tiger::TigerScanner& scanner }

%code{

#include <iostream>
#include <cstdlib>
#include <fstream>
#include "TigerScanner.hpp"

#undef yylex
#define yylex scanner.yylex

}

%define api.value.type variant
%define parse.assert

%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE ASSIGN COLON DOT COMMA SEMICOLON TIMES DIVIDE PLUS MINUS EQ NEQ GT LT GE LE AND OR END ARRAY BREAK DO ELSE END FOR FUNCTION IF IN LET NIL OF THEN TO TYPE VAR WHILE
%type <int>         INT
%type <std::string> STRING
%type <std::string> IDENTIFIER

%%

explist : printexp | explist printexp;

printexp : exp {
    std::cout << $1 << std::endl;
}

exp : exp '+' term { $$ = $1 + $3; }
    | exp '-' term { $$ = $1 - $3; }
    | term         { $$ = $1; } ;

term : term '*' factor  { $$ = $1 * $3; }
     | term '/' factor  { $$ = $1 / $3; }
     | factor           { $$ = $1; } ;

factor : NUMBER         { $$ = $1; }
       | '(' exp ')'    { $$ = $2; } ;

%%

/* allows for printing of an error message */
void mq::CalcParser::error(const std::string& msg)
{
    std::cerr << "Error: " << msg << "\n";
}
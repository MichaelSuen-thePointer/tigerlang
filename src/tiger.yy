%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {tiger}
%define parser_class_name {TigerParser}
%define parse.error verbose

%code requires {

namespace tiger
{
    class TigerScanner;
}

#ifdef YY_NULLPTR
#undef YY_NULLPTR
#endif
#define YY_NULLPTR nullptr
#include "TigerAST.hpp"
}

%parse-param { tiger::TigerScanner& scanner  }
%parse-param { std::unique_ptr<Program>& prog }

%code{

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <memory>
#include "TigerScanner.hpp"

#undef yylex
#define yylex scanner.yylex

}

%define api.value.type variant
%define parse.assert

%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE ASSIGN COLON DOT COMMA SEMICOLON TIMES DIVIDE PLUS MINUS EQ NEQ GT LT GE LE AND OR ARRAY BREAK DO ELSE END FOR FUNCTION IF IN LET NIL OF THEN TO TYPE VAR WHILE UNKNOWN
%token <int>         INT
%token <std::string> STRING
%token <std::string> IDENTIFIER
%type <Expression*> expression
%type <Expression*> breakable-expression
%type <IfThenElse*> if-then-else
%type <IfThen*> if-then
%type <For*> for
%type <While*> while
%type <Let*> let-expression
%type <std::vector<Expression*>> semicolon-separated-expression
%type <std::vector<Expression*>> nonempty-semicolon-separated-expression
%type <Assignment*> assignment-expression
%type <Expression*> logical-or-expression
%type <Expression*> logical-and-expression
%type <Expression*> equality-expression
%type <Expression*> additive-expression
%type <Expression*> multiplicative-expression
%type <Expression*> unary-expression
%type <Expression*> postfix-expression
%type <Expression*> lvalue-expression
%type <std::vector<std::tuple<std::string, Expression*, tiger::location>>> comma-separated-record-field
%type <std::vector<std::tuple<std::string, Expression*, tiger::location>>> nonempty-comma-separated-record-field
%type <std::vector<Expression*>> comma-separated-expression
%type <std::vector<Expression*>> nonempty-comma-separated-expression
%type <std::vector<Declaration*>> space-separated-declaration
%type <Declaration*> declaration
%type <TypeDeclaration*> type-declaration
%type <FunctionDeclaration*> function-declaration
%type <std::vector<FunctionParameter>> comma-separated-parameter
%type <std::vector<FunctionParameter>> nonempty-comma-separated-parameter
%type <VariableDeclaration*> variable-declaration
%type <std::string> nullable-colon-type
%type <Type*> type
%type <RecordType*> record-type
%type <std::vector<FieldDeclaration>> comma-separated-field

%locations

%%

program:
    expression {
        prog.reset(new Program($1));
        prog->loc(@1);
    };

breakable-expression:
    BREAK { $$ = new Break(); $$->loc(@1); }
    | expression { $$ = $1; };

expression:
    if-then-else { $$ = $1; }
    | if-then { $$ = $1; }
    | while { $$ = $1; }
    | for { $$ = $1; }
    | let-expression { $$ = $1; }
    | assignment-expression { $$ = $1; }
    | logical-or-expression { $$ = $1; };

if-then-else:
    IF expression THEN expression ELSE expression {
        $$ = new IfThenElse($2, $4, $6);
        $$->loc(@1);
    };

if-then:
    IF expression THEN expression { $$ = new IfThen($2, $4); };

for:
    FOR IDENTIFIER ASSIGN expression TO expression DO breakable-expression {
        $$ = new For($2, $4, $6, $8);
        $$->loc(@1);
    };

while:
    WHILE expression DO breakable-expression {
        $$ = new While($2, $4);
        $$->loc(@1);
    };

let-expression:
    LET space-separated-declaration IN semicolon-separated-expression END {
        $$ = new Let($2, $4); 
        $$->loc(@1);
    };

semicolon-separated-expression:
    %empty { $$ = std::vector<Expression*>(); }
    | nonempty-semicolon-separated-expression { $$ = $1; };

nonempty-semicolon-separated-expression:
    expression{
        $$ = std::vector<Expression*>(1, $1);
    }
    | semicolon-separated-expression SEMICOLON expression {
        $$ = $1;
        $$.push_back($3);
    };

assignment-expression:
    lvalue-expression ASSIGN expression { $$ = new Assignment($1, $3); $$->loc(@1); };

logical-or-expression:
    logical-and-expression { $$ = $1; }
    | logical-or-expression OR logical-and-expression { $$ = new Or($1, $3); $$->loc(@1); };

logical-and-expression:
    equality-expression { $$ = $1; }
    | logical-and-expression AND equality-expression { $$ = new And($1, $3); $$->loc(@1); };

equality-expression:
    additive-expression { $$ = $1; }
    | additive-expression EQ additive-expression { $$ = new Eq($1, $3); $$->loc(@1); }
    | additive-expression NEQ additive-expression { $$ = new Ne($1, $3); $$->loc(@1); }
    | additive-expression LT additive-expression { $$ = new Lt($1, $3); $$->loc(@1); }
    | additive-expression LE additive-expression { $$ = new Le($1, $3); $$->loc(@1); }
    | additive-expression GT additive-expression { $$ = new Gt($1, $3); $$->loc(@1); }
    | additive-expression GE additive-expression { $$ = new Ge($1, $3); $$->loc(@1); };

additive-expression:
    multiplicative-expression { $$ = $1; }
    | additive-expression PLUS multiplicative-expression { $$ = new Add($1, $3); $$->loc(@1); }
    | additive-expression MINUS multiplicative-expression { $$ = new Sub($1, $3); $$->loc(@1); };

multiplicative-expression:
    unary-expression { $$ = $1; }
    | multiplicative-expression TIMES unary-expression { $$ = new Mul($1, $3); $$->loc(@1); }
    | multiplicative-expression DIVIDE unary-expression { $$ = new Div($1, $3); $$->loc(@1); };

unary-expression:
    postfix-expression { $$ = $1; }
    | MINUS unary-expression { $$ = new Negation($2); $$->loc(@1); };

postfix-expression:
    lvalue-expression { $$ = $1; }
    | STRING { $$ = new StringLiteral($1); $$->loc(@1); }
    | INT { $$ = new IntLiteral($1); $$->loc(@1); }
    | NIL { $$ = new Nil(); $$->loc(@1); }
    | LPAREN semicolon-separated-expression RPAREN { $$ = new Sequence($2); $$->loc(@1); }
    | IDENTIFIER LPAREN comma-separated-expression RPAREN { $$ = new Call($1, $3); $$->loc(@1); }
    | IDENTIFIER LBRACE comma-separated-record-field RBRACE { $$ = new RecordCreate($1, $3); $$->loc(@1); }
    | IDENTIFIER LBRACKET expression RBRACKET OF expression { $$ = new ArrayCreate($1, $3, $6); $$->loc(@1); };

lvalue-expression:
    IDENTIFIER { $$ = new Identifier($1); $$->loc(@1); }
    | IDENTIFIER LBRACKET expression RBRACKET {
        auto i = new Identifier($1);
        i->loc(@1);
        $$ = new Subscript(i, $3);
        $$->loc(@1);
    }
    | lvalue-expression LBRACKET expression RBRACKET { $$ = new Subscript($1, $3); $$->loc(@1); }
    | lvalue-expression DOT IDENTIFIER { $$ = new FieldExpression($1, $3); $$->loc(@1); };

comma-separated-record-field:
    %empty { $$ = std::vector<std::tuple<std::string, Expression*, tiger::location>>(); }
    | nonempty-comma-separated-record-field { $$ = $1; };

nonempty-comma-separated-record-field:
    IDENTIFIER EQ expression {
        $$ = std::vector<std::tuple<std::string, Expression*, tiger::location>>();
        $$.emplace_back($1, $3, @1);
    }
    | comma-separated-record-field COMMA IDENTIFIER EQ expression {
        $1.emplace_back($3, $5, @1);
        $$ = $1;
    };

comma-separated-expression:
    %empty { $$ = std::vector<Expression*>(); }
    | nonempty-comma-separated-expression { $$ = $1; };

nonempty-comma-separated-expression:
    expression { $$ = std::vector<Expression*>(1, $1); }
    | comma-separated-expression COMMA expression { $1.push_back($3); $$ = $1; };

space-separated-declaration:
    %empty{ $$ = std::vector<Declaration*>(); }
    | space-separated-declaration declaration {
        $$ = $1;
        $$.push_back($2); 
    };

declaration:
    function-declaration { $$ = $1; }
    | variable-declaration { $$ = $1; }
    | type-declaration { $$ = $1; };

type-declaration:
    TYPE IDENTIFIER EQ type { $$ = new TypeDeclaration($2, $4); $$->loc(@1); };

type:
    IDENTIFIER { $$ = new IdentifierType($1); $$->loc(@1); }
    | ARRAY OF IDENTIFIER { $$ = new ArrayType($3); $$->loc(@1); }
    | record-type { $$ = $1; };

record-type:
    LBRACE comma-separated-field RBRACE { $$ = new RecordType($2); $$->loc(@1); };

comma-separated-field:
    IDENTIFIER COLON IDENTIFIER {
        $$ = std::vector<FieldDeclaration>();
        $$.emplace_back($1, $3);
        $$.back().loc(@1);
    }
    | comma-separated-field COMMA IDENTIFIER COLON IDENTIFIER {
        $$ = $1;
        $$.emplace_back($3, $5);
        $$.back().loc(@3);
    };

function-declaration:
    FUNCTION IDENTIFIER LPAREN comma-separated-parameter RPAREN COLON IDENTIFIER EQ expression {
        $$ = new FunctionDeclaration($2, $4, $7, $9);
        $$->loc(@1); 
    }
    | FUNCTION IDENTIFIER LPAREN comma-separated-parameter RPAREN EQ expression {
        $$ = new FunctionDeclaration($2, $4, "", $7);
        $$->loc(@1); 
    };

comma-separated-parameter:
    %empty { $$ = std::vector<FunctionParameter>(); }
    | nonempty-comma-separated-parameter { $$ = $1; };

nonempty-comma-separated-parameter:
    IDENTIFIER COLON IDENTIFIER {
        $$ = std::vector<FunctionParameter>();
        $$.emplace_back($1, $3);
        $$.back().loc(@1);
    }
    | comma-separated-parameter COMMA IDENTIFIER COLON IDENTIFIER {
        $$ = $1;
        $$.emplace_back($3, $5);
        $$.back().loc(@3);
    };

variable-declaration:
    VAR IDENTIFIER nullable-colon-type ASSIGN expression {
        $$ = new VariableDeclaration($2, $3, $5);
        $$->loc(@1); 
    };

nullable-colon-type:
    %empty { $$ = ""; }
    | COLON IDENTIFIER { $$ = $2; };


%%

/* allows for printing of an error message */
void tiger::TigerParser::error(const location& loc, const std::string& msg)
{
    std::cerr << "Error: " << msg << " at " << loc << "\n";
}
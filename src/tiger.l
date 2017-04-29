%{
#include <string>
#include "TigerScanner.hpp"
#undef YY_DECL
#define YY_DECL int tiger::TigerScanner::yylex(tiger::TigerParser::semantic_type* const lval)

using token = tiger::TigerParser::token_type;

%}

%option nodefault
%option debug
%option noyywrap
%option yyclass="tiger::TigerScanner"
%option c++

%%
%{
    yylval = lval;
%}
\( { return token::LPAREN; }
\) { return token::RPAREN; }
\[ { return token::LBRACKET; }
\] { return token::RBRACKET; }
\{ { return token::LBRACE; }
\} { return token::RBRACE; }
:= { return token::ASSIGN; }
:  { return token::COLON; }
\. { return token::DOT; }
,  { return token::COMMA; }
;  { return token::SEMICOLON; }
\* { return token::TIMES; }
\/ { return token::DIVIDE; }
\+ { return token::PLUS; }
\- { return token::MINUS; }
=  { return token::EQ; }
<> { return token::NEQ; }
>  { return token::GT; }
<  { return token::LT; }
>= { return token::GE; }
<= { return token::LE; }
&  { return token::AND; }
|  { return token::OR; }

array    { return token::ARRAY; }
break    { return token::BREAK; }
do       { return token::DO; }
else     { return token::ELSE; }
end      { return token::END; }
for      { return token::FOR; }
function { return token::FUNCTION; }
if       { return token::IF; }
in       { return token::IN; }
let      { return token::LET; }
nil      { return token::NIL; }
of       { return token::OF; }
then     { return token::THEN; }
to       { return token::TO; }
type     { return token::TYPE; }
var      { return token::VAR; }
while    { return token::WHILE; }

[0-9]+ {
    yylval->build(atoi(yytext));
    return token::INT;
}

\"([\40-\176]|\\n|\\r|\\t|\\\"|\\\\|\\[0-9]{3,3}|\\[ \t\r\n]*\\)*\" {
    yylval->build(escapeString(yytext));
    return token::STRING;
}

[a-zA-Z][a-zA-Z0-9_]* {
    yylval->build(std::string(yytext));
    return token::ID;
}

"/*"(([^\*]*(\*[^\/])?)*)"*/" { continue; }

[:space:] { continue; }

. { error(yytext); }

%%
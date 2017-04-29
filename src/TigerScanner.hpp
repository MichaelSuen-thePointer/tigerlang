#pragma once

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

#include "tiger.tab.hh"

namespace tiger
{
class TigerScanner : public yyFlexLexer
{
public:
    TigerScanner(std::istream* in)
        : yyFlexLexer(in)
    {
    }
    
    virtual int yylex(TigerParser::semantic_type* const lval);
private:
    TigerParser::semantic_type* yylval = nullptr;
};

}
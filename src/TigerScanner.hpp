#pragma once
#include <string>

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif
#include "tiger.tab.hh"
#include "location.hh"
namespace tiger
{
class TigerScanner : public yyFlexLexer
{
public:
    TigerScanner(std::istream* in)
        : yyFlexLexer(in)
    {
    }
    virtual int yylex(TigerParser::semantic_type* const lval, TigerParser::location_type* location);
private:
    std::string escapeString(const char *text, TigerParser::location_type* loc);
    int escapeStringImpl(const char* raw, std::string& ret);
    void updateLocation(const char* text, TigerParser::location_type* loc);

    void error(const char* yytext, TigerParser::location_type* loc);
};

}
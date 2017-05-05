#include "TigerScanner.hpp"

namespace tiger
{

std::string TigerScanner::escapeString(const char *text, TigerParser::location_type* loc)
{
    std::string ret;
    auto res = escapeStringImpl(text, ret);
    if (res != 0)
    {
        error(text, loc);
    }
    return ret;
}

int TigerScanner::escapeStringImpl(const char *raw, std::string &ret)
{
    static const int ESCAPE_LOST = 101;
    // the specify unkown escape char will be store in ret
    static const int ESCAPE_UNKNOWN = 102;

    int result = 0, n = 0, i = 0;
    const char *p = raw;
    //begin:
    ret = "";
    p--;
normal:
    p++;
    if (*p == 0)
        goto exit;
    if (*p == '\\')
        goto escape;
    ret += *p;
    goto normal;
escape:
    p++;
    if (*p == 0)
    {
        result = ESCAPE_LOST;
        goto exit;
    }
    if (*p == '\\')
        ret += '\\';
    else if (*p == 'n')
        ret += '\n';
    else if (*p == 't')
        ret += '\t';
    else if (*p == '\"')
        ret += '\"';
    else if (*p == '^')
        goto controlc;
    else if (isspace(*p))
        goto skip;
    else if (*p >= '0' && *p <= '9')
    {
        n = *p - '0';
        for (i = 0, p++; i < 2; i++, p++)
        {
            if (*p == 0)
            {
                result = ESCAPE_LOST;
                goto exit;
            }
            if (*p >= '0' && *p <= '9')
            {
                n *= 10;
                n += *p - '0';
            }
            else
                break;
        }
        p--;
        if (i < 2 || n >= 256)
        {
            ret = "\\";
            while (i--)
            {
                ret += p[1 - i];
            }
            result = ESCAPE_UNKNOWN;
            goto exit;
        }
        ret += (char)n;
        goto normal;
    }
    else
    {
        ret = "\\";
        ret += *p;
        result = ESCAPE_UNKNOWN;
        goto exit;
    }
    goto normal;
controlc:
    p++;
    if (*p == 'c')
    {
        ret += (char)3;
        goto normal;
    }
    if (*p == 0)
    {
        result = ESCAPE_LOST;
        goto exit;
    }
    ret = "\\^";
    ret += *p;
    result = ESCAPE_UNKNOWN;
    goto exit;
skip:
    p++;
    if (*p == 0)
    {
        result = ESCAPE_LOST;
        goto exit;
    }
    if (isspace(*p))
    {
        if (p[1] == 0)
        {
            result = ESCAPE_LOST;
            goto exit;
        }
        else if (p[1] == '\\')
        {
            p++;
            goto normal;
        }
    }
    ret += *p;
    goto skip;
exit:

    return result;
}
void TigerScanner::updateLocation(const char *str, TigerParser::location_type* loc)
{
    loc->step();
    while (*str)
    {
        if (*str == '\n')
        {
            loc->lines();
        }
        else
        {
            loc->columns();
        }
        str++;
    }
}

void TigerScanner::error(const char *text, TigerParser::location_type* loc)
{
    std::cerr << "Bad token: `" << text << "` at " << *loc << "\n";
}
}
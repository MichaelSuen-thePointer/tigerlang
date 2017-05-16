#pragma once
#include "TigerAST.hpp"

namespace tiger
{

class Frame
{
private:
    static std::vector<std::unique_ptr<Frame>> _frameBuffer;
    static std::vector<int> _stack;
public:
    static void enterFrame()
    {
        auto current = _frameBuffer[_stack.back()].get();
        _frameBuffer.emplace_back(current);
        _stack.push_back(_frameBuffer.size() - 1);
    }

    static void exitFrame()
    {

    }
};

class TigerFrame
{
protected:
    TigerFrame* _staticLink;
    std::map<std::string, int> _parameters;
    std::map<std::string, int> _variables;
    std::vector<int> _scopeOffset;
    int _parameterOffset;

public:
    constexpr static int WordSize = 4;
    explicit TigerFrame(TigerFrame* staticLink)
        : _staticLink(staticLink), _scopeOffset(1, 0), _parameterOffset(WordSize)
    {
    }
    TigerFrame* staticLink()const
    {
        return _staticLink;
    }
    void inScope()
    {
        _scopeOffset.push_back(_scopeOffset.back());
    }
    void outScope()
    {
        _scopeOffset.pop_back();
    }
    void addVariable(const std::string& name)
    {
        _scopeOffset.back() -= WordSize;
        _variables[name] = _scopeOffset.size();
    }
    void addParameter(const std::string& name)
    {
        _parameterOffset += WordSize;
        _parameters[name] = _parameterOffset;
    }

    int getOffset(const std::string& name)
    {
        {
            auto r = _variables.find(name);
            if (r != _variables.end() && r->second >= _scopeOffset.back())
            {                                     //栈变量的offset是负的
                return r->second;
            }
        }
        {
            auto r = _parameters.find(name);
            if (r != _variables.end())
            {
                return r->second;
            }
        }
        assert(0);
        return 0;
    }

    TigerFrame* frameByName(const std::string& name)
    {
        TigerFrame* current = this;
        for (; current != nullptr; current = current->staticLink())
        {
            {
                auto r = current->_variables.find(name);
                if (r != current->_variables.end() && r->second >= _scopeOffset.back())
                {                                     //栈变量的offset是负的
                    return current;
                }
            }
            {
                auto r = current->_parameters.find(name);
                if (r != current->_variables.end())
                {
                    return current;
                }
            }
        }
        assert(0);
        return nullptr;
    }
};

}
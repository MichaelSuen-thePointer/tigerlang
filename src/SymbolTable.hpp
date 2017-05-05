#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <cassert>

namespace tiger
{
class Type;
class FunctionDeclaration;
class IdentifierType;
class SymbolTable
{
    std::vector<std::map<std::string, Type*>> _varTable;
    std::vector<std::map<std::string, Type*>> _typeTable;
    std::vector<std::map<std::string, FunctionDeclaration*>> _funcTable;
    static std::map<std::string, std::unique_ptr<Type>> _tempBuffer;
    size_t _currDepth = 1;
public:
    SymbolTable()
    {
        initializeIntrinsic();
    }

    void initializeIntrinsic();

    static Type* getArrayType(std::string& typeId);
    static Type* getNil();
private:
    std::map<std::string, Type*>& vars() { return _varTable[_currDepth - 1]; }
    std::map<std::string, Type*>& types() { return _typeTable[_currDepth - 1]; }
    std::map<std::string, FunctionDeclaration*>& funcs() { return _funcTable[_currDepth - 1]; }
public:
    bool addVariable(std::string const& name, std::string const& typeId)
    {
        if (vars().find(name) != vars().end())
        {
            return false;
        }
        auto ty = checkType(typeId);
        if (ty == nullptr)
        {
            return false;
        }
        return addVariable(name, ty);
    }
    bool addVariable(std::string const& name, Type* type)
    {
        if (vars().find(name) != vars().end())
        {
            return false;
        }
        vars().emplace(name, type);
        return true;
    }
    bool addType(std::string const& name, Type* type)
    {
        assert(type);
        if (types().find(name) != types().end())
        {
            return false;
        }
        types().emplace(name, type);
        return true;
    }
    bool addType(std::string const& name, std::string const& typeId)
    {
        if (types().find(name) != types().end())
        {
            return false;
        }
        auto ty = checkType(typeId);
        if (ty == nullptr)
        {
            return false;
        }
        return addType(name, ty);
    }
    bool addFunction(std::string const& name, FunctionDeclaration* decl)
    {
        if (funcs().find(name) != funcs().end())
        {
            return false;
        }
        funcs().emplace(name, decl);
        return true;
    }
    Type* checkType(std::string const& name)
    {
        return check(name, _typeTable);
    }
    Type* checkVariable(std::string const& name)
    {
        return check(name, _varTable);
    }
    FunctionDeclaration* checkFunction(std::string const& name)
    {
        return check(name, _funcTable);
    }

    void enterScope()
    {
        ++_currDepth;

        advance(_currDepth, _varTable);
        advance(_currDepth, _typeTable);
        advance(_currDepth, _funcTable);
    }

    void exitScope()
    {
        --_currDepth;
        assert(_currDepth >= 1);
    }
private:
    template<class T>
    static void advance(size_t depth, T& table)
    {
        if (table.size() < depth)
        {
            assert(table.size() + 1 == depth);
            table.emplace_back();
            table[depth - 1].clear();
        }
        else
        {
            table[depth - 1].clear();
        }
    }

    template<class T>
    typename T::mapped_type check(std::string const& name, std::vector<T>& table)
    {
        auto i = _currDepth;
        while (i--)
        {
            auto r = table[i].find(name);
            if (r != table[i].end())
            {
                return r->second;
            }
        }
        return nullptr;
    }
};

class ScopeGuard
{
    SymbolTable& ref;
public:
    ScopeGuard(SymbolTable& r)
        : ref(r)
    {
        ref.enterScope();
    }
    ScopeGuard(ScopeGuard const&) = delete;
    ~ScopeGuard()
    {
        ref.exitScope();
    }
};

}

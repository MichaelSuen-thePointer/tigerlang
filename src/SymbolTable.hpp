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
class IdentifierTypeDeclaration;
class SymbolTable
{
    std::vector<std::map<std::string, IdentifierTypeDeclaration*>> _varTable;
    std::vector<std::map<std::string, Type*>> _typeTable;
    std::vector<std::map<std::string, FunctionDeclaration*>> _funcTable;
    static std::map<std::string, std::unique_ptr<Type>> _tempBuffer;
    size_t _currDepth = 1;
public:
    SymbolTable();

    void initializeIntrinsic();

    static Type* getArrayType(std::string& typeId);
    static Type* getNil();
private:
    std::map<std::string, IdentifierTypeDeclaration*>& vars();
    std::map<std::string, Type*>& types();
    std::map<std::string, FunctionDeclaration*>& funcs();
public:
    bool addVariable(std::string const& name, std::string const& typeId);

    bool addVariable(std::string const& name, Type* type);

    bool addType(std::string const& name, Type* type);

    bool addType(std::string const& name, std::string const& typeId);

    bool addFunction(std::string const& name, FunctionDeclaration* decl);

    Type* checkType(std::string const& name);

    IdentifierTypeDeclaration* SymbolTable::checkVariable(std::string const& name, bool& inCurrScope);

    FunctionDeclaration* checkFunction(std::string const& name);

    void enterScope();

    void exitScope();
private:
    template<class T>
    static void advance(size_t depth, T& table);

    template <class T>
    typename T::mapped_type SymbolTable::check(std::string const& name, std::vector<T>& table, size_t& foundDepth);
};

template <class T>
void SymbolTable::advance(size_t depth, T& table)
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

template <class T>
typename T::mapped_type SymbolTable::check(std::string const& name, std::vector<T>& table, size_t& foundDepth)
{
    auto i = _currDepth;
    while (i--)
    {
        auto r = table[i].find(name);
        if (r != table[i].end())
        {
            foundDepth = i;
            return r->second;
        }
    }
    foundDepth = -1;
    return nullptr;
}

class ScopeGuard
{
    SymbolTable& ref;
public:
    ScopeGuard(SymbolTable& r);
    ScopeGuard(ScopeGuard const&) = delete;
    ~ScopeGuard();
};

}

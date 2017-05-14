#include "SymbolTable.hpp"
#include "TigerAST.hpp"
#include <memory>
namespace tiger
{
SymbolTable::SymbolTable()
{
    initializeIntrinsic();
}

void SymbolTable::initializeIntrinsic()
{
    _varTable.emplace_back();
    _typeTable.emplace_back();
    _funcTable.emplace_back();
    //functions
    std::vector<FunctionParameter> p;
    {
        static std::unique_ptr<FunctionDeclaration> printFunc{
            new FunctionDeclaration("print", p = {{"s", "string"}}, "", nullptr)
        };
        addFunction("print", printFunc.get());
    }
    {
        static std::unique_ptr<FunctionDeclaration> flushFunc{
            new FunctionDeclaration("flush", p = {}, "", nullptr)
        };
        addFunction("flush", flushFunc.get());
    }
    {
        static std::unique_ptr<FunctionDeclaration> getcharFunc{
            new FunctionDeclaration("getchar", p = {}, "string", nullptr)
        };
        addFunction("getchar", getcharFunc.get());
    }
    {
        static std::unique_ptr<FunctionDeclaration> ordFunc{
            new FunctionDeclaration("ord", p = {{"s", "string"}}, "int", nullptr)
        };
        addFunction("ord", ordFunc.get());
    }
    {
        static std::unique_ptr<FunctionDeclaration> chrFunc{
            new FunctionDeclaration("chr", p = { { "i", "int" } }, "string", nullptr)
        };
        addFunction("chr", chrFunc.get());
    }
    {
        static std::unique_ptr<FunctionDeclaration> sizeFunc{
            new FunctionDeclaration("size", p = { { "s", "string" } }, "int", nullptr)
        };
        addFunction("size", sizeFunc.get());
    }
    {
        static std::unique_ptr<FunctionDeclaration> substringFunc{
            new FunctionDeclaration("substring",
            p = {{ "s", "string" },{"first", "int"},{"n", "int"} },
            "string", nullptr)
        };
        addFunction("substring", substringFunc.get());
    }
    {
        static std::unique_ptr<FunctionDeclaration> concatFunc{
            new FunctionDeclaration("concat",
            p = { { "s1", "string" },{ "s2", "string" } },
            "string", nullptr)
        };
        addFunction("concat", concatFunc.get());
    }
    {
        static std::unique_ptr<FunctionDeclaration> notFunc{
            new FunctionDeclaration("not",
            p = { { "i", "int" } },
            "int", nullptr)
        };
        addFunction("not", notFunc.get());
    }
    {
        static std::unique_ptr<FunctionDeclaration> exitFunc{
            new FunctionDeclaration("exit",
            p = { { "i", "int" } },
            "", nullptr)
        };
        addFunction("exit", exitFunc.get());
    }
    //types
    static std::unique_ptr<IdentifierType> intType{
        new IdentifierType("int")
    };
    addType("int", intType.get());
    static std::unique_ptr<IdentifierType> stringType{
        new IdentifierType("string")
    };
    addType("string", stringType.get());
    addType("nil", getNil());
}

Type* SymbolTable::getArrayType(std::string& typeId)
{
    auto r = _tempBuffer.find(typeId);
    assert(typeId == "int" || typeId == "string");
    if (r == _tempBuffer.end())
    {
        auto p = new ArrayType(typeId);
        _tempBuffer.emplace(typeId, std::unique_ptr<Type>(p));
        return p;
    }
    auto p = r->second.get();
    assert(dynamic_cast<Type*>(p) != nullptr);
    return static_cast<Type*>(p);
}

Type* SymbolTable::getNil()
{
    std::vector<FieldDeclaration> empty;
    static std::unique_ptr<RecordType> nilType{
        new RecordType(empty)
    };
    return nilType.get();
}

std::map<std::string, IdentifierTypeDeclaration*>& SymbolTable::vars()
{
    return _varTable[_currDepth - 1];
}

std::map<std::string, Type*>& SymbolTable::types()
{
    return _typeTable[_currDepth - 1];
}

std::map<std::string, FunctionDeclaration*>& SymbolTable::funcs()
{
    return _funcTable[_currDepth - 1];
}
/*
bool SymbolTable::addVariable(std::string const& name, std::string const& typeId)
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
*/

bool SymbolTable::addVariable(std::string const& name, IdentifierTypeDeclaration* type)
{
    if (vars().find(name) != vars().end())
    {
        return false;
    }
    vars().emplace(name, type);
    return true;
}

bool SymbolTable::addLoopVariable(std::string const& name)
{
    assert(vars().find(name) == vars().end());
    assert(name != "int" && name != "string");
    auto p = new VariableDeclaration(name, "int", nullptr);

    _tempBuffer.emplace(name, std::unique_ptr<IdentifierTypeDeclaration>(p));

    vars().emplace(name, p);

    return true;
}

bool SymbolTable::addType(std::string const& name, Type* type)
{
    assert(type);
    if (types().find(name) != types().end())
    {
        return false;
    }
    types().emplace(name, type);
    return true;
}

bool SymbolTable::addType(std::string const& name, std::string const& typeId)
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

bool SymbolTable::addFunction(std::string const& name, FunctionDeclaration* decl)
{
    if (funcs().find(name) != funcs().end())
    {
        return false;
    }
    funcs().emplace(name, decl);
    return true;
}

Type* SymbolTable::checkType(std::string const& name)
{
    size_t ignore;
    return check(name, _typeTable, ignore);
}

IdentifierTypeDeclaration* SymbolTable::checkVariable(std::string const& name, bool& inCurrScope)
{
    size_t depth;
    auto r = check(name, _varTable, depth);
    inCurrScope = depth == _currDepth - 1;
    return r;
}

FunctionDeclaration* SymbolTable::checkFunction(std::string const& name)
{
    size_t ignore;
    return check(name, _funcTable, ignore);
}

void SymbolTable::enterScope()
{
    ++_currDepth;

    advance(_currDepth, _varTable);
    advance(_currDepth, _typeTable);
    advance(_currDepth, _funcTable);
}

void SymbolTable::exitScope()
{
    --_currDepth;
    assert(_currDepth >= 1);
}

ScopeGuard::ScopeGuard(SymbolTable& r): ref(r)
{
    ref.enterScope();
}

ScopeGuard::~ScopeGuard()
{
    ref.exitScope();
}

std::map<std::string, std::unique_ptr<ASTNode>> SymbolTable::_tempBuffer;

}

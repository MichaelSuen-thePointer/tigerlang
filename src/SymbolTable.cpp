#include "SymbolTable.hpp"
#include "TigerAST.hpp"
#include <memory>
namespace tiger
{
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
    if (r == _tempBuffer.end())
    {
        return _tempBuffer.emplace(typeId, new ArrayType(typeId)).first->second.get();
    }
    return r->second.get();
}

Type* SymbolTable::getNil()
{
    std::vector<FieldDeclaration> empty;
    static std::unique_ptr<RecordType> nilType{
        new RecordType(empty)
    };
    return nilType.get();
}

std::map<std::string, std::unique_ptr<Type>> SymbolTable::_tempBuffer;


}

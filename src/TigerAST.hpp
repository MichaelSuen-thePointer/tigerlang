#pragma once
#include <memory>
#include <string>
#include <vector>
#include "location.hh"
#include <cassert>
#include "SymbolTable.hpp"

namespace tiger
{
class SymbolTable;
class Type;
class ASTNode
{
protected:
    location _loc;
public:
    virtual ~ASTNode() = default;
    void loc(location const& loc);

    const location& loc() const;
    void semanticCheck();


    [[noreturn]]
    void semanticError(std::string const& msg) const;
public:
    virtual void semanticCheckImpl(SymbolTable& table);
};

class Expression;

class Program : public ASTNode
{
    std::unique_ptr<Expression> _exp;

public:
    explicit Program(Expression* r);

    const std::unique_ptr<Expression>& expression() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Expression : public ASTNode
{
protected:
    Type* _expressionType = nullptr;
    bool _lValue = false;
public:
    bool hasValue() const;

    const Type* expressionType() const;

    bool isLValue() const;

    Type* expressionType();
};

class Declaration : public ASTNode
{
public:
    virtual void semanticCheckInside(SymbolTable& table) = 0;
};

class IdentifierTypeDeclaration : public Declaration
{
protected:
    std::string _identifier;
    std::string _typeID;
    bool _escape = false;
public:
    IdentifierTypeDeclaration(std::string id, std::string typeID);

    const std::string& identifier() const;

    const std::string& typeId() const;

    bool escape() const;

    void escape(bool value);
};

class VariableDeclaration : public IdentifierTypeDeclaration
{
protected:
    std::unique_ptr<Expression> _initializer;

public:
    VariableDeclaration(std::string id, std::string typeID, Expression* exp);

    const std::unique_ptr<Expression>& initializer() const;
protected:
    void semanticCheckImpl(SymbolTable& table) override;

    void semanticCheckInside(SymbolTable& table) override;
};

class Type;

class TypeDeclaration : public Declaration
{
protected:
    std::string _typeID;
    std::unique_ptr<Type> _type;

public:
    TypeDeclaration(std::string typeID, Type* type);

    const std::string& typeId() const;

    const std::unique_ptr<Type>& type() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;

    void semanticCheckInside(SymbolTable& table) override;
};

class FunctionParameter : public IdentifierTypeDeclaration
{
public:
    FunctionParameter(std::string id, std::string typeID);

    void semanticCheckInside(SymbolTable& table) override;
};

class FunctionDeclaration : public Declaration
{
protected:
    std::string _identifier;
    std::vector<FunctionParameter> _parameters;
    std::string _returnType;
    std::unique_ptr<Expression> _body;
public:
    FunctionDeclaration(std::string identifier, std::vector<FunctionParameter>& params,
                        std::string returnType, Expression* body);

    const std::string& identifier() const;

    const std::vector<FunctionParameter>& parameters() const;

    const std::string& returnType() const;

    const std::unique_ptr<Expression>& body() const;
protected:
    void semanticCheckImpl(SymbolTable& table) override;

    void semanticCheckInside(SymbolTable& table) override;
};

class Type : public ASTNode
{
public:
    virtual bool isInt() const;
    virtual bool isString() const;
    virtual bool isArray() const;
    virtual bool isRecord() const;

    friend bool operator==(const Type& l, const Type& r);
    friend bool operator!=(const Type& l, const Type& r);

    enum Kind
    {
        Id, Array, Record
    };
    virtual Kind kind() const = 0;
};

class IdentifierType : public Type
{
protected:
    std::string _identifier;
public:
    explicit IdentifierType(std::string id);

    bool isInt() const override;

    bool isString() const override;

    const std::string& identifier() const;

    Kind kind() const override;
protected:

    void semanticCheckImpl(SymbolTable& table) override;
};

class ArrayType : public Type
{
protected:
    std::string _typeID;

public:
    explicit ArrayType(std::string id);
    bool isArray() const override;

    const std::string& typeId() const;

    Kind kind() const override;
protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class FieldDeclaration : public ASTNode
{
protected:
    std::string _identifier;
    std::string _typeID;

public:
    FieldDeclaration(std::string id, std::string typeID);

    const std::string& identifier() const;

    const std::string& typeId() const;
};

class RecordType : public Type
{
protected:
    std::vector<FieldDeclaration> _fields;

public:
    RecordType(std::vector<FieldDeclaration>& fields);
    bool isRecord() const override;

    const std::vector<FieldDeclaration>& fields() const;

    Kind kind() const override;
protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Identifier : public Expression
{
protected:
    std::string _identifier;

public:
    Identifier(std::string id);

    const std::string& identifier() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Subscript : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::unique_ptr<Expression> _right;

public:
    Subscript(Expression* l, Expression* r);

    const std::unique_ptr<Expression>& left() const;

    const std::unique_ptr<Expression>& right() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class FieldExpression : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::string _identifier;

public:
    FieldExpression(Expression* l, std::string r);

    const std::unique_ptr<Expression>& left() const;

    const std::string& identifier() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Nil : public Expression
{
public:
protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class IntLiteral : public Expression
{
protected:
    int _value;

public:
    explicit IntLiteral(int v);

    int value() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class StringLiteral : public Expression
{
protected:
    std::string _value;

public:
    explicit StringLiteral(std::string v);

    const std::string& value() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Sequence : public Expression
{
protected:
    std::vector<std::unique_ptr<Expression>> _expressions;

public:
    explicit Sequence(std::vector<Expression *>& exp);

    const std::vector<std::unique_ptr<Expression>>& expressions() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Negation : public Expression
{
protected:
    std::unique_ptr<Expression> _expression;

public:
    explicit Negation(Expression* exp);

    const std::unique_ptr<Expression>& expression() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Call : public Expression
{
protected:
    std::string _identifier;
    std::vector<std::unique_ptr<Expression>> _arguments;

public:
    Call(std::string id, std::vector<Expression *> args);

    const std::string& identifier() const;

    const std::vector<std::unique_ptr<Expression>>& arguments() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Infix : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::unique_ptr<Expression> _right;

public:
    Infix(Expression* l, Expression* r);

    const std::unique_ptr<Expression>& left() const;

    const std::unique_ptr<Expression>& right() const;

protected:
    virtual void semanticCheckInfix(SymbolTable& table);
    void semanticCheckImpl(SymbolTable& table) override final;
};

class Eq : public Infix
{
public:
    Eq(Expression* l, Expression* r);

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Ne : public Infix
{
public:
    Ne(Expression* l, Expression* r);

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Gt : public Infix
{
public:
    Gt(Expression* l, Expression* r);

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Ge : public Infix
{
public:
    Ge(Expression* l, Expression* r);

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Lt : public Infix
{
public:
    Lt(Expression* l, Expression* r);

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Le : public Infix
{
public:
    Le(Expression* l, Expression* r);

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Add : public Infix
{
public:
    Add(Expression* l, Expression* r);
};
class Sub : public Infix
{
public:
    Sub(Expression* l, Expression* r);
};
class Mul : public Infix
{
public:
    Mul(Expression* l, Expression* r);
};
class Div : public Infix
{
public:
    Div(Expression* l, Expression* r);
};

class And : public Infix
{
public:
    And(Expression* l, Expression* r);
};

class Or : public Infix
{
public:
    Or(Expression* l, Expression* r);
};

class ArrayCreate : public Expression
{
protected:
    std::string _typeID;
    std::unique_ptr<Expression> _size;
    std::unique_ptr<Expression> _initializer;

public:
    ArrayCreate(std::string typeId, Expression* expression, Expression* initializer);

    const std::string& typeId() const;

    const std::unique_ptr<Expression>& size() const;

    const std::unique_ptr<Expression>& initializer() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class FieldInitializer : public ASTNode
{
protected:
    std::string _identifier;
    std::unique_ptr<Expression> _initializer;
public:
    FieldInitializer(std::string id, Expression* initializer);

    const std::string& identifier() const;

    const std::unique_ptr<Expression>& initializer() const;
};

class FieldCreate : public ASTNode
{
protected:
    std::string _identifier;
    std::unique_ptr<Expression> _initializer;

public:
    FieldCreate(std::string id, Expression* init);

    const std::string& identifier() const;

    const std::unique_ptr<Expression>& initializer() const;
};

class Break : public Expression
{
public:
protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class RecordCreate : public Expression
{
protected:
    std::string _typeID;
    std::vector<FieldCreate> _fields;

public:
    RecordCreate(std::string id, std::vector<std::tuple<std::string, Expression*, location>>& fields);

    const std::string& typeId() const;

    const std::vector<FieldCreate>& fields() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Assignment : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::unique_ptr<Expression> _right;

public:
    Assignment(Expression* l, Expression* r);

    const std::unique_ptr<Expression>& left() const;

    const std::unique_ptr<Expression>& right() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class IfThenElse : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _thenBranch;
    std::unique_ptr<Expression> _elseBranch;

public:
    IfThenElse(Expression* cond, Expression* then, Expression* els);

    const std::unique_ptr<Expression>& condition() const;

    const std::unique_ptr<Expression>& thenBranch() const;

    const std::unique_ptr<Expression>& elseBranch() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class IfThen : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _thenBranch;

public:
    IfThen(Expression* cond, Expression* then);

    const std::unique_ptr<Expression>& condition() const;

    const std::unique_ptr<Expression>& thenBranch() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class While : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _body;

public:
    While(Expression* cond, Expression* body);

    const std::unique_ptr<Expression>& condition() const;

    const std::unique_ptr<Expression>& body() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class For : public Expression
{
protected:
    std::string _identifier;
    std::unique_ptr<Expression> _lowerBound;
    std::unique_ptr<Expression> _upperBound;
    std::unique_ptr<Expression> _body;

public:
    For(std::string id, Expression* l, Expression* r, Expression* body);

    const std::string& identifier() const;

    const std::unique_ptr<Expression>& lowerBound() const;

    const std::unique_ptr<Expression>& upperBound() const;

    const std::unique_ptr<Expression>& body() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Let : public Expression
{
protected:
    std::vector<std::unique_ptr<Declaration>> _bindings;
    std::vector<std::unique_ptr<Expression>> _body;

public:
    Let(std::vector<Declaration *> bindings, std::vector<Expression *> body);

    const std::vector<std::unique_ptr<Declaration>>& bindings() const;

    const std::vector<std::unique_ptr<Expression>>& body() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

}
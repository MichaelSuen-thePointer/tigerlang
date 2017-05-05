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
    void loc(location const& loc)
    {
        _loc = loc;
    }
    const location& loc() const
    {
        return _loc;
    }
    void semanticCheck();

public:
    virtual void semanticCheckImpl(SymbolTable& table)
    {
        assert(0);
    }
    [[noreturn]]
    void semanticError(std::string const& msg) const;
};

class Expression;

class Program : public ASTNode
{
    std::unique_ptr<Expression> _exp;

public:
    explicit Program(Expression *r)
        : _exp(r)
    {
    }

    const std::unique_ptr<Expression>& expression() const
    {
        return _exp;
    }


protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Expression : public ASTNode
{
protected:
    Type* _expressionType = nullptr;
    bool _lValue = false;
public:
    bool hasValue() const
    {
        return _expressionType != nullptr;
    }
    const Type* expressionType() const
    {
        assert(_expressionType);
        return _expressionType;
    }
    bool isLValue() const
    {
        return _lValue;
    }
    Type* expressionType()
    {
        if (_expressionType == nullptr)
        {
            assert(_expressionType);
        }
        return _expressionType;
    }
};

class Declaration : public ASTNode
{
public:
    virtual void semanticCheckInside(SymbolTable& table) = 0;
};

class VariableDeclaration : public Declaration
{
protected:
    std::string _identifier;
    std::string _typeID;
    std::unique_ptr<Expression> _initializer;

    bool _escape = true;
public:
    bool escape() const
    {
        return _escape;
    }

    VariableDeclaration(std::string id, std::string typeID, Expression *exp)
        : _identifier(std::move(id)), _typeID(std::move(typeID)), _initializer(exp)
    {
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

    const std::string& typeId() const
    {
        return _typeID;
    }

    const std::unique_ptr<Expression>& initializer() const
    {
        return _initializer;
    }
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
    TypeDeclaration(std::string typeID, Type *type)
        : _typeID(std::move(typeID)), _type(type)
    {
    }

    const std::string& typeId() const
    {
        return _typeID;
    }

    const std::unique_ptr<Type>& type() const
    {
        return _type;
    }

protected:
    void semanticCheckImpl(SymbolTable& table) override;

    void semanticCheckInside(SymbolTable& table) override;
};

class FunctionParameter : public ASTNode
{
protected:
    std::string _identifier;
    std::string _typeID;

public:
    FunctionParameter(std::string id, std::string typeID)
        : _identifier(std::move(id)), _typeID(std::move(typeID))
    {
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

    const std::string& typeId() const
    {
        return _typeID;
    }

};

class FunctionDeclaration : public Declaration
{
protected:
    std::string _identifier;
    std::vector<FunctionParameter> _parameters;
    std::string _returnType;
    std::unique_ptr<Expression> _body;
public:
    FunctionDeclaration(std::string identifier, std::vector<FunctionParameter> &params,
        std::string returnType, Expression *body)
        : _identifier(std::move(identifier)), _parameters(std::move(params)), _returnType(std::move(returnType)), _body(body)
    {
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

    const std::vector<FunctionParameter>& parameters() const
    {
        return _parameters;
    }

    const std::string& returnType() const
    {
        return _returnType;
    }

    const std::unique_ptr<Expression>& body() const
    {
        return _body;
    }
protected:
    void semanticCheckImpl(SymbolTable& table) override;

    void semanticCheckInside(SymbolTable& table) override;
};

class Type : public ASTNode
{
public:
    virtual bool isInt() const { return false; }
    virtual bool isString() const { return false; }
    virtual bool isArray() const { return false; }
    virtual bool isRecord() const { return false; }

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
    explicit IdentifierType(std::string id)
        : _identifier(std::move(id))
    {
    }
    bool isInt() const override
    {
        return _identifier == "int";
    }
    bool isString() const override
    {
        return _identifier == "string";
    }
    const std::string& identifier() const
    {
        return _identifier;
    }

    Kind kind() const override
    {
        return Id;
    }
    void semanticCheckImpl(SymbolTable& table) override;
};

class ArrayType : public Type
{
protected:
    std::string _typeID;

public:
    explicit ArrayType(std::string id)
        : _typeID(std::move(id))
    {
    }
    bool isArray() const override { return true; }
    const std::string& typeId() const
    {
        return _typeID;
    }
    Kind kind() const override
    {
        return Array;
    }
    void semanticCheckImpl(SymbolTable& table) override;
};

class FieldDeclaration : public ASTNode
{
protected:
    std::string _identifier;
    std::string _typeID;

public:
    FieldDeclaration(std::string id, std::string typeID)
        : _identifier(std::move(id)), _typeID(std::move(typeID))
    {
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

    const std::string& typeId() const
    {
        return _typeID;
    }

};

class RecordType : public Type
{
protected:
    std::vector<FieldDeclaration> _fields;

public:
    RecordType(std::vector<FieldDeclaration> &fields)
        : _fields(std::move(fields))
    {
    }
    bool isRecord() const override { return true; }
    const std::vector<FieldDeclaration>& fields() const
    {
        return _fields;
    }
    Kind kind() const override
    {
        return Record;
    }
    void semanticCheckImpl(SymbolTable& table) override;
};

class Identifier : public Expression
{
protected:
    std::string _identifier;

public:
    Identifier(std::string id)
        : _identifier(std::move(id))
    {
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Subscript : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::unique_ptr<Expression> _right;

public:
    Subscript(Expression *l, Expression *r)
        : _left(l), _right(r)
    {
    }

    const std::unique_ptr<Expression>& left() const
    {
        return _left;
    }

    const std::unique_ptr<Expression>& right() const
    {
        return _right;
    }

protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class FieldExpression : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::string _identifier;

public:
    FieldExpression(Expression *l, std::string r)
        : _left(l), _identifier(std::move(r))
    {
    }

    const std::unique_ptr<Expression>& left() const
    {
        return _left;
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class Nil : public Expression
{
public:
    void semanticCheckImpl(SymbolTable& table) override;
};

class IntLiteral : public Expression
{
protected:
    int _value;

public:
    explicit IntLiteral(int v)
        : _value(v)
    {
    }

    int value() const
    {
        return _value;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class StringLiteral : public Expression
{
protected:
    std::string _value;

public:
    explicit StringLiteral(std::string v)
        : _value(v)
    {
    }

    const std::string& value() const
    {
        return _value;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class Sequence : public Expression
{
protected:
    std::vector<std::unique_ptr<Expression>> _expressions;

public:
    explicit Sequence(std::vector<Expression *> &exp)
        : _expressions(exp.begin(), exp.end())
    {
    }

    const std::vector<std::unique_ptr<Expression>>& expressions() const
    {
        return _expressions;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class Negation : public Expression
{
protected:
    std::unique_ptr<Expression> _expression;

public:
    explicit Negation(Expression *exp)
        : _expression(exp)
    {
    }

    const std::unique_ptr<Expression>& expression() const
    {
        return _expression;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class Call : public Expression
{
protected:
    std::string _identifier;
    std::vector<std::unique_ptr<Expression>> _arguments;

public:
    Call(std::string id, std::vector<Expression *> args)
        : _identifier(std::move(id)), _arguments(args.begin(), args.end())
    {
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

    const std::vector<std::unique_ptr<Expression>>& arguments() const
    {
        return _arguments;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class Infix : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::unique_ptr<Expression> _right;

public:
    Infix(Expression *l, Expression *r)
        : _left(l), _right(r)
    {
    }

    const std::unique_ptr<Expression>& left() const
    {
        return _left;
    }

    const std::unique_ptr<Expression>& right() const
    {
        return _right;
    }

protected:
    virtual void semanticCheckInfix(SymbolTable& table);
    void semanticCheckImpl(SymbolTable& table) override final;
};

class Eq : public Infix
{
public:
    Eq(Expression *l, Expression *r) : Infix(l, r)
    {
    }

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Ne : public Infix
{
public:
    Ne(Expression *l, Expression *r) : Infix(l, r)
    {
    }

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Gt : public Infix
{
public:
    Gt(Expression *l, Expression *r) : Infix(l, r)
    {
    }

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Ge : public Infix
{
public:
    Ge(Expression *l, Expression *r) : Infix(l, r)
    {
    }

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Lt : public Infix
{
public:
    Lt(Expression *l, Expression *r) : Infix(l, r)
    {
    }

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Le : public Infix
{
public:
    Le(Expression *l, Expression *r) : Infix(l, r)
    {
    }

protected:
    void semanticCheckInfix(SymbolTable& table) override;
};
class Add : public Infix
{
public:
    Add(Expression *l, Expression *r) : Infix(l, r)
    {
    }

};
class Sub : public Infix
{
public:
    Sub(Expression *l, Expression *r) : Infix(l, r)
    {
    }

};
class Mul : public Infix
{
public:
    Mul(Expression *l, Expression *r) : Infix(l, r)
    {
    }

};
class Div : public Infix
{
public:
    Div(Expression *l, Expression *r) : Infix(l, r)
    {
    }

};

class And : public Infix
{
public:
    And(Expression *l, Expression *r)
        : Infix(l, r)
    {
    }

};

class Or : public Infix
{
public:
    Or(Expression *l, Expression *r)
        : Infix(l, r)
    {
    }

};

class ArrayCreate : public Expression
{
protected:
    std::string _typeID;
    std::unique_ptr<Expression> _size;
    std::unique_ptr<Expression> _initializer;

public:
    ArrayCreate(std::string typeId, Expression *expression, Expression *initializer)
        : _typeID(std::move(typeId)), _size(expression), _initializer(initializer)
    {
    }

    const std::string& typeId() const
    {
        return _typeID;
    }

    const std::unique_ptr<Expression>& size() const
    {
        return _size;
    }

    const std::unique_ptr<Expression>& initializer() const
    {
        return _initializer;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class FieldInitializer : public ASTNode
{
protected:
    std::string _identifier;
    std::unique_ptr<Expression> _initializer;
public:
    FieldInitializer(std::string id, Expression* initializer)
        : _identifier(std::move(id))
        , _initializer(initializer)
    {
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

    const std::unique_ptr<Expression>& initializer() const
    {
        return _initializer;
    }
};

class FieldCreate : public ASTNode
{
protected:
    std::string _identifier;
    std::unique_ptr<Expression> _initializer;

public:
    FieldCreate(std::string id, Expression* init)
        : _identifier(std::move(id)), _initializer(init)
    {
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

    const std::unique_ptr<Expression>& initializer() const
    {
        return _initializer;
    }

};

class Break : public Expression
{
public:
    void semanticCheckImpl(SymbolTable& table) override;
};

class RecordCreate : public Expression
{
protected:
    std::string _typeID;
    std::vector<FieldCreate> _fields;

public:
    RecordCreate(std::string id, std::vector<std::tuple<std::string, Expression*, location>>& fields)
        : _typeID(std::move(id)), _fields()
    {
        _fields.reserve(fields.size());
        for (auto& p : fields)
        {
            _fields.emplace_back(std::get<0>(p), std::get<1>(p));
            _fields.back().loc(std::get<2>(p));
        }
    }

    const std::string& typeId() const
    {
        return _typeID;
    }

    const std::vector<FieldCreate>& fields() const
    {
        return _fields;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class Assignment : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::unique_ptr<Expression> _right;

public:
    Assignment(Expression *l, Expression *r)
        : _left(l), _right(r)
    {
    }

    const std::unique_ptr<Expression>& left() const
    {
        return _left;
    }

    const std::unique_ptr<Expression>& right() const
    {
        return _right;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class IfThenElse : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _thenBranch;
    std::unique_ptr<Expression> _elseBranch;

public:
    IfThenElse(Expression *cond, Expression *then, Expression *els)
        : _condition(cond), _thenBranch(then), _elseBranch(els)
    {
    }

    const std::unique_ptr<Expression>& condition() const
    {
        return _condition;
    }

    const std::unique_ptr<Expression>& thenBranch() const
    {
        return _thenBranch;
    }

    const std::unique_ptr<Expression>& elseBranch() const
    {
        return _elseBranch;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class IfThen : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _thenBranch;

public:
    IfThen(Expression *cond, Expression *then)
        : _condition(cond), _thenBranch(then)
    {
    }

    const std::unique_ptr<Expression>& condition() const
    {
        return _condition;
    }

    const std::unique_ptr<Expression>& thenBranch() const
    {
        return _thenBranch;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class While : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _body;

public:
    While(Expression *cond, Expression *body)
        : _condition(cond), _body(body)
    {
    }

    const std::unique_ptr<Expression>& condition() const
    {
        return _condition;
    }

    const std::unique_ptr<Expression>& body() const
    {
        return _body;
    }

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
    For(std::string id, Expression *l, Expression *r, Expression *body)
        : _identifier(std::move(id)), _lowerBound(l), _upperBound(r), _body(body)
    {
    }

    const std::string& identifier() const
    {
        return _identifier;
    }

    const std::unique_ptr<Expression>& lowerBound() const
    {
        return _lowerBound;
    }

    const std::unique_ptr<Expression>& upperBound() const
    {
        return _upperBound;
    }

    const std::unique_ptr<Expression>& body() const
    {
        return _body;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

class Let : public Expression
{
protected:
    std::vector<std::unique_ptr<Declaration>> _bindings;
    std::vector<std::unique_ptr<Expression>> _body;

public:
    Let(std::vector<Declaration *> bindings, std::vector<Expression *> body)
        : _bindings(bindings.begin(), bindings.end()), _body(body.begin(), body.end())
    {
    }

    const std::vector<std::unique_ptr<Declaration>>& bindings() const
    {
        return _bindings;
    }

    const std::vector<std::unique_ptr<Expression>>& body() const
    {
        return _body;
    }

    void semanticCheckImpl(SymbolTable& table) override;
};

}

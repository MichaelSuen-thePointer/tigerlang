#include <memory>
#include <string>
#include <vector>

namespace tiger
{

class ASTNode
{
public:
    virtual ~ASTNode() = default;
};

class Expression;

class Program : public ASTNode
{
    std::unique_ptr<Expression> _exp;
public:
    explicit Program(Expression* r)
        : _exp(r)
    {
    }
};

class Expression : public ASTNode
{
};

class Declaration : public ASTNode
{
};

class VariableDeclaration : public Declaration
{
protected:
    std::string _identifier;
    std::string _typeID;
    std::unique_ptr<Expression> _initializer;
public:
    VariableDeclaration(std::string id, std::string typeID, Expression* exp)
        : _identifier(std::move(id))
        , _typeID(std::move(typeID))
        , _initializer(exp)
    {
    }
};

class Type;

class TypeDeclaration : public Declaration
{
protected:
    std::string _typeID;
    std::unique_ptr<Type> _type;
public:
    TypeDeclaration(std::string typeID, Type* type)
        : _typeID(std::move(typeID))
        , _type(type)
    {
    }
};

class FieldDeclaration : public ASTNode
{
protected:
    std::string _identifier;
    std::string _typeID;
public:
    FieldDeclaration(std::string id, std::string typeID)
        : _identifier(std::move(id))
        , _typeID(std::move(typeID))
    {
    }
};

class FunctionDeclaration : public Declaration
{
protected:
    std::string _identifier;
    std::vector<FieldDeclaration> _parameters;
    std::string _returnType;
    std::unique_ptr<Expression> _body;
public:
    FunctionDeclaration(std::string identifier, std::vector<FieldDeclaration>& params,
        std::string returnType, Expression* body)
        : _identifier(std::move(identifier))
        , _parameters(std::move(params))
        , _returnType(std::move(returnType))
        , _body(body)
    {
    }
};

class Type : public ASTNode
{
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
};

class RecordType : public Type
{
protected:
    std::vector<FieldDeclaration> _fields;
public:
    RecordType(std::vector<FieldDeclaration>& fields)
        : _fields(std::move(fields))
    {
    }
};

class LValue : public Expression
{
};

class Identifier : public LValue
{
protected:
    std::string _identifier;
public:
    Identifier(std::string id)
        : _identifier(std::move(id))
    {
    }
};

class Subscript : public LValue
{
protected:
    std::unique_ptr<LValue> _left;
    std::unique_ptr<Expression> _right;
public:
    Subscript(LValue* l, Expression* r)
        : _left(l), _right(r)
    {
    }
};

class FieldExpression : public LValue
{
protected:
    std::unique_ptr<LValue> _left;
    std::string _identifier;
public:
    FieldExpression(LValue* l, std::string r)
        : _left(l)
        , _identifier(std::move(r))
    {
    }
};

class Nil : public Expression
{
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
};

class Sequence : public Expression
{
protected:
    std::vector<std::unique_ptr<Expression>> _expressions;
public:
    explicit Sequence(std::vector<Expression*>& exp)
        : _expressions(exp.begin(), exp.end())
    {
    }
};

class Negation : public Expression
{
protected:
    std::unique_ptr<Expression> _expression;
public:
    explicit Negation(Expression* exp)
        : _expression(exp)
    {
    }
};

class Call : public Expression
{
protected:
    std::string _identifier;
    std::vector<std::unique_ptr<Expression>> _arguments;
public:
    Call(std::string id, std::vector<Expression*> args)
        : _identifier(std::move(id))
        , _arguments(args.begin(), args.end())
    {
    }
};

class Infix : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::unique_ptr<Expression> _right;
public:
    Infix(Expression* l, Expression* r)
        : _left(l)
        , _right(r)
    {
    }
};

class Multiplicative : public Infix
{
public:
    Multiplicative(Expression* l, Expression* r)
        : Infix(l, r)
    {
    }
};

class Additive : public Infix
{
public:
    Additive(Expression* l, Expression* r)
        : Infix(l, r)
    {
    }
};

class Comparison : public Infix
{
public:
    Comparison(Expression* l, Expression* r)
        : Infix(l, r)
    {
    }
};

class LogicalAnd : public Infix
{
public:
    LogicalAnd(Expression* l, Expression* r)
        : Infix(l, r)
    {
    }
};

class LogicalOr : public Infix
{
public:
    LogicalOr(Expression* l, Expression* r)
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
    ArrayCreate(std::string typeId, Expression* expression, Expression* initializer)
        : _typeID(std::move(typeId))
        , _size(expression)
        , _initializer(initializer)
    {
    }
};

class FieldCreate : public ASTNode
{
protected:
    std::string _identifier;
    std::unique_ptr<Expression> _expression;
public:
    FieldCreate(std::string id, Expression* exp)
        : _identifier(std::move(id))
        , _expression(exp)
    {
    }
};

class Break : public Expression
{
};

class RecordCreate : public Expression
{
protected:
    std::string _typeID;
    std::vector<std::unique_ptr<FieldCreate>> _fields;
public:
    RecordCreate(std::string id, std::vector<FieldCreate*> fields)
        : _typeID(std::move(id))
        , _fields(fields.begin(), fields.end())
    {
    }
};

class Assignment : public Expression
{
protected:
    std::unique_ptr<LValue> _left;
    std::unique_ptr<Expression> _right;
public:
    Assignment(LValue* l, Expression* r)
        : _left(l)
        , _right(r)
    {
    }
};

class IfThenElse : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _thenBranch;
    std::unique_ptr<Expression> _elseBranch;
public:
    IfThenElse(Expression* cond, Expression* then, Expression* els)
        : _condition(cond)
        , _thenBranch(then)
        , _elseBranch(els)
    {
    }
};

class IfThen : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _thenBranch;
public:
    IfThen(Expression* cond, Expression* then)
        : _condition(cond)
        , _thenBranch(then)
    {
    }
};

class While : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _body;
public:
    While(Expression* cond, Expression* body)
        : _condition(cond)
        , _body(body)
    {
    }
};

class For : public Expression
{
protected:
    std::unique_ptr<Expression> _lowerBound;
    std::unique_ptr<Expression> _upperBound;
    std::unique_ptr<Expression> _body;
public:
    For(Expression* l, Expression* r, Expression* body)
        : _lowerBound(l)
        , _upperBound(r)
        , _body(body)
    {
    }
};

class Let : public Expression
{
protected:
    std::vector<std::unique_ptr<Declaration>> _bindings;
    std::vector<std::unique_ptr<Expression>> _body;
public:
    Let(std::vector<Declaration*> bindings, std::vector<Expression*> body)
        : _bindings(bindings.begin(), bindings.end())
        , _body(body.begin(), body.end())
    {
    }
};

}

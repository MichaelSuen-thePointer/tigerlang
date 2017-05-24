#pragma once
#include <memory>
#include <string>
#include <vector>
#include <cassert>
#include "location.hh"
namespace tiger
{
class Frame;
class SymbolTable;
class Type;
namespace ir
{
class IRTNode;
}
class ASTNode
{
protected:
    location _loc;
    int id;
private:
    static int autoIncreament;
public:
    virtual ~ASTNode() = default;
    ASTNode();
    void loc(location const& loc);

    const location& loc() const;
    void semanticCheck();

    [[noreturn]]
    void semanticError(std::string const& msg) const;
public:
    virtual void semanticCheckImpl(SymbolTable& table);
    virtual std::string graphviz();

    static std::string graphvizCode;
};

class Expression;

class Program : public ASTNode
{
    std::unique_ptr<Expression> _exp;

public:
    explicit Program(Expression* r);

    const std::unique_ptr<Expression>& expression() const;
    virtual std::string graphviz();

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

    virtual std::unique_ptr<ir::IRTNode> toIR(Frame& f) = 0;
    virtual std::string graphviz();
};

class Declaration : public ASTNode
{
public:
    virtual void semanticCheckInside(SymbolTable& table) = 0;

    virtual void addToFrame(Frame& f);
};

class IdentifierTypeDeclaration : public Declaration
{
protected:
    std::string _identifier;
    std::string _typeID;
    Type* _type = nullptr;
protected:
    bool _escape = false;
public:
    IdentifierTypeDeclaration(std::string id, std::string typeID);
    virtual std::string graphviz();

    const std::string& identifier() const;

    const std::string& typeId() const;

    bool escape() const;

    void escape(bool value);

    Type* type() const;
    void type(Type* value);
};

class VariableDeclaration : public IdentifierTypeDeclaration
{
protected:
    std::unique_ptr<Expression> _initializer;

public:
    VariableDeclaration(std::string id, std::string typeID, Expression* exp);
    virtual std::string graphviz();

    const std::unique_ptr<Expression>& initializer() const;
protected:
    void semanticCheckImpl(SymbolTable& table) override;

    void semanticCheckInside(SymbolTable& table) override;
public:
    void addToFrame(Frame& f) override;
};

class Type;

class TypeDeclaration : public Declaration
{
protected:
    std::string _typeID;
    std::unique_ptr<Type> _type;

public:
    TypeDeclaration(std::string typeID, Type* type);
    virtual std::string graphviz();

    const std::string& typeId() const;

    const std::unique_ptr<Type>& type() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;

    void semanticCheckInside(SymbolTable& table) override;
public:
    void addToFrame(Frame& f) override;
};

class FunctionParameter : public IdentifierTypeDeclaration
{
public:
    FunctionParameter(std::string id, std::string typeID);
    virtual std::string graphviz();

    void semanticCheckImpl(SymbolTable& table) override;

    void semanticCheckInside(SymbolTable& table) override;

    void addToFrame(Frame& f) override;
};

class FunctionDeclaration : public Declaration
{
protected:
    std::string _identifier;
    std::vector<FunctionParameter> _parameters;
    std::string _returnType;
    std::unique_ptr<Expression> _body;
    bool _isBuiltin = false;
public:
    FunctionDeclaration(std::string identifier, std::vector<FunctionParameter>& params,
        std::string returnType, Expression* body, bool isBuiltin = false);
    virtual std::string graphviz();

    const bool& isBuiltin() const;

    const std::string& identifier() const;

    std::string mangledName() const;

    const std::vector<FunctionParameter>& parameters() const;

    const std::string& returnType() const;

    const std::unique_ptr<Expression>& body() const;
protected:
    void semanticCheckImpl(SymbolTable& table) override;

    void semanticCheckInside(SymbolTable& table) override;
public:
    void addToFrame(Frame& f) override;
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
    virtual std::string graphviz();

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
    virtual std::string graphviz();

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
    virtual std::string graphviz();
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
    virtual std::string graphviz();

    const std::string& identifier() const;

    const std::string& typeId() const;
};

class RecordType : public Type
{
protected:
    std::vector<FieldDeclaration> _fields;

public:
    RecordType(std::vector<FieldDeclaration>& fields);
    virtual std::string graphviz();
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
    virtual std::string graphviz();

    const std::string& identifier() const;

    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
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
    virtual std::string graphviz();

    const std::unique_ptr<Expression>& left() const;

    const std::unique_ptr<Expression>& right() const;

    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
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
    virtual std::string graphviz();

    const std::unique_ptr<Expression>& left() const;

    const std::string& identifier() const;

    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
protected:
    void semanticCheckImpl(SymbolTable& table) override;
};

class Nil : public Expression
{
public:
protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
    virtual std::string graphviz();
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
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
    virtual std::string graphviz();
};

class StringLiteral : public Expression
{
protected:
    std::string _value;

public:
    explicit StringLiteral(std::string v);
    virtual std::string graphviz();

    const std::string& value() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
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
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
    virtual std::string graphviz();
};

class Negation : public Expression
{
protected:
    std::unique_ptr<Expression> _expression;

public:
    explicit Negation(Expression* exp);
    virtual std::string graphviz();

    const std::unique_ptr<Expression>& expression() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Call : public Expression
{
protected:
    std::string _identifier;
    std::vector<std::unique_ptr<Expression>> _arguments;
    FunctionDeclaration* _func = nullptr;
public:
    Call(std::string id, std::vector<Expression *> args);
    virtual std::string graphviz();

    const std::string& identifier() const;

    const std::vector<std::unique_ptr<Expression>>& arguments() const;
protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Infix : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::unique_ptr<Expression> _right;

public:
    Infix(Expression* l, Expression* r);
    virtual std::string graphviz();

    const std::unique_ptr<Expression>& left() const;

    const std::unique_ptr<Expression>& right() const;

protected:
    virtual void semanticCheckInfix(SymbolTable& table);
    void semanticCheckImpl(SymbolTable& table) override final;

    std::string _graphviz(std::string name);
};

class Eq : public Infix
{
public:
    Eq(Expression* l, Expression* r);
    virtual std::string graphviz();

protected:
    void semanticCheckInfix(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Ne : public Infix
{
public:
    Ne(Expression* l, Expression* r);
    virtual std::string graphviz();

protected:
    void semanticCheckInfix(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Gt : public Infix
{
public:
    Gt(Expression* l, Expression* r);

protected:
    void semanticCheckInfix(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
    virtual std::string graphviz();
};

class Ge : public Infix
{
public:
    Ge(Expression* l, Expression* r);

protected:
    void semanticCheckInfix(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
    virtual std::string graphviz();
};

class Lt : public Infix
{
public:
    Lt(Expression* l, Expression* r);
    virtual std::string graphviz();

protected:
    void semanticCheckInfix(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Le : public Infix
{
public:
    Le(Expression* l, Expression* r);
    virtual std::string graphviz();

protected:
    void semanticCheckInfix(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Add : public Infix
{
public:
    Add(Expression* l, Expression* r);
    virtual std::string graphviz();
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Sub : public Infix
{
public:
    Sub(Expression* l, Expression* r);
    virtual std::string graphviz();
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Mul : public Infix
{
public:
    Mul(Expression* l, Expression* r);
    virtual std::string graphviz();
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Div : public Infix
{
public:
    Div(Expression* l, Expression* r);
    virtual std::string graphviz();
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class And : public Infix
{
public:
    And(Expression* l, Expression* r);
    virtual std::string graphviz();
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Or : public Infix
{
public:
    Or(Expression* l, Expression* r);
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
    virtual std::string graphviz();
};

class ArrayCreate : public Expression
{
protected:
    std::string _typeID;
    std::unique_ptr<Expression> _size;
    std::unique_ptr<Expression> _initializer;

public:
    ArrayCreate(std::string typeId, Expression* expression, Expression* initializer);
    virtual std::string graphviz();

    const std::string& typeId() const;

    const std::unique_ptr<Expression>& size() const;

    const std::unique_ptr<Expression>& initializer() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class FieldInitializer : public ASTNode
{
protected:
    std::string _identifier;
    std::unique_ptr<Expression> _initializer;
public:
    FieldInitializer(std::string id, Expression* initializer);
    virtual std::string graphviz();

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
    virtual std::string graphviz();

    const std::string& identifier() const;

    const std::unique_ptr<Expression>& initializer() const;
};

class Break : public Expression
{
public:
protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
    virtual std::string graphviz();
};

class RecordCreate : public Expression
{
protected:
    std::string _typeID;
    std::vector<FieldCreate> _fields;

public:
    RecordCreate(std::string id, std::vector<std::tuple<std::string, Expression*, location>>& fields);
    virtual std::string graphviz();

    const std::string& typeId() const;

    const std::vector<FieldCreate>& fields() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Assignment : public Expression
{
protected:
    std::unique_ptr<Expression> _left;
    std::unique_ptr<Expression> _right;

public:
    Assignment(Expression* l, Expression* r);
    virtual std::string graphviz();

    const std::unique_ptr<Expression>& left() const;

    const std::unique_ptr<Expression>& right() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class IfThenElse : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _thenBranch;
    std::unique_ptr<Expression> _elseBranch;

public:
    IfThenElse(Expression* cond, Expression* then, Expression* els);
    virtual std::string graphviz();

    const std::unique_ptr<Expression>& condition() const;

    const std::unique_ptr<Expression>& thenBranch() const;

    const std::unique_ptr<Expression>& elseBranch() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class IfThen : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _thenBranch;

public:
    IfThen(Expression* cond, Expression* then);
    virtual std::string graphviz();

    const std::unique_ptr<Expression>& condition() const;

    const std::unique_ptr<Expression>& thenBranch() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class While : public Expression
{
protected:
    std::unique_ptr<Expression> _condition;
    std::unique_ptr<Expression> _body;

public:
    While(Expression* cond, Expression* body);
    virtual std::string graphviz();

    const std::unique_ptr<Expression>& condition() const;

    const std::unique_ptr<Expression>& body() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
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
    virtual std::string graphviz();

    const std::string& identifier() const;

    const std::unique_ptr<Expression>& lowerBound() const;

    const std::unique_ptr<Expression>& upperBound() const;

    const std::unique_ptr<Expression>& body() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

class Let : public Expression
{
protected:
    std::vector<std::unique_ptr<Declaration>> _bindings;
    std::vector<std::unique_ptr<Expression>> _body;

public:
    Let(std::vector<Declaration *> bindings, std::vector<Expression *> body);
    virtual std::string graphviz();

    const std::vector<std::unique_ptr<Declaration>>& bindings() const;

    const std::vector<std::unique_ptr<Expression>>& body() const;

protected:
    void semanticCheckImpl(SymbolTable& table) override;
public:
    std::unique_ptr<ir::IRTNode> toIR(Frame& f) override;
};

}

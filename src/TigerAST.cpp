#include "TigerAST.hpp"
#include "SymbolTable.hpp"
namespace tiger
{
void ASTNode::loc(location const& loc)
{
    _loc = loc;
}

const location& ASTNode::loc() const
{
    return _loc;
}

Program::Program(Expression* r)
    : _exp(r)
{
}

const std::unique_ptr<Expression>& Program::expression() const
{
    return _exp;
}

bool Expression::hasValue() const
{
    return _expressionType != nullptr;
}

const Type* Expression::expressionType() const
{
    assert(_expressionType);
    return _expressionType;
}

bool Expression::isLValue() const
{
    return _lValue;
}

Type* Expression::expressionType()
{
    if (_expressionType == nullptr)
    {
        assert(_expressionType);
    }
    return _expressionType;
}


IdentifierTypeDeclaration::IdentifierTypeDeclaration(std::string id, std::string typeID)
    : _identifier(std::move(id))
    , _typeID(std::move(typeID))
{
}

const std::string& IdentifierTypeDeclaration::identifier() const
{
    return _identifier;
}

const std::string& IdentifierTypeDeclaration::typeId() const
{
    return _typeID;
}

bool IdentifierTypeDeclaration::escape() const
{
    return _escape;
}

void IdentifierTypeDeclaration::escape(bool value)
{
    _escape = value;
}

void IdentifierTypeDeclaration::type(Type* value)
{
    _type = value;
}

VariableDeclaration::VariableDeclaration(std::string id, std::string typeID, Expression* exp)
    : IdentifierTypeDeclaration(std::move(id), std::move(typeID))
    , _initializer(exp)
{
}

const std::unique_ptr<Expression>& VariableDeclaration::initializer() const
{
    return _initializer;
}

TypeDeclaration::TypeDeclaration(std::string typeID, Type* type)
    : _typeID(std::move(typeID))
    , _type(type)
{
}

const std::string& TypeDeclaration::typeId() const
{
    return _typeID;
}

const std::unique_ptr<Type>& TypeDeclaration::type() const
{
    return _type;
}

FunctionParameter::FunctionParameter(std::string id, std::string typeID)
    : IdentifierTypeDeclaration(std::move(id), std::move(typeID))
{
}

void FunctionParameter::semanticCheckImpl(SymbolTable& table)
{
    _type = table.checkType(_typeID);
    assert(_type);
}

FunctionDeclaration::FunctionDeclaration(std::string identifier, std::vector<FunctionParameter>& params, std::string returnType, Expression* body)
    : _identifier(std::move(identifier))
    , _parameters(std::move(params))
    , _returnType(std::move(returnType))
    , _body(body)
{
}

const std::string& FunctionDeclaration::identifier() const
{
    return _identifier;
}

const std::vector<FunctionParameter>& FunctionDeclaration::parameters() const
{
    return _parameters;
}

const std::string& FunctionDeclaration::returnType() const
{
    return _returnType;
}

const std::unique_ptr<Expression>& FunctionDeclaration::body() const
{
    return _body;
}

bool Type::isInt() const
{
    return false;
}

bool Type::isString() const
{
    return false;
}

bool Type::isArray() const
{
    return false;
}

bool Type::isRecord() const
{
    return false;
}

IdentifierType::IdentifierType(std::string id)
    : _identifier(std::move(id))
{
}

bool IdentifierType::isInt() const
{
    return _identifier == "int";
}

bool IdentifierType::isString() const
{
    return _identifier == "string";
}

const std::string& IdentifierType::identifier() const
{
    return _identifier;
}

Type::Kind IdentifierType::kind() const
{
    return Id;
}

ArrayType::ArrayType(std::string id)
    : _typeID(std::move(id))
{
}

bool ArrayType::isArray() const
{
    return true;
}

const std::string& ArrayType::typeId() const
{
    return _typeID;
}

Type::Kind ArrayType::kind() const
{
    return Array;
}

FieldDeclaration::FieldDeclaration(std::string id, std::string typeID)
    : _identifier(std::move(id))
    , _typeID(std::move(typeID))
{
}

const std::string& FieldDeclaration::identifier() const
{
    return _identifier;
}

const std::string& FieldDeclaration::typeId() const
{
    return _typeID;
}

RecordType::RecordType(std::vector<FieldDeclaration>& fields)
    : _fields(std::move(fields))
{
}

bool RecordType::isRecord() const
{
    return true;
}

const std::vector<FieldDeclaration>& RecordType::fields() const
{
    return _fields;
}

Type::Kind RecordType::kind() const
{
    return Record;
}

Identifier::Identifier(std::string id)
    : _identifier(std::move(id))
{
}

const std::string& Identifier::identifier() const
{
    return _identifier;
}

Subscript::Subscript(Expression* l, Expression* r)
    : _left(l)
    , _right(r)
{
}

const std::unique_ptr<Expression>& Subscript::left() const
{
    return _left;
}

const std::unique_ptr<Expression>& Subscript::right() const
{
    return _right;
}

FieldExpression::FieldExpression(Expression* l, std::string r)
    : _left(l)
    , _identifier(std::move(r))
{
}

const std::unique_ptr<Expression>& FieldExpression::left() const
{
    return _left;
}

const std::string& FieldExpression::identifier() const
{
    return _identifier;
}

IntLiteral::IntLiteral(int v)
    : _value(v)
{
}

int IntLiteral::value() const
{
    return _value;
}

StringLiteral::StringLiteral(std::string v)
    : _value(v)
{
}

const std::string& StringLiteral::value() const
{
    return _value;
}

Sequence::Sequence(std::vector<Expression*>& exp)
    : _expressions(exp.begin(), exp.end())
{
}

const std::vector<std::unique_ptr<Expression>>& Sequence::expressions() const
{
    return _expressions;
}

Negation::Negation(Expression* exp)
    : _expression(exp)
{
}

const std::unique_ptr<Expression>& Negation::expression() const
{
    return _expression;
}

Call::Call(std::string id, std::vector<Expression*> args)
    : _identifier(std::move(id))
    , _arguments(args.begin(), args.end())
{
}

const std::string& Call::identifier() const
{
    return _identifier;
}

const std::vector<std::unique_ptr<Expression>>& Call::arguments() const
{
    return _arguments;
}

Infix::Infix(Expression* l, Expression* r)
    : _left(l)
    , _right(r)
{
}

const std::unique_ptr<Expression>& Infix::left() const
{
    return _left;
}

const std::unique_ptr<Expression>& Infix::right() const
{
    return _right;
}

Eq::Eq(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Ne::Ne(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Gt::Gt(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Ge::Ge(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Lt::Lt(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Le::Le(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Add::Add(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Sub::Sub(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Mul::Mul(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Div::Div(Expression* l, Expression* r)
    : Infix(l, r)
{
}

And::And(Expression* l, Expression* r)
    : Infix(l, r)
{
}

Or::Or(Expression* l, Expression* r)
    : Infix(l, r)
{
}

ArrayCreate::ArrayCreate(std::string typeId, Expression* expression, Expression* initializer)
    : _typeID(std::move(typeId))
    , _size(expression)
    , _initializer(initializer)
{
}

const std::string& ArrayCreate::typeId() const
{
    return _typeID;
}

const std::unique_ptr<Expression>& ArrayCreate::size() const
{
    return _size;
}

const std::unique_ptr<Expression>& ArrayCreate::initializer() const
{
    return _initializer;
}

FieldInitializer::FieldInitializer(std::string id, Expression* initializer)
    : _identifier(std::move(id))
    , _initializer(initializer)
{
}

const std::string& FieldInitializer::identifier() const
{
    return _identifier;
}

const std::unique_ptr<Expression>& FieldInitializer::initializer() const
{
    return _initializer;
}

FieldCreate::FieldCreate(std::string id, Expression* init)
    : _identifier(std::move(id))
    , _initializer(init)
{
}

const std::string& FieldCreate::identifier() const
{
    return _identifier;
}

const std::unique_ptr<Expression>& FieldCreate::initializer() const
{
    return _initializer;
}

RecordCreate::RecordCreate(std::string id, std::vector<std::tuple<std::string, Expression*, location>>& fields)
    : _typeID(std::move(id))
    , _fields()
{
    _fields.reserve(fields.size());
    for (auto& p : fields)
    {
        _fields.emplace_back(std::get<0>(p), std::get<1>(p));
        _fields.back().loc(std::get<2>(p));
    }
}

const std::string& RecordCreate::typeId() const
{
    return _typeID;
}

const std::vector<FieldCreate>& RecordCreate::fields() const
{
    return _fields;
}

Assignment::Assignment(Expression* l, Expression* r)
    : _left(l)
    , _right(r)
{
}

const std::unique_ptr<Expression>& Assignment::left() const
{
    return _left;
}

const std::unique_ptr<Expression>& Assignment::right() const
{
    return _right;
}

IfThenElse::IfThenElse(Expression* cond, Expression* then, Expression* els)
    : _condition(cond)
    , _thenBranch(then)
    , _elseBranch(els)
{
}

const std::unique_ptr<Expression>& IfThenElse::condition() const
{
    return _condition;
}

const std::unique_ptr<Expression>& IfThenElse::thenBranch() const
{
    return _thenBranch;
}

const std::unique_ptr<Expression>& IfThenElse::elseBranch() const
{
    return _elseBranch;
}

IfThen::IfThen(Expression* cond, Expression* then)
    : _condition(cond)
    , _thenBranch(then)
{
}

const std::unique_ptr<Expression>& IfThen::condition() const
{
    return _condition;
}

const std::unique_ptr<Expression>& IfThen::thenBranch() const
{
    return _thenBranch;
}

While::While(Expression* cond, Expression* body)
    : _condition(cond)
    , _body(body)
{
}

const std::unique_ptr<Expression>& While::condition() const
{
    return _condition;
}

const std::unique_ptr<Expression>& While::body() const
{
    return _body;
}

For::For(std::string id, Expression* l, Expression* r, Expression* body)
    : _identifier(std::move(id))
    , _lowerBound(l)
    , _upperBound(r)
    , _body(body)
{
}

const std::string& For::identifier() const
{
    return _identifier;
}

const std::unique_ptr<Expression>& For::lowerBound() const
{
    return _lowerBound;
}

const std::unique_ptr<Expression>& For::upperBound() const
{
    return _upperBound;
}

const std::unique_ptr<Expression>& For::body() const
{
    return _body;
}

Let::Let(std::vector<Declaration*> bindings, std::vector<Expression*> body)
    : _bindings(bindings.begin(), bindings.end())
    , _body(body.begin(), body.end())
{
}

const std::vector<std::unique_ptr<Declaration>>& Let::bindings() const
{
    return _bindings;
}

const std::vector<std::unique_ptr<Expression>>& Let::body() const
{
    return _body;
}


bool operator==(const Type& l, const Type& r)
{
    auto lk = l.kind();
    auto rk = l.kind();
    if (lk != rk)
    {
        return false;
    }
    switch (lk)
    {
    case Type::Id:
    {
        auto& lt = static_cast<const IdentifierType&>(l);
        auto& rt = static_cast<const IdentifierType&>(r);
        return lt.identifier() == rt.identifier();
    }
    case Type::Array:
    {
        auto& lt = static_cast<const ArrayType&>(l);
        auto& rt = static_cast<const ArrayType&>(r);
        return lt.typeId() == rt.typeId();
    }
    case Type::Record:
    {
        if (&r == SymbolTable::getNil())
        {
            return true;
        }
        return &l == &r;
    }
    }
    assert(0);
    return false;
}

bool operator!=(const Type& l, const Type& r)
{
    return !(l == r);
}


    int ASTNode::autoIncreament = 0;
    std::string ASTNode::graphvizCode = "";

    std::string ASTNode::graphviz() {
        graphvizCode = "digraph{}\n";
        return "";
    }

    std::string Program::graphviz() {
        graphvizCode = "digraph {\n";
        _exp->graphviz();
        graphvizCode += "}\n";
        return "";
    }

    std::string Expression::graphviz() {
        std::string name = "Expression" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string IdentifierTypeDeclaration::graphviz() {
        std::string name = "IdentifierTypeDeclaration" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string VariableDeclaration::graphviz() {
        std::string name = "VariableDeclaration" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _initializer->graphviz() + "\n";
        return name;
    }

    std::string TypeDeclaration::graphviz() {
        std::string name = "TypeDeclaration" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string FunctionParameter::graphviz() {
        std::string name = "FunctionParameter" + id;
        graphvizCode += name + "\n";
        return name;
    }

    std::string FunctionDeclaration::graphviz() {
        std::string name = "FunctionDeclaration" + id;
        graphvizCode += name + "\n";
        for (auto& fp : _parameters) {
            graphvizCode += name + "->" + fp.graphviz() + "\n";
        }
        graphvizCode += name + "->" + _body->graphviz() + "\n";
        return name;
    }

    std::string Type::graphviz() {
        std::string name = "Type" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string IdentifierType::graphviz() {
        std::string name = "IdentifierType" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string ArrayType::graphviz() {
        std::string name = "ArrayType" + id;
        graphvizCode += name + "\n";
        return name;
    }

    std::string FieldDeclaration::graphviz() {
        std::string name = "FieldDeclaration" + id;
        graphvizCode += name + "\n";
        return name;
    }

    std::string RecordType::graphviz() {
        std::string name = "RecordType" + std::to_string(id);
        graphvizCode += name + "\n";
        for (auto& fd : _fields) {
            graphvizCode += name + "->" + fd.graphviz() + "\n";
        }
        return name;
    }

    std::string Identifier::graphviz() {
        std::string name = "Identifier" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string Subscript::graphviz() {
        std::string name = "Subscript" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _left->graphviz() + "\n";
        graphvizCode += name + "->" + _right->graphviz() + "\n";
        return name;
    }

    std::string FieldExpression::graphviz() {
        std::string name = "FieldExpression" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _left->graphviz() + "\n";
        return name;
    }

    std::string Nil::graphviz() {
        std::string name = "Nil" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string IntLiteral::graphviz() {
        std::string name = "IntLiteral" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string StringLiteral::graphviz() {
        std::string name = "StringLiteral" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string Sequence::graphviz() {
        std::string name = "Sequence" + std::to_string(id);
        graphvizCode += name + "\n";
        for (auto& e : _expressions) {
            graphvizCode += name + "->" + e->graphviz() + "\n";
        }
        return name;
    }

    std::string Negation::graphviz() {
        std::string name = "Negation" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _expression->graphviz() + "\n";
        return name;
    }

    std::string Call::graphviz() {
        std::string name = "Call" + std::to_string(id);
        graphvizCode += name + "\n";
        for (auto& a : _arguments) {
            graphvizCode += name + "->" + a->graphviz() + "\n";
        }
        return name;
    }

    std::string Infix::_graphviz(std::string name) {
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _left->graphviz() + "\n";
        graphvizCode += name + "->" + _right->graphviz() + "\n";
        return name;
    }

    std::string Infix::graphviz() {
        std::string name = "Infix" + std::to_string(id);
        return _graphviz(name);
    }

    std::string Eq::graphviz() {
        return _graphviz("Eq" + std::to_string(id));
    }

    std::string Ne::graphviz() {
        return _graphviz("Ne" + std::to_string(id));
    }

    std::string Gt::graphviz() {
        return _graphviz("Gt" + std::to_string(id));
    }

    std::string Ge::graphviz() {
        return _graphviz("Ge" + std::to_string(id));
    }

    std::string Lt::graphviz() {
        return _graphviz("Lt" + std::to_string(id));
    }

    std::string Le::graphviz() {
        return _graphviz("Le" + std::to_string(id));
    }

    std::string Add::graphviz() {
        return _graphviz("Add" + std::to_string(id));
    }

    std::string Sub::graphviz() {
        return _graphviz("Sub" + std::to_string(id));
    }

    std::string Mul::graphviz() {
        return _graphviz("Mul" + std::to_string(id));
    }

    std::string Div::graphviz() {
        return _graphviz("Div" + std::to_string(id));
    }

    std::string And::graphviz() {
        return _graphviz("And" + std::to_string(id));
    }

    std::string Or::graphviz() {
        return _graphviz("Or" + std::to_string(id));
    }

    std::string ArrayCreate::graphviz() {
        std::string name = "ArrayCreate" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _initializer->graphviz() + "\n";
        graphvizCode += name + "->" + _size->graphviz() + "\n";
        return name;
    }

    std::string FieldInitializer::graphviz() {
        std::string name = "FieldInitializer" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _initializer->graphviz() + "\n";
        return name;
    }

    std::string FieldCreate::graphviz() {
        std::string name = "FiledCreate" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _initializer->graphviz() + "\n";
        return name;
    }

    std::string Break::graphviz() {
        std::string name = "Break" + std::to_string(id);
        graphvizCode += name + "\n";
        return name;
    }

    std::string RecordCreate::graphviz() {
        std::string name = "RecordCreate" + std::to_string(id);
        graphvizCode += name + "\n";
        for (auto& fc : _fields) {
            graphvizCode += name + "->" + fc.graphviz() + "\n";
        }
        return name;
    }

    std::string Assignment::graphviz() {
        std::string name = "Assignment" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _left->graphviz() + "\n";
        graphvizCode += name + "->" + _right->graphviz() + "\n";
        return name;
    }

    std::string IfThenElse::graphviz() {
        std::string name = "IfThenElse" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _condition->graphviz() + "\n";
        graphvizCode += name + "->" + _thenBranch->graphviz() + "\n";
        graphvizCode += name + "->" + _elseBranch->graphviz() + "\n";
        return name;
    }

    std::string IfThen::graphviz() {
        std::string name = "IfThen" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _condition->graphviz() + "\n";
        graphvizCode += name + "->" + _thenBranch->graphviz() + "\n";
        return name;
    }

    std::string While::graphviz() {
        std::string name = "While" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _condition->graphviz() + "\n";
        graphvizCode += name + "->" + _body->graphviz() + "\n";
        return name;
    }

    std::string For::graphviz() {
        std::string name = "For" + std::to_string(id);
        graphvizCode += name + "\n";
        graphvizCode += name + "->" + _lowerBound->graphviz() + "\n";
        graphvizCode += name + "->" + _upperBound->graphviz() + "\n";
        graphvizCode += name + "->" + _body->graphviz() + "\n";
        return name;
    }

    std::string Let::graphviz() {
        std::string name = "Let" + std::to_string(id);
        graphvizCode += name + "\n";
        for (auto& b : _bindings) {
            graphvizCode += name + "->" + b->graphviz() + "\n";
        }
        for (auto& b : _body) {
            graphvizCode += name + "->" + b->graphviz() + "\n";
        }
        return name;
    }
}

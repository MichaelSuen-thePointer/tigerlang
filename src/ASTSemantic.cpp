#include <sstream>
#include <set>
#include "TigerAST.hpp"
#include "SymbolTable.hpp"

namespace tiger
{

void ASTNode::semanticCheckImpl(SymbolTable& table)
{
    assert(0);
}


void ASTNode::semanticCheck()
{
    SymbolTable table;
    semanticCheckImpl(table);
}

void checkHasValue(const std::unique_ptr<Expression>& exp)
{
    if (!exp->hasValue())
    {
        exp->semanticError("Expression should has value");
    }
}

[[noreturn]]
void ASTNode::semanticError(std::string const& msg) const
{
    std::ostringstream ostr;
    ostr << msg << " at " << loc();
    throw std::runtime_error(ostr.str());
}

void Program::semanticCheckImpl(SymbolTable& table)
{
    _exp->semanticCheckImpl(table);
}

Type* IdentifierTypeDeclaration::type() const
{
    return _type;
}

void VariableDeclaration::semanticCheckImpl(SymbolTable& table)
{
    _initializer->semanticCheckImpl(table);
    checkHasValue(_initializer);
    if (_typeID == "")
    {
        auto ty = _initializer->expressionType();
        _type = ty;
        if (table.addVariable(_identifier, this) == false)
        {
            semanticError("Duplicate variable name");
        }
    }
    else
    {
        table.addVariable(_identifier, this);
    }
}

void VariableDeclaration::semanticCheckInside(SymbolTable& table)
{
    if (_typeID != "")
    {
        auto ty = table.checkType(_typeID);
        if (*ty != *_initializer->expressionType())
        {
            _initializer->semanticError("Type mismatch");
        }
    }
}

void TypeDeclaration::semanticCheckImpl(SymbolTable& table)
{
    if (table.addType(_typeID, _type.get()) == false)
    {
        semanticError("Duplicate type name");
    }
}

void TypeDeclaration::semanticCheckInside(SymbolTable& table)
{
    _type->semanticCheckImpl(table);
}

void TypeDeclaration::addToFrame(Frame& f)
{
}

void FunctionParameter::semanticCheckInside(SymbolTable&)
{
    assert(0);
}

void FunctionDeclaration::semanticCheckImpl(SymbolTable& table)
{
    if (table.addFunction(_identifier, this) == false)
    {
        semanticError("Duplicate function name");
    }
    for (auto& param : _parameters)
    {
        param.semanticCheckImpl(table);
    }
}

void FunctionDeclaration::semanticCheckInside(SymbolTable& table)
{
    std::set<std::string> unique;
    ScopeGuard g(table);
    for (auto& param : _parameters)
    {
        if (unique.insert(param.identifier()).second == false)
        {
            param.semanticError("Duplicate parameter name");
        }
        if (table.checkType(param.typeId()) == nullptr)
        {
            param.semanticError("Type not exist");
        }
        table.addVariable(param.identifier(), &param);
    }
    _body->semanticCheckImpl(table);
    if (_returnType != "")
    {
        auto retType = table.checkType(_returnType);
        if (retType == nullptr)
        {
            semanticError("Return type not exist");
        }
        checkHasValue(_body);
        if (*_body->expressionType() != *retType)
        {
            semanticError("Return type not match");
        }
    }
    else
    {
        if (_body->hasValue())
        {
            _body->semanticError("Function does not return type but its body do");
        }
    }

}

void IdentifierType::semanticCheckImpl(SymbolTable& table)
{
    if (table.checkType(_identifier) == nullptr)
    {
        semanticError("Type " + _identifier + " does not exist");
    }
}

void ArrayType::semanticCheckImpl(SymbolTable& table)
{
    if (table.checkType(_typeID) == nullptr)
    {
        semanticError("Type " + _typeID + " does not exist");
    }
}

void RecordType::semanticCheckImpl(SymbolTable& table)
{
    std::set<std::string> unique;
    for (auto const& id : _fields)
    {
        if (unique.insert(id.identifier()).second == false)
        {
            semanticError("Duplicate record entry");
        }
        if (table.checkType(id.typeId()) == nullptr)
        {
            semanticError("Type " + id.typeId() + " does not exist");
        }
    }
}

void Identifier::semanticCheckImpl(SymbolTable& table)
{
    bool inCurrScope = false;
    auto ty = table.checkVariable(_identifier, inCurrScope);
    if (ty == nullptr)
    {
        semanticError("Variable not exist");
    }
    if (!inCurrScope)
    {
        ty->escape(true);
    }
    _expressionType = ty->type();
    assert(_expressionType);
    _lValue = true;
}

void Subscript::semanticCheckImpl(SymbolTable& table)
{
    _left->semanticCheckImpl(table);
    _right->semanticCheckImpl(table);
    if (_left->hasValue() && _left->expressionType()->isArray() &&
        _right->hasValue() && _right->expressionType()->isInt())
    {
        auto arrTy = static_cast<const ArrayType*>(_left->expressionType());
        _expressionType = table.checkType(arrTy->typeId());
        assert(_expressionType);
        _lValue = true;
    }
    else
    {
        semanticError("Array subscription not valid");
    }
}

void FieldExpression::semanticCheckImpl(SymbolTable& table)
{
    _left->semanticCheckImpl(table);
    if (_left->hasValue() && _left->expressionType()->isRecord())
    {
        auto recTy = static_cast<const RecordType*>(_left->expressionType());
        auto r = std::find_if(recTy->fields().begin(), recTy->fields().end(), [&](const FieldDeclaration& e)
        {
            return e.identifier() == _identifier;
        });
        if (r == recTy->fields().end())
        {
            semanticError("Record type does not have field: " + _identifier);
        }
        _expressionType = table.checkType(r->typeId());
        assert(_expressionType);
        _lValue = true;
    }
    else
    {
        semanticError("Left hand side is not record type");
    }
}

void Nil::semanticCheckImpl(SymbolTable& table)
{
    _expressionType = table.checkType("nil");
}

void IntLiteral::semanticCheckImpl(SymbolTable& table)
{
    _expressionType = table.checkType("int");
}

void StringLiteral::semanticCheckImpl(SymbolTable& table)
{
    _expressionType = table.checkType("string");
}

void Sequence::semanticCheckImpl(SymbolTable& table)
{
    for (auto& exp : _expressions)
    {
        exp->semanticCheckImpl(table);
    }
    if (_expressions.size() == 0)
    {
        _expressionType = nullptr;
    }
    else
    {
        _expressionType = _expressions.back()->hasValue() ? _expressions.back()->expressionType()
            : nullptr;
    }
}

void Negation::semanticCheckImpl(SymbolTable& table)
{
    _expression->semanticCheckImpl(table);
    checkHasValue(_expression);
    if (!_expression->expressionType()->isInt())
    {
        semanticError("Negation is invalid");
    }
    _expressionType = _expression->expressionType();
}

void Call::semanticCheckImpl(SymbolTable& table)
{
    for (auto& exp : _arguments)
    {
        exp->semanticCheckImpl(table);
    }
    auto ty = table.checkFunction(_identifier);
    if (ty == nullptr)
    {
        semanticError("Function does not exist");
    }
    if (ty->parameters().size() != _arguments.size())
    {
        semanticError("Argument number mismatch");
    }
    _func = ty;
    for (size_t i = 0; i < _arguments.size(); ++i)
    {
        _arguments[i]->semanticCheckImpl(table);
        checkHasValue(_arguments[i]);
        if (*_arguments[i]->expressionType() != *table.checkType(ty->parameters()[i].typeId()))
        {
            semanticError("Type mismatch");
        }
    }
    if (ty->returnType() == "")
    {
        _expressionType = nullptr;
    }
    else
    {
        _expressionType = table.checkType(ty->returnType());
        if (_expressionType == nullptr)
        {
            semanticError("Return type not exist");
        }
    }
}

void Infix::semanticCheckImpl(SymbolTable& table)
{
    _left->semanticCheckImpl(table);
    _right->semanticCheckImpl(table);
    checkHasValue(_left);
    checkHasValue(_right);
    semanticCheckInfix(table);

    _expressionType = table.checkType("int");
}

void Infix::semanticCheckInfix(SymbolTable& table)
{
    if (_left->expressionType()->isInt() && _right->expressionType()->isInt())
    {
        return;
    }
    semanticError("Expression type not valid");
}

void Eq::semanticCheckInfix(SymbolTable& table)
{
    if (*_left->expressionType() != *_right->expressionType())
    {
        semanticError("Type mismatch in equlity comparison");
    }
}

void Ne::semanticCheckInfix(SymbolTable& table)
{
    if (*_left->expressionType() != *_right->expressionType())
    {
        semanticError("Type mismatch in inequlity comparison");
    }
}
template<class T>
static void checkComparisonImpl(T* ths, Expression* l, Expression* r)
{
    if (*l->expressionType() == *r->expressionType())
    {
        if (l->expressionType()->isInt() || r->expressionType()->isString())
        {
            return;
        }
    }
    ths->semanticError("Type should be int or string in comparison");
}
void Gt::semanticCheckInfix(SymbolTable& table)
{
    checkComparisonImpl(this, _left.get(), _right.get());
}

void Ge::semanticCheckInfix(SymbolTable& table)
{
    checkComparisonImpl(this, _left.get(), _right.get());
}

void Lt::semanticCheckInfix(SymbolTable& table)
{
    checkComparisonImpl(this, _left.get(), _right.get());
}

void Le::semanticCheckInfix(SymbolTable& table)
{
    checkComparisonImpl(this, _left.get(), _right.get());
}

void ArrayCreate::semanticCheckImpl(SymbolTable& table)
{
    auto ty = table.checkType(_typeID);
    if (ty == nullptr)
    {
        semanticError("Type not exist");
    }
    _size->semanticCheckImpl(table);
    checkHasValue(_size);
    _initializer->semanticCheckImpl(table);
    checkHasValue(_initializer);
    if (!_size->expressionType()->isInt())
    {
        semanticError("Array size is not int");
    }
    if (*_initializer->expressionType() != *ty)
    {
        semanticError("Array initializer type mismatch");
    }
    _expressionType = SymbolTable::getArrayType(_typeID);
}

void Break::semanticCheckImpl(SymbolTable& table)
{
}

void RecordCreate::semanticCheckImpl(SymbolTable& table)
{
    auto ty = table.checkType(_typeID);
    if (ty == nullptr)
    {
        semanticError("Record name not exist");
    }
    if (!ty->isRecord())
    {
        semanticError("Type name is not record");
    }
    auto recTy = static_cast<const RecordType*>(ty);
    if (recTy->fields().size() != _fields.size())
    {
        semanticError("Number of fields mismatch");
    }
    for (size_t i = 0; i < _fields.size(); ++i)
    {
        if (_fields[i].identifier() != recTy->fields()[i].identifier())
        {
            _fields[i].semanticError("Field name mismatch");
        }
        _fields[i].initializer()->semanticCheckImpl(table);
        checkHasValue(_fields[i].initializer());
        if (*_fields[i].initializer()->expressionType() != *table.checkType(recTy->fields()[i].typeId()))
        {
            _fields[i].semanticError("Initializer type mismatch");
        }
    }
    _expressionType = ty;
}

void Assignment::semanticCheckImpl(SymbolTable& table)
{
    _left->semanticCheckImpl(table);
    checkHasValue(_left);
    if (!_left->isLValue())
    {
        _left->semanticError("Left hand side is not lvalue");
    }
    _right->semanticCheckImpl(table);
    checkHasValue(_right);
    if (*_left->expressionType() == *_right->expressionType())
    {
        return;
    }
    _right->semanticError("Type mismatch");
}

void IfThenElse::semanticCheckImpl(SymbolTable& table)
{
    _condition->semanticCheckImpl(table);
    checkHasValue(_condition);
    if (!_condition->expressionType()->isInt())
    {
        _condition->semanticError("Condition should be int type");
    }
    _thenBranch->semanticCheckImpl(table);
    _elseBranch->semanticCheckImpl(table);
    if (_thenBranch->hasValue() && _elseBranch->hasValue())
    {
        if (*_thenBranch->expressionType() == *_elseBranch->expressionType())
        {
            _expressionType = _thenBranch->expressionType();
        }
        else
        {
            semanticError("Type in two branches mismatch");
        }
    }
    else if (!_thenBranch->hasValue() && !_elseBranch->hasValue())
    {
        return;
    }
    else
    {
        semanticError("Two branches should either has same type or has no value");
    }
}

void IfThen::semanticCheckImpl(SymbolTable& table)
{
    _condition->semanticCheckImpl(table);
    checkHasValue(_condition);
    if (!_condition->expressionType()->isInt())
    {
        _condition->semanticError("Condition should be int type");
    }
    _thenBranch->semanticCheckImpl(table);
    if (_thenBranch->hasValue())
    {
        _thenBranch->semanticError("If then expression should not have value");
    }
}

void While::semanticCheckImpl(SymbolTable& table)
{
    _condition->semanticCheckImpl(table);
    checkHasValue(_condition);
    if (!_condition->expressionType()->isInt())
    {
        _condition->semanticError("Condition should be int type");
    }
    _body->semanticCheckImpl(table);
    if (_body->hasValue())
    {
        _body->semanticError("While body should not have value");
    }
}

void For::semanticCheckImpl(SymbolTable& table)
{
    ScopeGuard g(table);

    _lowerBound->semanticCheckImpl(table);
    _upperBound->semanticCheckImpl(table);
    checkHasValue(_lowerBound);
    checkHasValue(_lowerBound);
    if (!_lowerBound->expressionType()->isInt())
    {
        _lowerBound->semanticError("Expression should be int");
    }
    if (!_upperBound->expressionType()->isInt())
    {
        _upperBound->semanticError("Expression should be int");
    }
    table.addLoopVariable(_identifier);
    _body->semanticCheckImpl(table);
    if (_body->hasValue())
    {
        _body->semanticError("For body should not have value");
    }
}

void Let::semanticCheckImpl(SymbolTable& table)
{
    ScopeGuard g(table);
    for (auto& decl : _bindings)
    {
        decl->semanticCheckImpl(table);
    }
    for (auto& decl : _bindings)
    {
        decl->semanticCheckInside(table);
    }
    for (auto& exp : _body)
    {
        exp->semanticCheckImpl(table);
    }
    if (_body.size() != 0)
    {
        _expressionType = _body.back()->hasValue()
            ? _body.back()->expressionType()
            : nullptr;
    }
}

}

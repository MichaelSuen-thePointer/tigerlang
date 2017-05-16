#pragma once
#include <string>
#include <memory>
#include <vector>

namespace tiger
{
namespace ir
{

class IRTNode
{
public:
    virtual ~IRTNode() = default;
};

class IRExpression : public IRTNode
{

};

class IRStatement : public IRTNode
{

};

class Constant : public IRExpression
{
protected:
    int _value;
public:
    explicit Constant(int value)
        : _value(value)
    {
    }

    int value() const
    {
        return _value;
    }
};

class Name : public IRExpression
{
protected:
    std::string _name;
public:
    explicit Name(std::string name)
        : _name(std::move(name))
    {
    }

    const std::string& name() const
    {
        return _name;
    }
};

class IRMoveTarget : public IRExpression
{

};

class TemporaryVariable : public IRMoveTarget
{
protected:
    int _id;
public:

    explicit TemporaryVariable(int id)
        : _id(id)
    {
    }

    int id() const
    {
        return _id;
    }
};

class MemoryAccess : public IRMoveTarget
{
protected:
    int _offset;
public:

    explicit MemoryAccess(int offset)
        : _offset(offset)
    {
    }

    int offset() const
    {
        return _offset;
    }
};

class BinaryOperation : public IRExpression
{
public:
    std::unique_ptr<IRExpression> _left;
    std::unique_ptr<IRExpression> _right;
public:
    BinaryOperation(IRExpression* left, IRExpression* right)
        : _left(left), _right(right)
    {
    }

    const std::unique_ptr<IRExpression>& left() const
    {
        return _left;
    }

    const std::unique_ptr<IRExpression>& right() const
    {
        return _right;
    }
};

class Plus : public BinaryOperation
{
public:
    Plus(IRExpression* left, IRExpression* right)
        : BinaryOperation(left, right)
    {
    }
};

class Minus : public BinaryOperation
{
public:
    Minus(IRExpression* left, IRExpression* right)
        : BinaryOperation(left, right)
    {
    }
};

class Multiply : public BinaryOperation
{
public:
    Multiply(IRExpression* left, IRExpression* right)
        : BinaryOperation(left, right)
    {
    }
};

class Divide : public BinaryOperation
{
public:
    Divide(IRExpression* left, IRExpression* right)
        : BinaryOperation(left, right)
    {
    }
};

class And : public BinaryOperation
{
public:
    And(IRExpression* left, IRExpression* right)
        : BinaryOperation(left, right)
    {
    }
};

class Or : public BinaryOperation
{
public:
    Or(IRExpression* left, IRExpression* right)
        : BinaryOperation(left, right)
    {
    }
};

class Xor : public BinaryOperation
{
public:
    Xor(IRExpression* left, IRExpression* right)
        : BinaryOperation(left, right)
    {
    }
};

class LeftShift : public BinaryOperation
{
public:
    LeftShift(IRExpression* left, IRExpression* right)
        : BinaryOperation(left, right)
    {
    }
};

class RightShift : public BinaryOperation
{
public:
    RightShift(IRExpression* left, IRExpression* right)
        : BinaryOperation(left, right)
    {
    }
};

class FunctionCall : public IRExpression
{
protected:
    std::string _name;
    std::vector<std::unique_ptr<IRExpression>> _parameters;
public:
    FunctionCall(std::string name, std::vector<IRExpression*>& exp)
        : _name(std::move(name))
        , _parameters(exp.begin(), exp.end())
    {
        exp.clear();
    }

    const std::string& name() const
    {
        return _name;
    }

    const std::vector<std::unique_ptr<IRExpression>>& parameters() const
    {
        return _parameters;
    }
};

class EffectSequence : public IRExpression
{
protected:
    std::unique_ptr<IRStatement> _left;
    std::unique_ptr<IRExpression> _right;
public:
    EffectSequence(IRStatement* left, IRExpression* right)
        : _left(left), _right(right)
    {
    }

    const std::unique_ptr<IRStatement>& left() const
    {
        return _left;
    }

    const std::unique_ptr<IRExpression>& right() const
    {
        return _right;
    }
};

class Move : public IRStatement
{
protected:
    std::unique_ptr<IRMoveTarget> _target;
    std::unique_ptr<IRExpression> _exp;
public:
    Move(IRMoveTarget* target, IRExpression* exp)
        : _target(target), _exp(exp)
    {
    }

    const std::unique_ptr<IRMoveTarget>& target() const
    {
        return _target;
    }

    const std::unique_ptr<IRExpression>& exp() const
    {
        return _exp;
    }
};

class Expression : public IRStatement
{
protected:
    std::unique_ptr<IRExpression> _exp;
public:
    explicit Expression(IRExpression* exp)
        : _exp(exp)
    {
    }

    const std::unique_ptr<IRExpression>& exp() const
    {
        return _exp;
    }
};

class Jump : public IRStatement
{
protected:
    std::unique_ptr<IRExpression> _target;
    std::vector<std::unique_ptr<Expression>> _allTargets;
public:
    Jump(IRExpression* target, std::vector<IRExpression*>& allTargets)
        : _target(target), _allTargets(allTargets.begin(), allTargets.end())
    {
        allTargets.clear();
    }

    const std::unique_ptr<IRExpression>& target() const
    {
        return _target;
    }

    const std::vector<std::unique_ptr<Expression>>& allTargets() const
    {
        return _allTargets;
    }
};

class CompareJump : public IRStatement
{
protected:
    std::unique_ptr<IRExpression> _left;
    std::unique_ptr<IRExpression> _right;
    std::string _trueBranch;
    std::string _falseBranch;
public:
    CompareJump(IRExpression* left, IRExpression* right,
        std::string trueBranch, std::string falseBranch)
        : _left(left), _right(right), _trueBranch(std::move(trueBranch)), _falseBranch(std::move(falseBranch))
    {
    }

    const std::unique_ptr<IRExpression>& left() const
    {
        return _left;
    }

    const std::unique_ptr<IRExpression>& right() const
    {
        return _right;
    }

    const std::string& trueBranch() const
    {
        return _trueBranch;
    }

    const std::string& falseBranch() const
    {
        return _falseBranch;
    }
};

class EqCompare : public CompareJump
{
public:
    EqCompare(IRExpression* left, IRExpression* right, std::string trueBranch, std::string falseBranch)
        : CompareJump(left, right, std::move(trueBranch), std::move(falseBranch))
    {
    }
};

class NeCompare : public CompareJump
{
public:
    NeCompare(IRExpression* left, IRExpression* right, std::string trueBranch, std::string falseBranch)
        : CompareJump(left, right, std::move(trueBranch), std::move(falseBranch))
    {
    }
};

class GtCompare : public CompareJump
{
public:
    GtCompare(IRExpression* left, IRExpression* right, std::string trueBranch, std::string falseBranch)
        : CompareJump(left, right, std::move(trueBranch), std::move(falseBranch))
    {
    }
};

class GeCompare : public CompareJump
{
public:
    GeCompare(IRExpression* left, IRExpression* right, std::string trueBranch, std::string falseBranch)
        : CompareJump(left, right, std::move(trueBranch), std::move(falseBranch))
    {
    }
};

class LtCompare : public CompareJump
{
public:
    LtCompare(IRExpression* left, IRExpression* right, std::string trueBranch, std::string falseBranch)
        : CompareJump(left, right, std::move(trueBranch), std::move(falseBranch))
    {
    }
};

class LeCompare : public CompareJump
{
public:
    LeCompare(IRExpression* left, IRExpression* right, std::string trueBranch, std::string falseBranch)
        : CompareJump(left, right, std::move(trueBranch), std::move(falseBranch))
    {
    }
};

class Sequence : public IRStatement
{
protected:
    std::unique_ptr<IRStatement> _left;
    std::unique_ptr<IRStatement> _right;
public:
    Sequence(IRStatement* left, IRStatement* right)
        : _left(left), _right(right)
    {
    }

    const std::unique_ptr<IRStatement>& left() const
    {
        return _left;
    }

    const std::unique_ptr<IRStatement>& right() const
    {
        return _right;
    }
};

class Label : public IRTNode
{
protected:
    std::string _name;
public:
    explicit Label(std::string name)
        : _name(std::move(name))
    {
    }

    const std::string& name() const
    {
        return _name;
    }
};



}
}

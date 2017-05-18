#pragma once
#include <string>
#include <memory>
#include <vector>

namespace tiger
{
namespace ir
{
class IRExpression;
class IRStatement;
class IRTNode
{
public:
    virtual ~IRTNode() = default;

    virtual IRExpression* toExpression() = 0;
    virtual IRStatement* toStatement() = 0;
};

class IRExpression : public IRTNode
{
public:
    IRExpression* toExpression() override
    {
        return this;
    }
    IRStatement* toStatement() override;
};

class IRStatement : public IRTNode
{
public:
    IRStatement* toStatement() override
    {
        return this;
    }
    IRExpression* toExpression() override;
};

class Constant : public IRExpression
{
protected:
    int _value;
public:
    static Constant* newStaticLinkOffset();

    static Constant* newWordSize();

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
    std::string _id;
    static int _internalCounter;
public:
    static TemporaryVariable* format(const std::string& desc, int line, int column)
    {
        return new TemporaryVariable(desc + std::to_string(line) + "_" + std::to_string(column));
    }
    static TemporaryVariable* newFP()
    {
        return new TemporaryVariable("fp");
    }
    static TemporaryVariable* newSP()
    {
        return new TemporaryVariable("sp");
    }
    explicit TemporaryVariable(std::string id)
        : _id(std::move(id))
    {
    }

    const std::string& id() const
    {
        return _id;
    }
};

class MemoryAccess : public IRMoveTarget
{
protected:
    std::unique_ptr<IRExpression> _offset;
public:

    explicit MemoryAccess(IRExpression* offset)
        : _offset(offset)
    {
    }

    const std::unique_ptr<IRExpression>& offset() const
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
    std::unique_ptr<IRExpression> _func;
    std::vector<std::unique_ptr<IRExpression>> _parameters;
public:
    FunctionCall(IRExpression* func, std::vector<IRExpression*>& exp)
        : _func(func)
        , _parameters(exp.begin(), exp.end())
    {
        exp.clear();
    }

    FunctionCall(IRExpression* func, std::vector<IRExpression*>&& exp)
        : FunctionCall(func, exp)
    {
    }

    const std::unique_ptr<IRExpression>& function() const
    {
        return _func;
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

class ExpressionStatement : public IRStatement
{
protected:
    std::unique_ptr<IRExpression> _exp;
public:
    explicit ExpressionStatement(IRExpression* exp)
        : _exp(exp)
    {
    }

    const std::unique_ptr<IRExpression>& exp() const
    {
        return _exp;
    }
};

class Label : public IRStatement
{
protected:
    std::string _name;
public:
    static Label* format(const std::string& desc, int line, int column)
    {
        return new Label(name(desc, line, column));
    }
    static std::string name(const std::string& desc, int line, int column)
    {
        return desc + std::to_string(line) + "_" + std::to_string(column);
    }
    explicit Label(std::string name)
        : _name(std::move(name))
    {
    }

    const std::string& name() const
    {
        return _name;
    }
};

class Jump : public IRStatement
{
protected:
    std::unique_ptr<Label> _target;
public:
    explicit Jump(Label* target)
        : _target(target)
    {
    }

    const std::unique_ptr<Label>& target() const
    {
        return _target;
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
    template<class T>
    static std::unique_ptr<CompareJump> makeCJump(IRExpression* left, IRExpression* right, int li, int co);
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

class SequenceExpression : public IRStatement
{
protected:
    std::unique_ptr<IRStatement> _left;
    std::unique_ptr<IRStatement> _right;
public:
    SequenceExpression(IRStatement* left, IRStatement* right)
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

class Nop : public IRStatement
{
};


template <class T>
std::unique_ptr<CompareJump> CompareJump::makeCJump(IRExpression* left, IRExpression* right, int li, int co)
{
    auto falseBranch = new SequenceExpression(
        Label::format("iffalse", li, co),
        new Move(TemporaryVariable::format("ifresult", li, co), new Constant(1)));
    auto gotoEnd = new SequenceExpression(
        new Jump(Label::format("ifend", li, co)),
        falseBranch);
    auto trueBranch = new SequenceExpression(
        Label::format("iftrue", li, co),
        new SequenceExpression(
            new Move(TemporaryVariable::format("ifresult", li, co), new Constant(0)),
            gotoEnd));
    auto ifEnd = new EffectSequence(Label::format("ifend", li, co),
        TemporaryVariable::format("ifresult", li, co));
    auto ifJump = new T(left, right,
        Label::name("iftrue", li, co),
        Label::name("iffalse", li, co));
    return std::make_unique<EffectSequence>(
        new SequenceExpression(ifJump, trueBranch),
        ifEnd);
}

}
}

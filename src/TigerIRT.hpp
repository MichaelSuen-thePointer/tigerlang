#pragma once
#include <string>
#include <memory>
#include <vector>

namespace tiger
{
class ASTNode;
namespace ir
{
class IRExpression;
class IRStatement;
class IRCompare;
class IRTNode
{
protected:
    ASTNode* _ast = nullptr;
public:
    explicit IRTNode(ASTNode* ast)
        : _ast(ast)
    {
    }
    virtual ~IRTNode() = default;

    const ASTNode* ast() const
    {
        return _ast;
    }
    ASTNode* ast()
    {
        return _ast;
    }
    virtual IRExpression* toExpression() = 0;
    virtual IRStatement* toStatement() = 0;
    virtual IRCompare* toCompare() = 0;
};

class IRExpression : public IRTNode
{
public:
    explicit IRExpression(ASTNode* ast)
        : IRTNode(ast)
    {
    }

    IRExpression* toExpression() final override
    {
        return this;
    }
    IRStatement* toStatement() final override;

    IRCompare* toCompare() final override;
};

class IRStatement : public IRTNode
{
public:
    explicit IRStatement(ASTNode* ast)
        : IRTNode(ast)
    {
    }

    IRStatement* toStatement() override
    {
        return this;
    }
    IRExpression* toExpression() override;

    IRCompare* toCompare() override;
};

class Constant : public IRExpression
{
protected:
    int _value;
public:
    static Constant* newStaticLinkOffset();

    static Constant* newWordSize();

    explicit Constant(int value)
        : IRExpression(nullptr)
        , _value(value)
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
    Name(ASTNode* ast, std::string name)
        : IRExpression(ast)
        , _name(std::move(name))
    {
    }

    const std::string& name() const
    {
        return _name;
    }
};

class IRMoveTarget : public IRExpression
{
public:
    explicit IRMoveTarget(ASTNode* ast)
        : IRExpression(ast)
    {
    }
};

class TemporaryVariable : public IRMoveTarget
{
protected:
    std::string _id;
    static int _internalCounter;
public:
    static TemporaryVariable* format(const std::string& desc, int line, int column, int end)
    {
        return new TemporaryVariable(desc + std::to_string(line) + "_" + std::to_string(column) + "_" + std::to_string(end));
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
        : IRMoveTarget(nullptr)
        , _id(std::move(id))
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

    explicit MemoryAccess(ASTNode* ast, IRExpression* offset)
        : IRMoveTarget(ast)
        , _offset(offset)
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
    BinaryOperation(ASTNode* ast, IRExpression* left, IRExpression* right)
        : IRExpression(ast)
        , _left(left)
        , _right(right)
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
    Plus(ASTNode* ast, IRExpression* left, IRExpression* right)
        : BinaryOperation(ast, left, right)
    {
    }
};

class Minus : public BinaryOperation
{
public:
    Minus(ASTNode* ast, IRExpression* left, IRExpression* right)
        : BinaryOperation(ast, left, right)
    {
    }
};

class Multiply : public BinaryOperation
{
public:
    Multiply(ASTNode* ast, IRExpression* left, IRExpression* right)
        : BinaryOperation(ast, left, right)
    {
    }
};

class Divide : public BinaryOperation
{
public:
    Divide(ASTNode* ast, IRExpression* left, IRExpression* right)
        : BinaryOperation(ast, left, right)
    {
    }
};

class LogicalAnd : public BinaryOperation
{
public:
    LogicalAnd(ASTNode* ast, IRExpression* left, IRExpression* right)
        : BinaryOperation(ast, left, right)
    {
    }
};

class LogicalOr : public BinaryOperation
{
public:
    LogicalOr(ASTNode* ast, IRExpression* left, IRExpression* right)
        : BinaryOperation(ast, left, right)
    {
    }
};

class Xor : public BinaryOperation
{
public:
    Xor(ASTNode* ast, IRExpression* left, IRExpression* right)
        : BinaryOperation(ast, left, right)
    {
    }
};

class LeftShift : public BinaryOperation
{
public:
    LeftShift(ASTNode* ast, IRExpression* left, IRExpression* right)
        : BinaryOperation(ast, left, right)
    {
    }
};

class RightShift : public BinaryOperation
{
public:
    RightShift(ASTNode* ast, IRExpression* left, IRExpression* right)
        : BinaryOperation(ast, left, right)
    {
    }
};

class FunctionCall : public IRExpression
{
protected:
    std::unique_ptr<IRExpression> _func;
    std::vector<std::unique_ptr<IRExpression>> _parameters;
public:
    FunctionCall(ASTNode* ast, IRExpression* func, std::vector<IRExpression*>& exp)
        : IRExpression(ast)
        , _func(func)
        , _parameters(exp.begin(), exp.end())
    {
        exp.clear();
    }

    FunctionCall(ASTNode* ast, IRExpression* func, std::vector<IRExpression*>&& exp)
        : FunctionCall(ast, func, exp)
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
    EffectSequence(ASTNode* ast, IRStatement* left, IRExpression* right)
        : IRExpression(ast)
        , _left(left)
        , _right(right)
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

    std::unique_ptr<IRExpression>& right()
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
    Move(ASTNode* ast, IRMoveTarget* target, IRExpression* exp)
        : IRStatement(ast)
        , _target(target)
        , _exp(exp)
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
    explicit ExpressionStatement(ASTNode* ast, IRExpression* exp)
        : IRStatement(ast)
        , _exp(exp)
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
    static Label* format(const std::string& desc, int line, int column, int end)
    {
        return new Label(name(desc, line, column, end));
    }
    static std::string name(const std::string& desc, int line, int column, int end)
    {
        return desc + std::to_string(line) + "_" + std::to_string(column) + "_" + std::to_string(end);
    }
    explicit Label(std::string name)
        : IRStatement(nullptr)
        , _name(std::move(name))
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
        : IRStatement(nullptr)
        , _target(target)
    {
    }

    const std::unique_ptr<Label>& target() const
    {
        return _target;
    }
};

class SequenceExpression;
class IRCompareJump;

class IRCompare : public IRStatement
{
protected:
    std::unique_ptr<IRStatement> _statement;
    std::vector<std::string> _trueBranchLabels;
    std::vector<std::string> _falseBranchLabels;
public:
    IRCompare(ASTNode* ast, IRCompareJump* ircjump);

    std::unique_ptr<IRStatement>& statement()
    {
        return _statement;
    }

    std::vector<std::string>& trueBranchLabels()
    {
        return _trueBranchLabels;
    }

    std::vector<std::string>& falseBranchLabels()
    {
        return _falseBranchLabels;
    }

    void addTrueBranchLabel(std::string s)
    {
        _trueBranchLabels.push_back(std::move(s));
    }
    void addFalseBranchLabel(std::string s)
    {
        _falseBranchLabels.push_back(std::move(s));
    }
    static IRStatement* makeLabelTree(const std::vector<std::string>& labels, IRStatement* stm);
    IRStatement* toStatement() override;
    IRExpression* toExpression() override;
    IRCompare* toCompare() override;
};

class IRCompareJump : public IRStatement
{
protected:
    std::unique_ptr<IRExpression> _left;
    std::unique_ptr<IRExpression> _right;
    std::string _trueBranch;
    std::string _falseBranch;
public:
    IRCompareJump(ASTNode* ast, IRExpression* left, IRExpression* right);

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

class EqCompare : public IRCompareJump
{
public:
    EqCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
        : IRCompareJump(ast, left, right)
    {
    }
};

class NeCompare : public IRCompareJump
{
public:
    NeCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
        : IRCompareJump(ast, left, right)
    {
    }
};

class GtCompare : public IRCompareJump
{
public:
    GtCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
        : IRCompareJump(ast, left, right)
    {
    }
};

class GeCompare : public IRCompareJump
{
public:
    GeCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
        : IRCompareJump(ast, left, right)
    {
    }
};

class LtCompare : public IRCompareJump
{
public:
    LtCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
        : IRCompareJump(ast, left, right)
    {
    }
};

class LeCompare : public IRCompareJump
{
public:
    LeCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
        : IRCompareJump(ast, left, right)
    {
    }
};

class SequenceExpression : public IRStatement
{
protected:
    std::unique_ptr<IRStatement> _left;
    std::unique_ptr<IRStatement> _right;
public:
    static IRStatement* makeExpressionTree(const std::vector<IRStatement*> stmts);
    SequenceExpression(ASTNode* ast, IRStatement* left, IRStatement* right)
        : IRStatement(ast)
        , _left(left)
        , _right(right)
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

    std::unique_ptr<IRStatement>& right()
    {
        return _right;
    }
};

}
}

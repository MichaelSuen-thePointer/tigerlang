#include <cassert>
#include "TigerIRT.hpp"
#include "TigerFrame.hpp"
#include "TigerAST.hpp"

namespace tiger
{
namespace ir
{
IRTNode::IRTNode(ASTNode* ast)
    : _ast(ast)
{
}

const ASTNode* IRTNode::ast() const
{
    return _ast;
}

ASTNode* IRTNode::ast()
{
    return _ast;
}

IRExpression::IRExpression(ASTNode* ast)
    : IRTNode(ast)
{
}

IRExpression* IRExpression::toExpression()
{
    return this;
}

IRStatement::IRStatement(ASTNode* ast)
    : IRTNode(ast)
{
}

IRStatement* IRStatement::toStatement()
{
    return this;
}

Constant::Constant(int value)
    : IRExpression(nullptr)
    , _value(value)
{
}

int Constant::value() const
{
    return _value;
}

Name::Name(ASTNode* ast, std::string name)
    : IRExpression(ast)
    , _name(std::move(name))
{
}

const std::string& Name::name() const
{
    return _name;
}

IRMoveTarget::IRMoveTarget(ASTNode* ast)
    : IRExpression(ast)
{
}

TemporaryVariable* TemporaryVariable::format(const std::string& desc, int line, int column, int end)
{
    return new TemporaryVariable(desc + std::to_string(line) + "_" + std::to_string(column) + "_" + std::to_string(end));
}

TemporaryVariable* TemporaryVariable::newFP()
{
    return new TemporaryVariable("fp");
}

TemporaryVariable* TemporaryVariable::newSP()
{
    return new TemporaryVariable("sp");
}

TemporaryVariable::TemporaryVariable(std::string id)
    : IRMoveTarget(nullptr)
    , _id(std::move(id))
{
}

const std::string& TemporaryVariable::id() const
{
    return _id;
}

MemoryAccess::MemoryAccess(ASTNode* ast, IRExpression* offset)
    : IRMoveTarget(ast)
    , _offset(offset)
{
}

const std::unique_ptr<IRExpression>& MemoryAccess::offset() const
{
    return _offset;
}

BinaryOperation::BinaryOperation(ASTNode* ast, IRExpression* left, IRExpression* right)
    : IRExpression(ast)
    , _left(left)
    , _right(right)
{
}

const std::unique_ptr<IRExpression>& BinaryOperation::left() const
{
    return _left;
}

const std::unique_ptr<IRExpression>& BinaryOperation::right() const
{
    return _right;
}

Plus::Plus(ASTNode* ast, IRExpression* left, IRExpression* right)
    : BinaryOperation(ast, left, right)
{
}

Minus::Minus(ASTNode* ast, IRExpression* left, IRExpression* right)
    : BinaryOperation(ast, left, right)
{
}

Multiply::Multiply(ASTNode* ast, IRExpression* left, IRExpression* right)
    : BinaryOperation(ast, left, right)
{
}

Divide::Divide(ASTNode* ast, IRExpression* left, IRExpression* right)
    : BinaryOperation(ast, left, right)
{
}

LogicalAnd::LogicalAnd(ASTNode* ast, IRExpression* left, IRExpression* right)
    : BinaryOperation(ast, left, right)
{
}

LogicalOr::LogicalOr(ASTNode* ast, IRExpression* left, IRExpression* right)
    : BinaryOperation(ast, left, right)
{
}

Xor::Xor(ASTNode* ast, IRExpression* left, IRExpression* right)
    : BinaryOperation(ast, left, right)
{
}

LeftShift::LeftShift(ASTNode* ast, IRExpression* left, IRExpression* right)
    : BinaryOperation(ast, left, right)
{
}

RightShift::RightShift(ASTNode* ast, IRExpression* left, IRExpression* right)
    : BinaryOperation(ast, left, right)
{
}

FunctionCall::FunctionCall(ASTNode* ast, IRExpression* func, std::vector<IRExpression*>& exp)
    : IRExpression(ast)
    , _func(func)
    , _parameters(exp.begin(), exp.end())
{
    exp.clear();
}

FunctionCall::FunctionCall(ASTNode* ast, IRExpression* func, std::vector<IRExpression*>&& exp)
    : FunctionCall(ast, func, exp)
{
}

const std::unique_ptr<IRExpression>& FunctionCall::function() const
{
    return _func;
}

const std::vector<std::unique_ptr<IRExpression>>& FunctionCall::parameters() const
{
    return _parameters;
}

EffectSequence::EffectSequence(ASTNode* ast, IRStatement* left, IRExpression* right)
    : IRExpression(ast)
    , _left(left)
    , _right(right)
{
}

const std::unique_ptr<IRStatement>& EffectSequence::left() const
{
    return _left;
}

const std::unique_ptr<IRExpression>& EffectSequence::right() const
{
    return _right;
}

std::unique_ptr<IRExpression>& EffectSequence::right()
{
    return _right;
}

Move::Move(ASTNode* ast, IRMoveTarget* target, IRExpression* exp)
    : IRStatement(ast)
    , _target(target)
    , _exp(exp)
{
}

const std::unique_ptr<IRMoveTarget>& Move::target() const
{
    return _target;
}

const std::unique_ptr<IRExpression>& Move::exp() const
{
    return _exp;
}

ExpressionStatement::ExpressionStatement(ASTNode* ast, IRExpression* exp)
    : IRStatement(ast)
    , _exp(exp)
{
}

const std::unique_ptr<IRExpression>& ExpressionStatement::exp() const
{
    return _exp;
}

Label* Label::format(const std::string& desc, int line, int column, int end)
{
    return new Label(name(desc, line, column, end));
}

std::string Label::name(const std::string& desc, int line, int column, int end)
{
    return desc + std::to_string(line) + "_" + std::to_string(column) + "_" + std::to_string(end);
}

Label::Label(std::string name)
    : IRStatement(nullptr)
    , _name(std::move(name))
{
}

const std::string& Label::name() const
{
    return _name;
}

Jump::Jump(Label* target)
    : IRStatement(nullptr)
    , _target(target)
{
}

const std::unique_ptr<Label>& Jump::target() const
{
    return _target;
}

std::unique_ptr<IRStatement>& IRCompare::statement()
{
    return _statement;
}

const std::vector<IRCompareJump*>& IRCompare::trueBranches() const
{
    return _trueBranches;
}

const std::vector<IRCompareJump*>& IRCompare::falseBranches() const
{
    return _falseBranches;
}

const std::unique_ptr<IRExpression>& IRCompareJump::left() const
{
    return _left;
}

const std::unique_ptr<IRExpression>& IRCompareJump::right() const
{
    return _right;
}

const std::string& IRCompareJump::trueBranch() const
{
    return _trueBranch;
}

const std::string& IRCompareJump::falseBranch() const
{
    return _falseBranch;
}

void IRCompareJump::trueBranch(std::string trueBranch)
{
    _trueBranch = std::move(trueBranch);
}

void IRCompareJump::falseBranch(std::string falseBranch)
{
    _falseBranch = std::move(falseBranch);
}

EqCompare::EqCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
    : IRCompareJump(ast, left, right)
{
}

NeCompare::NeCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
    : IRCompareJump(ast, left, right)
{
}

GtCompare::GtCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
    : IRCompareJump(ast, left, right)
{
}

GeCompare::GeCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
    : IRCompareJump(ast, left, right)
{
}

LtCompare::LtCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
    : IRCompareJump(ast, left, right)
{
}

LeCompare::LeCompare(ASTNode* ast, IRExpression* left, IRExpression* right)
    : IRCompareJump(ast, left, right)
{
}

SequenceExpression::SequenceExpression(ASTNode* ast, IRStatement* left, IRStatement* right)
    : IRStatement(ast)
    , _left(left)
    , _right(right)
{
}

const std::unique_ptr<IRStatement>& SequenceExpression::left() const
{
    return _left;
}

const std::unique_ptr<IRStatement>& SequenceExpression::right() const
{
    return _right;
}

std::unique_ptr<IRStatement>& SequenceExpression::right()
{
    return _right;
}

IRStatement* IRExpression::toStatement()
{
    return new ExpressionStatement(this->ast(), this);
}

IRCompare* IRExpression::toCompare()
{
    return new IRCompare(ast(), new NeCompare(ast(), this, new Constant(0)));
}
IRExpression* IRStatement::toExpression()
{
    return new EffectSequence(ast(), this, new Constant(0));
}

IRCompare* IRStatement::toCompare()
{
    assert(0);
    return nullptr;
}

Constant* Constant::newStaticLinkOffset()
{
    return new Constant(TigerFrame::StaticLinkOffset);
}

Constant* Constant::newWordSize()
{
    return new Constant(TigerFrame::WordSize);
}

IRCompare::IRCompare(ASTNode* ast, IRCompareJump* ircjump)
    : IRStatement(ast)
    , _statement(ircjump)
    , _trueBranches{ ircjump }
    , _falseBranches{ ircjump }
{
}

IRStatement* IRCompare::makeLabelTree(const std::vector<std::string>& labels, IRStatement* stm)
{
    if (labels.size() == 0)
    {
        return stm;
    }
    auto r = new SequenceExpression(nullptr, new Label(labels.back()), stm);
    for (auto iter = labels.rbegin() + 1; iter != labels.rend(); ++iter)
    {
        r = new SequenceExpression(nullptr, new Label(*iter), r);
    }
    return r;
}

IRStatement* IRCompare::toStatement()
{
    auto li = _statement->ast()->loc().begin.line;
    auto co = _statement->ast()->loc().begin.column;
    auto ed = _statement->ast()->loc().end.column;

    auto ignoreLabel = Label::name("ignoreJump", li, co, ed);
    refillTrueBranchLabel(ignoreLabel);
    refillFalseBranchLabel(ignoreLabel);
    auto stm = _statement.release();
    auto r = new SequenceExpression(stm->ast(), stm, new Label(ignoreLabel));
    delete this;
    return r;
}

IRExpression* IRCompare::toExpression()
{
    auto li = _statement->ast()->loc().begin.line;
    auto co = _statement->ast()->loc().begin.column;
    auto ed = _statement->ast()->loc().end.column;
    auto assignOne = new Move(nullptr, TemporaryVariable::format("branchResult", li, co, ed), new Constant(1));
    auto trueLabel = Label::name("trueLabel", li, co, ed);
    auto assignZero = new Move(nullptr, TemporaryVariable::format("branchResult", li, co, ed), new Constant(0));
    auto falseLabel = Label::name("falseLabel", li, co, ed);
    auto endLabel = Label::name("branchEnd", li, co, ed);
    auto branchEnd = TemporaryVariable::format("branchResult", li, co, ed);
    auto stm = _statement.release();
    refillTrueBranchLabel(trueLabel);
    refillFalseBranchLabel(falseLabel);
    auto branchIR = new EffectSequence(stm->ast(),
        new SequenceExpression(nullptr,
            new SequenceExpression(nullptr,
                new SequenceExpression(nullptr,
                    new SequenceExpression(nullptr,
                        new SequenceExpression(nullptr,
                            new SequenceExpression(nullptr,
                                stm,
                                new Label(trueLabel)),
                            assignOne),
                        new Jump(new Label(endLabel))),
                    new Label(falseLabel)),
                assignZero),
            new Label(endLabel)),
        branchEnd);
    delete this;
    return branchIR;
}

IRCompare* IRCompare::toCompare()
{
    return this;
}

IRCompareJump::IRCompareJump(ASTNode* ast, IRExpression* left, IRExpression* right)
    : IRStatement(ast)
    , _left(left)
    , _right(right)
{
}

IRStatement* SequenceExpression::makeExpressionTree(const std::vector<IRStatement*> stmts)
{
    if (stmts.size() == 0)
    {
        return new ExpressionStatement(nullptr, new Constant(0));
    }
    if (stmts.size() == 1)
    {
        return stmts[0];
    }
    auto r = new SequenceExpression(nullptr, stmts[0], stmts[1]);
    for (auto iter = stmts.begin() + 2; iter != stmts.end(); ++iter)
    {
        r = new SequenceExpression((*(stmts.end() - iter + stmts.begin()))->ast(), r, *iter);
    }
    return r;
}

void IRCompare::refillTrueBranchLabel(const std::string& s)
{
    for (auto e : _trueBranches)
    {
        e->trueBranch(s);
    }
}

void IRCompare::refillFalseBranchLabel(const std::string& s)
{
    for (auto e : _trueBranches)
    {
        e->falseBranch(s);
    }
}

void IRCompare::mergeTrueBranches(const std::vector<IRCompareJump*>& r)
{
    _trueBranches.insert(_trueBranches.end(), r.begin(), r.end());
}

void IRCompare::mergeFalseBranches(const std::vector<IRCompareJump*>& r)
{
    _falseBranches.insert(_falseBranches.end(), r.begin(), r.end());
}
}
}

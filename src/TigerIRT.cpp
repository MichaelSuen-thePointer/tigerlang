#include <cassert>
#include "TigerIRT.hpp"
#include "TigerFrame.hpp"
#include "TigerAST.hpp"
namespace tiger
{
namespace ir
{
IRStatement* IRExpression::toStatement()
{
    return new ExpressionStatement(this->ast(), this);
}

IRCompare* IRExpression::toCompare()
{
    auto cmp = new NeCompare(ast(), this, new Constant(0));
    auto r = new IRCompare(ast(), cmp);
    r->addTrueBranchLabel(cmp->trueBranch());
    r->addFalseBranchLabel(cmp->falseBranch());
    return r;
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
    , _trueBranchLabels(1, ircjump->trueBranch())
    , _falseBranchLabels(1, ircjump->falseBranch())
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
    auto allLabels = _trueBranchLabels;
    allLabels.insert(allLabels.begin(), _falseBranchLabels.begin(), _falseBranchLabels.end());

    assert(allLabels.size() >= 2);

    auto label = new SequenceExpression(nullptr, new Label(allLabels[allLabels.size() - 2]), new Label(allLabels.back()));
    for (auto iter = allLabels.rbegin() + 2; iter != allLabels.rend(); ++iter)
    {
        label = new SequenceExpression(nullptr, new Label(*iter), label);
    }
    auto stm = _statement.release();
    auto r = new SequenceExpression(stm->ast(), stm, label);
    delete this;
    return r;
}

IRExpression* IRCompare::toExpression()
{
    auto li = _statement->ast()->loc().begin.line;
    auto co = _statement->ast()->loc().begin.column;
    auto ed = _statement->ast()->loc().end.column;
    auto assignOne = new Move(nullptr, TemporaryVariable::format("branchResult", li, co, ed), new Constant(1));
    auto trueBranch = makeLabelTree(_trueBranchLabels, assignOne);
    auto assignZero = new Move(nullptr, TemporaryVariable::format("branchResult", li, co, ed), new Constant(0));
    auto falseBranch = new SequenceExpression(nullptr,
        new Jump(Label::format("branchEnd", li, co, ed)),
        makeLabelTree(_falseBranchLabels, assignZero));
    auto branchEnd = new EffectSequence(nullptr, Label::format("branchEnd", li, co, ed), TemporaryVariable::format("branchResult", li, co, ed));
    auto stm = _statement.release();
    auto branchIR = new EffectSequence(stm->ast(),
        new SequenceExpression(nullptr,
            new SequenceExpression(nullptr,
                stm,
                trueBranch),
            falseBranch
        ),
        branchEnd
    );
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
    , _trueBranch(Label::name("trueBranch", ast->loc().begin.line, ast->loc().begin.column, ast->loc().end.column))
    , _falseBranch(Label::name("falseBranch", ast->loc().begin.line, ast->loc().begin.column, ast->loc().end.column))
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
}
}

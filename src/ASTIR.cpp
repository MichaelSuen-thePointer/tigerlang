#include <memory>
#include "TigerAST.hpp"
#include "TigerIRT.hpp"
#include "TigerFrame.hpp"

namespace tiger
{
using namespace ir;
using std::make_unique;

static std::unique_ptr<MemoryAccess> traceStaticLink(Frame& f, const std::string& name)
{
    auto targetFrame = f.currentFrame()->frameByName(name);

    auto currFrame = f.currentFrame();

    std::unique_ptr<IRExpression> ir(TemporaryVariable::newFP());

    while (targetFrame != currFrame)
    {
        ir.reset(new MemoryAccess(nullptr, new Plus(nullptr, ir.release(), Constant::newStaticLinkOffset())));
        currFrame = currFrame->staticLink();
        assert(currFrame != nullptr);
    }

    auto offset = currFrame->getOffset(name);

    return make_unique<MemoryAccess>(nullptr, new Plus(nullptr, ir.release(), new Constant(offset)));
}


std::unique_ptr<IRTNode> Identifier::toIR(Frame& f)
{
    return traceStaticLink(f, _identifier);
}

std::unique_ptr<IRTNode> Subscript::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<MemoryAccess>(this, new Plus(nullptr, left.release()->toExpression(), new Multiply(nullptr, right.release()->toExpression(), Constant::newWordSize())));
    // MEM(left + right * WordSize)
}

std::unique_ptr<IRTNode> FieldExpression::toIR(Frame& f)
{
    auto left = _left->toIR(f);

    //calc offset
    assert(reinterpret_cast<RecordType*>(_left->expressionType()) != nullptr);
    auto recTy = static_cast<RecordType*>(_left->expressionType());
    auto i = std::find_if(recTy->fields().begin(), recTy->fields().end(), [this](const FieldDeclaration& e)
    {
        return e.identifier() == _identifier;
    });
    assert(i != recTy->fields().end());
    auto offset = (i - recTy->fields().begin()) * TigerFrame::WordSize;
    return make_unique<MemoryAccess>(nullptr, new Plus(nullptr, left.release()->toExpression(), new Constant(offset)));
}

std::unique_ptr<IRTNode> Nil::toIR(Frame& f)
{
    return make_unique<Constant>(0);
}

std::unique_ptr<IRTNode> IntLiteral::toIR(Frame& f)
{
    return make_unique<Constant>(_value);
}

std::unique_ptr<IRTNode> StringLiteral::toIR(Frame& f)
{
    std::string label = "string" + std::to_string(loc().begin.line) + "_" + std::to_string(loc().begin.column) + "_" + std::to_string(loc().end.column);
    f.addStringFragments(label, _value);
    return make_unique<Name>(this, label);
}

std::unique_ptr<IRTNode> Sequence::toIR(Frame& f)
{
    if (_expressions.size() == 0)
    {
        return make_unique<Constant>(0);
    }
    if (_expressions.size() == 1)
    {
        return _expressions[0]->toIR(f);
    }

    std::unique_ptr<IRExpression> root;
    std::unique_ptr<IRExpression>* currNode = &root;
    std::for_each(_expressions.begin(), _expressions.end() - 1, [&f, &currNode](const std::unique_ptr<Expression> &item)
    {
        auto peseq = new EffectSequence(item.get(), item->toIR(f).release()->toStatement(), nullptr);
        currNode->reset(peseq);
        currNode = &(peseq->right());
    });

    currNode->reset(_expressions.back()->toIR(f).release()->toExpression());
    return root;
}

std::unique_ptr<IRTNode> Negation::toIR(Frame& f)
{
    return make_unique<Minus>(this, new Constant(0), _expression->toIR(f)->toExpression());
}

std::unique_ptr<IRTNode> Call::toIR(Frame& f)
{
    std::vector<IRExpression*> args;
    std::transform(_arguments.begin(), _arguments.end(), std::back_inserter(args), [&f](std::unique_ptr<Expression>& e)
    {
        return e->toIR(f).release()->toExpression();
    });

    auto mangledFuncName = Label::name(_identifier, _func->loc().begin.line, _func->loc().begin.column, _func->loc().end.column);
    return make_unique<FunctionCall>(this, new Name(this, mangledFuncName), args);
}

std::unique_ptr<ir::IRTNode> Add::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<Plus>(this, left.release()->toExpression(), right.release()->toExpression());
}

std::unique_ptr<ir::IRTNode> Sub::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<Minus>(this, left.release()->toExpression(), right.release()->toExpression());
}

std::unique_ptr<ir::IRTNode> Mul::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<Multiply>(this, left.release()->toExpression(), right.release()->toExpression());
}

std::unique_ptr<ir::IRTNode> Div::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<Divide>(this, left.release()->toExpression(), right.release()->toExpression());
}

std::unique_ptr<IRTNode> Eq::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto ed = loc().end.column;
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);

    if (_left->expressionType()->isString())
    { //比较字符串
        assert(_right->expressionType()->isString());
        return make_unique<EffectSequence>(this,
            new Move(nullptr, TemporaryVariable::format("strcmpresult", li, co, ed),
                new FunctionCall(nullptr, new Name(nullptr, "builtin_streq"), std::vector<IRExpression*>{ left.release()->toExpression(),
                    right.release()->toExpression() })),
            TemporaryVariable::format("strcmpresult", li, co, ed));
    }
    auto r = new EqCompare(this, left.release()->toExpression(), right.release()->toExpression());
    return make_unique<IRCompare>(this, r);
}

std::unique_ptr<IRTNode> Ne::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto ed = loc().end.column;
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    if (_left->expressionType()->isString())
    { //比较字符串
        assert(_right->expressionType()->isString());
        return make_unique<EffectSequence>(this,
            new Move(nullptr, TemporaryVariable::format("strcmpresult", li, co, ed),
                new Minus(nullptr, new Constant(1), new FunctionCall(nullptr, new Name(nullptr, "builtin_streq"), std::vector<IRExpression*>{ left.release()->toExpression(),
                    right.release()->toExpression() }))),
            TemporaryVariable::format("strcmpresult", li, co, ed));
    }
    return make_unique<IRCompare>(this, new NeCompare(this, left.release()->toExpression(), right.release()->toExpression()));
}

std::unique_ptr<IRTNode> Gt::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);

    return make_unique<IRCompare>(this, new GtCompare(this, left.release()->toExpression(), right.release()->toExpression()));
}

std::unique_ptr<IRTNode> Ge::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);

    return make_unique<IRCompare>(this, new GeCompare(this, left.release()->toExpression(), right.release()->toExpression()));

}

std::unique_ptr<IRTNode> Lt::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);

    return make_unique<IRCompare>(this, new LtCompare(this, left.release()->toExpression(), right.release()->toExpression()));

}

std::unique_ptr<ir::IRTNode> Le::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);

    return make_unique<IRCompare>(this, new LeCompare(this, left.release()->toExpression(), right.release()->toExpression()));
}

std::unique_ptr<ir::IRTNode> And::toIR(Frame& f)
{
    auto left = _left->toIR(f).release();
    auto right = _right->toIR(f).release();

    auto leftCx = left->toCompare();
    auto rightCx = right->toCompare();

    auto leftStm = leftCx->statement().release();
    auto rightStm = rightCx->statement().release();

    auto shortCircuit = IRCompare::makeLabelTree(leftCx->trueBranchLabels(), rightStm);
    auto newStm = new SequenceExpression(this, leftStm, shortCircuit);

    rightCx->falseBranchLabels().insert(
        rightCx->falseBranchLabels().end(),
        leftCx->falseBranchLabels().begin(),
        leftCx->falseBranchLabels().end());
    rightCx->statement().reset(newStm);
    delete leftCx;
    return std::unique_ptr<IRTNode>(rightCx);
}

std::unique_ptr<ir::IRTNode> Or::toIR(Frame& f)
{
    auto left = _left->toIR(f).release();
    auto right = _right->toIR(f).release();

    auto leftCx = left->toCompare();
    auto rightCx = right->toCompare();

    auto leftStm = leftCx->statement().release();
    auto rightStm = rightCx->statement().release();

    auto shortCircuit = IRCompare::makeLabelTree(leftCx->falseBranchLabels(), rightStm);
    auto newStm = new SequenceExpression(this, leftStm, shortCircuit);

    rightCx->trueBranchLabels().insert(
        rightCx->trueBranchLabels().end(),
        leftCx->trueBranchLabels().begin(),
        leftCx->trueBranchLabels().end());
    rightCx->statement().reset(newStm);
    delete leftCx;
    return std::unique_ptr<IRTNode>(rightCx);
}

std::unique_ptr<IRTNode> ArrayCreate::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto ed = loc().end.column;
    auto initializer = _initializer->toIR(f).release()->toExpression();
    auto size = _size->toIR(f).release()->toExpression();
    return make_unique<FunctionCall>(this, new Name(nullptr, "builtin_initarray"), std::vector<IRExpression*>{ size, initializer });
}

std::unique_ptr<ir::IRTNode> RecordCreate::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto ed = loc().end.column;
    assert(expressionType()->isRecord());
    auto recordType = static_cast<RecordType*>(expressionType());
    auto fieldCount = recordType->fields().size();
    auto recordSize = TigerFrame::WordSize * fieldCount;
    recordSize = recordSize == 0 ? 1 : recordSize;
    auto allocMem = new Move(nullptr, TemporaryVariable::format("recordVar", li, co, ed),
        new FunctionCall(nullptr, new Name(nullptr, "builtin_malloc"), { new Constant(recordSize) })
    );
    auto resultVar = TemporaryVariable::format("recordVar", li, co, ed);
    if (fieldCount == 0)
    {
        return make_unique<EffectSequence>(this, allocMem, resultVar);
    }
    auto makeInitField = [li, co, ed](size_t i, IRExpression* exp)
    {
        return new Move(nullptr,
            new MemoryAccess(nullptr,
                new Plus(nullptr,
                    TemporaryVariable::format("recordVar", li, co, ed),
                    new Constant(i * TigerFrame::WordSize))),
            exp);
    };
    auto seq = make_unique<SequenceExpression>(nullptr, allocMem, nullptr);
    std::unique_ptr<IRStatement>* indir = &(seq->right());
    //field count >= 1
    for (size_t i = 0; i != fieldCount - 1; i++)
    {
        auto& thisField = recordType->fields()[i];
        auto r = std::find_if(_fields.begin(), _fields.end(), [&thisField](const FieldCreate& e)
        {
            return e.identifier() == thisField.identifier();
        });
        assert(r != _fields.end());
        auto initializer = r->initializer()->toIR(f).release()->toExpression();
        auto newSeq = new SequenceExpression(nullptr, makeInitField(i, initializer), nullptr);
        indir->reset(newSeq);
        indir = &newSeq->right();
    }
    {
        auto& lastField = recordType->fields().back();
        auto r = std::find_if(_fields.begin(), _fields.end(), [&lastField](const FieldCreate& e)
        {
            return e.identifier() == lastField.identifier();
        });
        assert(r != _fields.end());
        auto initializer = r->initializer()->toIR(f).release()->toExpression();
        indir->reset(makeInitField(fieldCount - 1, initializer));
    }
    return make_unique<EffectSequence>(this, seq.release(), TemporaryVariable::format("recordVar", li, co, ed));
}

std::unique_ptr<ir::IRTNode> Break::toIR(Frame& f)
{
    assert(f.currentLoopBreakLabel() != "");
    return make_unique<Jump>(new Label(f.currentLoopBreakLabel()));
}

std::unique_ptr<ir::IRTNode> Assignment::toIR(Frame& f)
{
    auto leftExp = _left->toIR(f).release()->toExpression();
    assert(dynamic_cast<IRMoveTarget*>(leftExp) != nullptr);
    auto left = static_cast<IRMoveTarget*>(leftExp);
    auto right = _right->toIR(f).release()->toExpression();
    return make_unique<Move>(this, left, right);
}

std::unique_ptr<ir::IRTNode> IfThen::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto ed = loc().end.column;

    auto cond = _condition->toIR(f).release()->toCompare();
    auto body = _thenBranch->toIR(f).release()->toStatement();

    auto trueBranch = IRCompare::makeLabelTree(cond->trueBranchLabels(), body);
    auto endLabel = Label::format("ifEnd", li, co, ed);
    auto falseBranch = IRCompare::makeLabelTree(cond->falseBranchLabels(), endLabel);
    auto condStm = cond->statement().release();
    delete cond;
    return make_unique<SequenceExpression>(this, condStm, new SequenceExpression(nullptr,
        trueBranch, falseBranch));
}

std::unique_ptr<ir::IRTNode> IfThenElse::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto ed = loc().end.column;

    auto cond = _condition->toIR(f).release()->toCompare();
    if (this->hasValue())
    {
        auto thenBranch = _thenBranch->toIR(f).release()->toExpression();
        auto elseBranch = _elseBranch->toIR(f).release()->toExpression();

        auto thenResult = new Move(nullptr, TemporaryVariable::format("ifResult", li, co, ed), thenBranch);
        auto elseResult = new Move(nullptr, TemporaryVariable::format("ifResult", li, co, ed), elseBranch);

        auto trueBranch = IRCompare::makeLabelTree(cond->trueBranchLabels(), thenResult);
        auto falseBranch = IRCompare::makeLabelTree(cond->falseBranchLabels(), elseResult);

        return make_unique<EffectSequence>(this,
            new SequenceExpression(nullptr,
                new SequenceExpression(nullptr,
                    cond,
                    new SequenceExpression(nullptr,
                        new SequenceExpression(nullptr,
                            trueBranch,
                            new Jump(Label::format("ifEnd", li, co, ed))),
                        falseBranch)),
                Label::format("ifEnd", li, co, ed)),
            TemporaryVariable::format("ifResult", li, co, ed));
    }
    else
    {
        auto thenBranch = _thenBranch->toIR(f).release()->toStatement();
        auto elseBranch = _elseBranch->toIR(f).release()->toStatement();

        auto trueBranch = IRCompare::makeLabelTree(cond->trueBranchLabels(), thenBranch);
        auto falseBranch = IRCompare::makeLabelTree(cond->falseBranchLabels(), elseBranch);

        return make_unique<SequenceExpression>(nullptr,
            new SequenceExpression(nullptr,
                cond,
                new SequenceExpression(nullptr,
                    new SequenceExpression(nullptr,
                        trueBranch,
                        new Jump(Label::format("ifEnd", li, co, ed))),
                    falseBranch)),
            Label::format("ifEnd", li, co, ed));
    }
}

std::unique_ptr<ir::IRTNode> While::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto ed = loc().end.column;

    auto startLabel = Label::name("whileStart", li, co, ed);
    auto endLabel = Label::name("whileEnd", li, co, ed);

    f.setLoopBreakLabel(endLabel);

    auto cond = _condition->toIR(f).release()->toCompare();
    auto body = _body->toIR(f).release()->toStatement();
    auto endLabels = IRCompare::makeLabelTree(cond->falseBranchLabels(), new Label(endLabel));
    auto continueLabels = IRCompare::makeLabelTree(cond->trueBranchLabels(), new SequenceExpression(nullptr,
        body,
        new SequenceExpression(nullptr,
            new Jump(new Label(startLabel)),
            endLabels)));
    auto condExp = cond->statement().release();
    return make_unique<SequenceExpression>(this,
        new Label(startLabel),
        new SequenceExpression(nullptr,
            condExp,
            continueLabels));
}

std::unique_ptr<ir::IRTNode> For::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto ed = loc().end.column;

    auto lowerBound = _lowerBound->toIR(f).release()->toExpression();
    auto upperBound = _upperBound->toIR(f).release()->toExpression();
    FrameScopeGuard fsg(f);
    f.currentFrame()->addVariable(_identifier);
    auto startLabel = Label::name("forStart", li, co, ed);
    auto initial = new Move(nullptr, traceStaticLink(f, _identifier).release(), lowerBound);
    auto compare = new IRCompare(nullptr, new LeCompare(_upperBound.get(), traceStaticLink(f, _identifier).release(), upperBound));
    assert(compare->falseBranchLabels().size() == 1);

    f.setLoopBreakLabel(compare->falseBranchLabels().back());

    auto body = _body->toIR(f).release()->toStatement();
    auto rejump = new SequenceExpression(nullptr, new Jump(new Label(startLabel)), new Label(compare->falseBranchLabels().back()));

    auto bodyPart = new SequenceExpression(nullptr, body, rejump);

    auto trueBranch = IRCompare::makeLabelTree(compare->trueBranchLabels(), bodyPart);
    auto condStm = compare->statement().release();
    delete compare;
    auto forIR = new SequenceExpression(nullptr,
        new Label(startLabel),
        new SequenceExpression(nullptr,
            initial,
            new SequenceExpression(nullptr,
                condStm,
                trueBranch)));

    return make_unique<EffectSequence>(this,
        new SequenceExpression(nullptr,
            new ExpressionStatement(nullptr,
                new Minus(nullptr,
                    TemporaryVariable::newSP(),
                    new Constant(4))),
            forIR),
        new Plus(nullptr, TemporaryVariable::newSP(), new Constant(4)));
}

std::unique_ptr<ir::IRTNode> Let::toIR(Frame& f)
{
    FrameScopeGuard fsg{ f };
    auto offset = f.currentFrame()->currentOffset();

    for (auto& decl : _bindings)
    {
        decl->addToFrame(f);
    }
    auto deltaSP = offset - f.currentFrame()->currentOffset();
    std::vector<IRStatement*> nodes;
    nodes.push_back(new ExpressionStatement(nullptr, new Minus(nullptr, TemporaryVariable::newSP(), new Constant(deltaSP))));
    int variableCount = 0;
    for (auto& decl : _bindings)
    {
        if (auto varDecl = dynamic_cast<VariableDeclaration*>(decl.get()))
        {
            auto initializer = varDecl->initializer()->toIR(f).release()->toExpression();
            nodes.push_back(new Move(varDecl, traceStaticLink(f, varDecl->identifier()).release(), initializer));
        }
    }
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto ed = loc().end.column;
    if (this->hasValue())
    {
        assert(_body.size() >= 1);
        for (auto iter = _body.begin(); iter != _body.end() - 1; ++iter)
        {
            nodes.push_back((*iter)->toIR(f).release()->toStatement());
        }
        nodes.push_back(new Move(_body.back().get(), TemporaryVariable::format("letResult", li, co, ed), _body.back()->toIR(f).release()->toExpression()));
    }
    else
    {
        for (auto& item : _body)
        {
            nodes.push_back(item->toIR(f).release()->toStatement());
        }
    }
    nodes.push_back(new ExpressionStatement(nullptr, new Plus(nullptr, TemporaryVariable::newSP(), new Constant(-deltaSP))));

    auto letIR = SequenceExpression::makeExpressionTree(nodes);

    if (hasValue())
    {
        return make_unique<EffectSequence>(this, letIR, TemporaryVariable::format("letResult", li, co, ed));
    }
    return std::unique_ptr<IRTNode>(letIR);
}

void Declaration::addToFrame(Frame& f)
{
    assert(0);
}

void VariableDeclaration::addToFrame(Frame& f)
{
    f.currentFrame()->addVariable(_identifier);
}

void FunctionParameter::addToFrame(Frame& f)
{
    f.currentFrame()->addParameter(_identifier);
}

void FunctionDeclaration::addToFrame(Frame& f)
{
    FrameGuard fg{ f };
    for (auto& param : _parameters)
    {
        param.addToFrame(f);
    }
    auto funcName = Label::name(_identifier, loc().begin.line, loc().begin.column, loc().end.column);
    IRTNode* body;
    if (_body->hasValue())
    {
        body = _body->toIR(f).release()->toExpression();
    }
    else
    {
        body = _body->toIR(f).release()->toStatement();
    }
    f.addFunction(funcName, body);
}

}
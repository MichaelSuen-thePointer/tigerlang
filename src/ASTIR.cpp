#include "TigerAST.hpp"
#include "TigerIRT.hpp"
#include <memory>
namespace tiger
{
using namespace ir;
using std::make_unique;

static std::unique_ptr<MemoryAccess> traceStaticLink(Frame& f, std::string& name)
{
    auto targetFrame = f.currentFrame()->frameByName(name);

    auto currFrame = f.currentFrame();

    std::unique_ptr<IRExpression> ir(TemporaryVariable::newFP());

    while (targetFrame != currFrame)
    {
        ir.reset(new MemoryAccess(new Plus(ir.release(), Constant::newStaticLinkOffset())));
        currFrame = currFrame->staticLink();
        assert(currFrame != nullptr);
    }

    auto offset = currFrame->getOffset(name);

    return make_unique<MemoryAccess>(new Plus(ir.release(), new Constant(offset)));
}


std::unique_ptr<IRTNode> Identifier::toIR(Frame& f)
{
    return traceStaticLink(f, _identifier);
}

std::unique_ptr<IRTNode> Subscript::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<MemoryAccess>(new Plus(left.release()->toExpression(), new Multiply(right.release()->toExpression(), Constant::newWordSize())));
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
        e.identifier() == _identifier;
    });
    assert(i != recTy->fields().end());
    auto offset = (i - recTy->fields().begin()) * TigerFrame::WordSize;
    return make_unique<MemoryAccess>(new Plus(left.release()->toExpression(), new Constant(offset)));
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
    std::string label = "s" + std::to_string(loc().begin.line) + "_" + std::to_string(loc().begin.column);
    f.addStringFragments(label, _value);
    return make_unique<Name>(label);
}

std::unique_ptr<IRTNode> Sequence::toIR(Frame& f)
{
    if (_expressions.size() == 0)
    {
        return make_unique<Nop>();
    }
    if (_expressions.size() == 1)
    {
        return _expressions[0]->toIR(f);
    }
    std::unique_ptr<IRExpression> root;
    std::unique_ptr<IRExpression>* currNode = &root;
    std::for_each(_expressions.begin(), _expressions.end() - 1, [&f, &currNode](const std::unique_ptr<Expression> &item)
    {
        auto peseq = new EffectSequence(item->toIR(f).release()->toStatement(), nullptr);
        currNode->reset(peseq);
        currNode = const_cast<std::unique_ptr<IRExpression>*>(&peseq->right());
        //临时糊一下
    });
    currNode->reset(_expressions.back()->toIR(f).release()->toExpression());
    return root;
}

std::unique_ptr<IRTNode> Negation::toIR(Frame& f)
{
    return make_unique<Minus>(new Constant(0), _expression->toIR(f)->toExpression());
}

std::unique_ptr<IRTNode> Call::toIR(Frame& f)
{
    std::vector<IRExpression*> args;
    std::transform(_arguments.begin(), _arguments.end(), std::back_inserter(args), [&f](std::unique_ptr<Expression>& e)
    {
        return e->toIR(f).release()->toExpression();
    });
    return make_unique<FunctionCall>(new Name(_identifier), args);
}


std::unique_ptr<ir::IRTNode> Add::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<Plus>(left.release()->toExpression(), right.release()->toExpression());
}

std::unique_ptr<ir::IRTNode> Sub::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<Minus>(left.release()->toExpression(), right.release()->toExpression());
}

std::unique_ptr<ir::IRTNode> Mul::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<Multiply>(left.release()->toExpression(), right.release()->toExpression());
}

std::unique_ptr<ir::IRTNode> Div::toIR(Frame& f)
{
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    return make_unique<Divide>(left.release()->toExpression(), right.release()->toExpression());
}

std::unique_ptr<IRTNode> Eq::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    if (_left->expressionType()->isString())
    { //比较字符串
        assert(_right->expressionType()->isString());
        return make_unique<EffectSequence>(
            new Move(TemporaryVariable::format("strcmpresult", li, co),
                new FunctionCall(new Name("builtin_streq"), std::vector<IRExpression*>{ left.release()->toExpression(),
                    right.release()->toExpression() })),
            TemporaryVariable::format("strcmpresult", li, co));
    }
    return CompareJump::makeCJump<EqCompare>(left.release()->toExpression(), right.release()->toExpression(), li, co);
}

std::unique_ptr<IRTNode> Ne::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);
    if (_left->expressionType()->isString())
    { //比较字符串
        assert(_right->expressionType()->isString());
        return make_unique<EffectSequence>(
            new Move(TemporaryVariable::format("strcmpresult", li, co),
                new Minus(new Constant(1), new FunctionCall(new Name("builtin_streq"), std::vector<IRExpression*>{ left.release()->toExpression(),
                    right.release()->toExpression() }))),
            TemporaryVariable::format("strcmpresult", li, co));
    }
    return CompareJump::makeCJump<NeCompare>(left.release()->toExpression(), right.release()->toExpression(), li, co);
}

std::unique_ptr<IRTNode> Gt::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);

    return CompareJump::makeCJump<GtCompare>(left.release()->toExpression(), right.release()->toExpression(), li, co);
}

std::unique_ptr<IRTNode> Ge::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);

    return CompareJump::makeCJump<GeCompare>(left.release()->toExpression(), right.release()->toExpression(), li, co);

}

std::unique_ptr<IRTNode> Lt::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);

    return CompareJump::makeCJump<LtCompare>(left.release()->toExpression(), right.release()->toExpression(), li, co);

}

std::unique_ptr<ir::IRTNode> Le::toIR(Frame& f)
{
    auto li = loc().begin.line;
    auto co = loc().begin.column;
    auto left = _left->toIR(f);
    auto right = _right->toIR(f);

    return CompareJump::makeCJump<LeCompare>(left.release()->toExpression(), right.release()->toExpression(), li, co);
}

}
#include <cassert>
#include "TigerIRT.hpp"
#include "TigerFrame.hpp"

namespace tiger
{
namespace ir
{
IRStatement* IRExpression::toStatement()
{
    return new ExpressionStatement(this);
}

IRExpression* IRStatement::toExpression()
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

}
}

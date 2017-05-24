#include <iostream>
#include <typeinfo>
#include "TigerIRT.hpp"
namespace tiger
{
namespace ir
{
void Constant::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"Constant\",\n\"Value\": " << _value << "\n}";
}

void Name::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"Name\",\n\"Name\": \"" << _name << "\"\n}";
}

void TemporaryVariable::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"TempVar\",\n\"Id\": \"" << _id << "\"\n}";
}

void MemoryAccess::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"Mem\",\n\"Offset\": ";
    _offset->dump(out);
    out << "\n}";
}

void BinaryOperation::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"Binop\",\n\"op\": \"" << typeid(*this).name() << "\",\n";
    out << R"("Left": )"; _left->dump(out); out << ",\n";
    out << R"("Right": )"; _right->dump(out); out << "\n";
    out << "}";
}

void FunctionCall::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"FunctionCall\",\n\"Name\": "; _func->dump(out);;
    
    int c = 0;
    for (auto& param : _parameters)
    {
        out << ",\n";
        out << "\"Param " << c << "\": ";
        param->dump(out);
        c++;
    }
    out << "\n}";
}

void EffectSequence::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"Eseq\",\n";
    out << "\"Left\": ";
    _left->dump(out);
    out << ",\n";
    out << "\"Right\": ";
    _right->dump(out);
    out << "\n}";
}

void Move::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"Move\",\n";
    out << "\"Left\": ";
    _target->dump(out);
    out << ",\n";
    out << "\"Right\": ";
    _exp->dump(out);
    out << "\n}";
}

void ExpressionStatement::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"IgnoreValue\",\n\"Exp\": ";
    _exp->dump(out);
    out << "\n}";
}

void Label::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"Label\",\n\"Name\": \"" << _name << "\"\n}";
}

void Jump::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"Jump\",\n\"Target\": ";
    _target->dump(out);
    out << "\n}";
}

void IRCompare::dump(std::ostream& out)
{
    out << "\"`IRCompare` SHOULD NOT APPEAR IN THE FINAL IR TREE!!!\"";
}

void IRCompareJump::_dump(std::ostream& out, std::string cond)
{
    out << "{\n\"Type\": \"" << "CJump" << "\",\n";
    out << R"("Cond": )"; out << "\"" << cond << "\",\n";
    out << R"("Left": )"; _left->dump(out); out << ",\n";
    out << R"("Right": )"; _right->dump(out); out << ",\n";
    out << R"("True": ")" << _trueBranch << "\",\n";
    out << R"("False": ")" << _falseBranch << "\"\n";
    out << "}";
}

    void EqCompare::dump(std::ostream& out) {
        return _dump(out, "eq");
    }

    void NeCompare::dump(std::ostream& out) {
        return _dump(out, "ne");
    }

    void LtCompare::dump(std::ostream& out) {
        return _dump(out, "lt");
    }

    void GtCompare::dump(std::ostream& out) {
        return _dump(out, "gt");
    }

    void GeCompare::dump(std::ostream& out) {
        return _dump(out, "ge");
    }

    void LeCompare::dump(std::ostream& out) {
        return _dump(out, "le");
    }

void SequenceExpression::dump(std::ostream& out)
{
    out << "{\n\"Type\": \"Seq\",\n";
    out << "\"Left\": ";
    _left->dump(out);
    out << ",\n";
    out << "\"Right\": ";
    _right->dump(out);
    out << "\n}";
}

}
}
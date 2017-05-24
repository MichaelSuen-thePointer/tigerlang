//
// Created by InsZVA on 2017/5/23.
//
#include <iostream>
#include "TigerIRT.hpp"

namespace tiger {
    namespace ir {
        void IRTNode::graphviz(std::ostream& out) {
            out << "digraph {" << std::endl << "node [shape=record]\n";
            int id = 0;
            _graphviz(id, out);
            out << "}" << std::endl;
        }

        std::string Constant::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "Constant" + std::to_string(id++);
            out << name << " [label=\"{<f0> Constant|<f1> " << _value << "}\"]\n";
            return name;
        }

        std::string Name::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "Name" + std::to_string(id++);
            out << name << " [label=\"{<f0> Name|<f1> " << _name << "}\"]\n";
            return name;
        }

        std::string TemporaryVariable::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "TempVar" + std::to_string(id++);
            out << name << " [label=\"{<f0> TempVar|<f1> " << _id << "}\"]\n";
            return name;
        }

        std::string MemoryAccess::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "MemoryAccess" + std::to_string(id++);
            out << name << " [label=\"MemoryAccess\"]\n";
            out << name << "->" << _offset->_graphviz(id, out) << "\n";
            return name;
        }

        std::string BinaryOperation::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "BinaryOperation" + std::to_string(id++);
            out << name << " [label=\"{<f0> BinaryOperation|<f1> " << typeid(*this).name() << "}\"]\n";
            out << name << "->" << _left->_graphviz(id, out) << " [label=\"left\"]\n";
            out << name << "->" << _right->_graphviz(id, out) << " [label=\"right\"]\n";
            return name;
        }

        std::string FunctionCall::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "FunctionCall" + std::to_string(id++);
            out << name << " [label=\"FunctionCall\"]\n";
            out << name << "->" << _func->_graphviz(id, out) << " [label=\"func\"]\n";
            int c = 0;
            for (auto& param : _parameters) {
                out << name << "->" << param->_graphviz(id, out) << " [label=\"param" << c << "\"]\n";
            }
            return name;
        }

        std::string EffectSequence::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "EffectSequence" + std::to_string(id++);
            out << name << " [label=\"Eseq\"]\n";
            out << name << "->" << _left->_graphviz(id, out) << " [label=\"left\"]\n";
            out << name << "->" << _right->_graphviz(id, out) << " [label=\"right\"]\n";
            return name;
        }

        std::string Move::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "Move" + std::to_string(id++);
            out << name << " [label=\"Move\"]\n";
            out << name << "->" << _target->_graphviz(id, out) << " [label=\"target\"]\n";
            out << name << "->" << _exp->_graphviz(id, out) << " [label=\"exp\"]\n";
            return name;
        }

        std::string ExpressionStatement::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "IgnoreValue" + std::to_string(id++);
            out << name << " [label=\"MemoryAccess\"]\n";
            out << name << "->" << _exp->_graphviz(id, out) << "\n";
            return name;
        }

        std::string Label::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "Label" + std::to_string(id++);
            out << name << " [label=\"{<f0> Label|<f1> " << _name << "}\"]\n";
            return name;
        }

        std::string Jump::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "Jmp" + std::to_string(id++);
            out << name << " [label=\"Jmp\"]\n";
            out << name << "->" << _target->_graphviz(id, out) << "\n";
            return name;
        }

        std::string IRCompare::_graphviz(int& id, std::ostream& out)
        {
            out << "\"`IRCompare` SHOULD NOT APPEAR IN THE FINAL IR TREE!!!\"";
            return "";
        }

        std::string IRCompareJump::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "CJump" + std::to_string(id++);
            out << name << " [label=\"{<f0> CJump|<f1> T:" << _trueBranch << "|<f2> F:" << _falseBranch << "}\"]\n";
            out << name << "->" << _left->_graphviz(id, out) << " [label=\"left\"]\n";
            out << name << "->" << _right->_graphviz(id, out) << " [label=\"right\"]\n";
            return name;
        }

        std::string SequenceExpression::_graphviz(int& id, std::ostream& out)
        {
            std::string name = "Seq" + std::to_string(id++);
            out << name << " [label=\"Seq\"]\n";
            out << name << "->" << _left->_graphviz(id, out) << " [label=\"left\"]\n";
            out << name << "->" << _right->_graphviz(id, out) << " [label=\"right\"]\n";
            return name;
        }
    }
}
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
    explicit IRTNode(ASTNode* ast);
    virtual ~IRTNode() = default;

    const ASTNode* ast() const;

    ASTNode* ast();
    virtual IRExpression* toExpression() = 0;
    virtual IRStatement* toStatement() = 0;
    virtual IRCompare* toCompare() = 0;

    virtual void dump(std::ostream& out) = 0;
};

class IRExpression : public IRTNode
{
public:
    explicit IRExpression(ASTNode* ast);

    IRExpression* toExpression() final override;
    IRStatement* toStatement() final override;
    IRCompare* toCompare() final override;
};

class IRStatement : public IRTNode
{
public:
    explicit IRStatement(ASTNode* ast);

    IRStatement* toStatement() override;
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

    explicit Constant(int value);

    int value() const;
    void dump(std::ostream& out) override;
};

class Name : public IRExpression
{
protected:
    std::string _name;
public:
    Name(ASTNode* ast, std::string name);

    const std::string& name() const;

    void dump(std::ostream& out) override;
};

class IRMoveTarget : public IRExpression
{
public:
    explicit IRMoveTarget(ASTNode* ast);
};

class TemporaryVariable : public IRMoveTarget
{
protected:
    std::string _id;
public:
    static TemporaryVariable* format(const std::string& desc, int line, int column, int end);

    static TemporaryVariable* newFP();

    static TemporaryVariable* newSP();

    explicit TemporaryVariable(std::string id);

    const std::string& id() const;
    void dump(std::ostream& out) override;
};

class MemoryAccess : public IRMoveTarget
{
protected:
    std::unique_ptr<IRExpression> _offset;
public:

    explicit MemoryAccess(ASTNode* ast, IRExpression* offset);

    const std::unique_ptr<IRExpression>& offset() const;

    void dump(std::ostream& out) override;
};

class BinaryOperation : public IRExpression
{
public:
    std::unique_ptr<IRExpression> _left;
    std::unique_ptr<IRExpression> _right;
public:
    BinaryOperation(ASTNode* ast, IRExpression* left, IRExpression* right);

    const std::unique_ptr<IRExpression>& left() const;

    const std::unique_ptr<IRExpression>& right() const;

    void dump(std::ostream& out) override;
};

class Plus : public BinaryOperation
{
public:
    Plus(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class Minus : public BinaryOperation
{
public:
    Minus(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class Multiply : public BinaryOperation
{
public:
    Multiply(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class Divide : public BinaryOperation
{
public:
    Divide(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class LogicalAnd : public BinaryOperation
{
public:
    LogicalAnd(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class LogicalOr : public BinaryOperation
{
public:
    LogicalOr(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class Xor : public BinaryOperation
{
public:
    Xor(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class LeftShift : public BinaryOperation
{
public:
    LeftShift(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class RightShift : public BinaryOperation
{
public:
    RightShift(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class FunctionCall : public IRExpression
{
protected:
    std::unique_ptr<IRExpression> _func;
    std::vector<std::unique_ptr<IRExpression>> _parameters;
public:
    FunctionCall(ASTNode* ast, IRExpression* func, std::vector<IRExpression*>& exp);

    FunctionCall(ASTNode* ast, IRExpression* func, std::vector<IRExpression*>&& exp);

    const std::unique_ptr<IRExpression>& function() const;

    const std::vector<std::unique_ptr<IRExpression>>& parameters() const;
    void dump(std::ostream& out) override;
};

class EffectSequence : public IRExpression
{
protected:
    std::unique_ptr<IRStatement> _left;
    std::unique_ptr<IRExpression> _right;
public:
    EffectSequence(ASTNode* ast, IRStatement* left, IRExpression* right);

    const std::unique_ptr<IRStatement>& left() const;

    const std::unique_ptr<IRExpression>& right() const;

    std::unique_ptr<IRExpression>& right();

    void dump(std::ostream& out) override;
};

class Move : public IRStatement
{
protected:
    std::unique_ptr<IRMoveTarget> _target;
    std::unique_ptr<IRExpression> _exp;
public:
    Move(ASTNode* ast, IRMoveTarget* target, IRExpression* exp);

    const std::unique_ptr<IRMoveTarget>& target() const;

    const std::unique_ptr<IRExpression>& exp() const;

    void dump(std::ostream& out) override;
};

class ExpressionStatement : public IRStatement
{
protected:
    std::unique_ptr<IRExpression> _exp;
public:
    explicit ExpressionStatement(ASTNode* ast, IRExpression* exp);

    const std::unique_ptr<IRExpression>& exp() const;

    void dump(std::ostream& out) override;
};

class Label : public IRStatement
{
protected:
    std::string _name;
public:
    static Label* format(const std::string& desc, int line, int column, int end);

    static std::string name(const std::string& desc, int line, int column, int end);

    explicit Label(std::string name);

    const std::string& name() const;
    void dump(std::ostream& out) override;
};

class Jump : public IRStatement
{
protected:
    std::unique_ptr<Label> _target;
public:
    explicit Jump(Label* target);

    const std::unique_ptr<Label>& target() const;
    void dump(std::ostream& out) override;
};

class SequenceExpression;
class IRCompareJump;

class IRCompare : public IRStatement
{
protected:
    std::unique_ptr<IRStatement> _statement;
    std::vector<IRCompareJump*> _trueBranches;
    std::vector<IRCompareJump*> _falseBranches;
public:
    IRCompare(ASTNode* ast, IRCompareJump* ircjump);

    std::unique_ptr<IRStatement>& statement();

    const std::vector<IRCompareJump*>& trueBranches() const;

    const std::vector<IRCompareJump*>& falseBranches() const;

    void mergeTrueBranches(const std::vector<IRCompareJump*>& r);

    void mergeFalseBranches(const std::vector<IRCompareJump*>& r);

    void refillTrueBranchLabel(const std::string& s);

    void refillFalseBranchLabel(const std::string& s);

    static IRStatement* makeLabelTree(const std::vector<std::string>& labels, IRStatement* stm);
    IRStatement* toStatement() override;
    IRExpression* toExpression() override;
    IRCompare* toCompare() override;
    void dump(std::ostream& out) override;
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

    const std::unique_ptr<IRExpression>& left() const;

    const std::unique_ptr<IRExpression>& right() const;

    const std::string& trueBranch() const;

    const std::string& falseBranch() const;

    void trueBranch(std::string trueBranch);

    void falseBranch(std::string falseBranch);

    void dump(std::ostream& out) override;
};

class EqCompare : public IRCompareJump
{
public:
    EqCompare(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class NeCompare : public IRCompareJump
{
public:
    NeCompare(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class GtCompare : public IRCompareJump
{
public:
    GtCompare(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class GeCompare : public IRCompareJump
{
public:
    GeCompare(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class LtCompare : public IRCompareJump
{
public:
    LtCompare(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class LeCompare : public IRCompareJump
{
public:
    LeCompare(ASTNode* ast, IRExpression* left, IRExpression* right);
};

class SequenceExpression : public IRStatement
{
protected:
    std::unique_ptr<IRStatement> _left;
    std::unique_ptr<IRStatement> _right;
public:
    static IRStatement* makeExpressionTree(const std::vector<IRStatement*> stmts);

    SequenceExpression(ASTNode* ast, IRStatement* left, IRStatement* right);

    const std::unique_ptr<IRStatement>& left() const;

    const std::unique_ptr<IRStatement>& right() const;

    std::unique_ptr<IRStatement>& right();
    void dump(std::ostream& out) override;
};
}
}

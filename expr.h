#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <vector>
#include <memory>

class ExprVisitor;

class Expr;

using ExprPtr = std::shared_ptr<Expr>;

class Expr {
public:
    virtual ~Expr() = default;

    virtual void accept(ExprVisitor &visitor) const = 0;
};

class IdentifierExpr : public Expr {
public:
    std::string name;

    IdentifierExpr(const std::string &name) : name(name) {}

    void accept(ExprVisitor &visitor) const override;
};

class NumberExpr : public Expr {
public:
    double value;

    NumberExpr(double value) : value(value) {}

    void accept(ExprVisitor &visitor) const override;
};

class StringExpr : public Expr {
public:
    std::string value;

    StringExpr(const std::string &value) : value(value) {}

    void accept(ExprVisitor &visitor) const override;
};

class MemberExpr : public Expr {
public:
    ExprPtr object;
    std::string member;

    MemberExpr(ExprPtr object, const std::string &member)
            : object(std::move(object)), member(member) {}

    void accept(ExprVisitor &visitor) const override;
};

class SubscriptExpr : public Expr {
public:
    ExprPtr array;
    ExprPtr index;

    SubscriptExpr(ExprPtr array, ExprPtr index)
            : array(std::move(array)), index(std::move(index)) {}

    void accept(ExprVisitor &visitor) const override;
};

class CallExpr : public Expr {
public:
    std::string callee;
    std::vector<ExprPtr> arguments;

    CallExpr(const std::string &callee, std::vector<ExprPtr> arguments)
            : callee(callee), arguments(std::move(arguments)) {}

    void accept(ExprVisitor &visitor) const override;
};

class BinaryExpr : public Expr {
public:
    enum class Operator {
        Add, Subtract, Multiply, Divide
    };
    ExprPtr left;
    ExprPtr right;
    Operator op;

    BinaryExpr(ExprPtr left, Operator op, ExprPtr right)
            : left(std::move(left)), op(op), right(std::move(right)) {}

    void accept(ExprVisitor &visitor) const override;
};

#endif // EXPR_H

#include "expr.h"
#include "expr_visitor.h"

void IdentifierExpr::accept(ExprVisitor &visitor) const {
    visitor.visit(*this);
}

void NumberExpr::accept(ExprVisitor &visitor) const {
    visitor.visit(*this);
}

void StringExpr::accept(ExprVisitor &visitor) const {
    visitor.visit(*this);
}

void MemberExpr::accept(ExprVisitor &visitor) const {
    visitor.visit(*this);
}

void SubscriptExpr::accept(ExprVisitor &visitor) const {
    visitor.visit(*this);
}

void CallExpr::accept(ExprVisitor &visitor) const {
    visitor.visit(*this);
}

void BinaryExpr::accept(ExprVisitor &visitor) const {
    visitor.visit(*this);
}

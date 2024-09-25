#ifndef EXPR_VISITOR_H
#define EXPR_VISITOR_H

#include "expr.h"
#include "json_parser.h"

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual void visit(const IdentifierExpr &expr) = 0;

    virtual void visit(const NumberExpr &expr) = 0;

    virtual void visit(const StringExpr &expr) = 0;

    virtual void visit(const MemberExpr &expr) = 0;

    virtual void visit(const SubscriptExpr &expr) = 0;

    virtual void visit(const CallExpr &expr) = 0;

    virtual void visit(const BinaryExpr &expr) = 0;

    JSONValue result;
};

#endif // EXPR_VISITOR_H

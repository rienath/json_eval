#ifndef EXPR_EVALUATOR_H
#define EXPR_EVALUATOR_H

#include "expr_visitor.h"
#include <functional>
#include <unordered_map>

class ExprEvaluator : public ExprVisitor {
public:
    explicit ExprEvaluator(const JSONValue &root);

    void visit(const IdentifierExpr &expr) override;

    void visit(const NumberExpr &expr) override;

    void visit(const StringExpr &expr) override;

    void visit(const MemberExpr &expr) override;

    void visit(const SubscriptExpr &expr) override;

    void visit(const CallExpr &expr) override;

    void visit(const BinaryExpr &expr) override;

    [[nodiscard]] std::string jsonValueToString(const JSONValue &value) const;

private:
    const JSONValue &root;
    JSONValue currentValue;

    using FunctionType = std::function<JSONValue(const std::vector<JSONValue> &)>;
    std::unordered_map<std::string, FunctionType> functions;

    [[nodiscard]] static JSONValue getValue(const JSONValue &value, const std::string &key);

    [[nodiscard]] static JSONValue getValue(const JSONValue &value, size_t index);

    void initializeFunctions();
};

#endif // EXPR_EVALUATOR_H

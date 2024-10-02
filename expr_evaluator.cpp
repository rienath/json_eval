#include "expr_evaluator.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <sstream>

ExprEvaluator::ExprEvaluator(const JSONValue &root) : root(root) {
    initializeFunctions();
}

void ExprEvaluator::initializeFunctions() { // NOLINT
    functions["min"] = [](const std::vector<JSONValue> &args) -> JSONValue {
        if (args.empty()) {
            throw std::runtime_error("min() requires at least one argument");
        }
        double minValue = std::numeric_limits<double>::max();
        for (const auto &arg: args) {
            if (arg.isNumber()) {
                minValue = std::min(minValue, arg.asNumber());
            } else if (arg.isArray()) {
                for (const auto &item: arg.asArray()) {
                    if (item.isNumber()) {
                        minValue = std::min(minValue, item.asNumber());
                    }
                }
            } else {
                throw std::runtime_error("min() arguments must be numbers or arrays of numbers");
            }
        }
        return minValue;
    };

    functions["max"] = [](const std::vector<JSONValue> &args) -> JSONValue {
        if (args.empty()) {
            throw std::runtime_error("max() requires at least one argument");
        }
        double maxValue = std::numeric_limits<double>::lowest();
        for (const auto &arg: args) {
            if (arg.isNumber()) {
                maxValue = std::max(maxValue, arg.asNumber());
            } else if (arg.isArray()) {
                for (const auto &item: arg.asArray()) {
                    if (item.isNumber()) {
                        maxValue = std::max(maxValue, item.asNumber());
                    }
                }
            } else {
                throw std::runtime_error("max() arguments must be numbers or arrays of numbers");
            }
        }
        return maxValue;
    };

    functions["size"] = [](const std::vector<JSONValue> &args) -> JSONValue {
        if (args.size() != 1) {
            throw std::runtime_error("size() requires exactly one argument");
        }
        const auto &arg = args[0];
        if (arg.isObject()) {
            return static_cast<double>(arg.asObject().size());
        }
        if (arg.isArray()) {
            return static_cast<double>(arg.asArray().size());
        }
        if (arg.isString()) {
            return static_cast<double>(arg.asString().size());
        }
        throw std::runtime_error("size() argument must be an object, array, or string");
    };
}

void ExprEvaluator::visit(const IdentifierExpr &expr) {
    if (expr.name == "null") {
        result = nullptr;
    } else if (expr.name == "true") {
        result = true;
    } else if (expr.name == "false") {
        result = false;
    } else {
        if (!root.isObject()) {
            throw std::runtime_error("Root JSON is not an object");
        }
        result = getValue(root, expr.name);
    }
}

void ExprEvaluator::visit(const NumberExpr &expr) {
    result = expr.value;
}

void ExprEvaluator::visit(const StringExpr &expr) {
    result = expr.value;
}

void ExprEvaluator::visit(const MemberExpr &expr) {
    expr.object->accept(*this);
    if (!result.isObject()) {
        throw std::runtime_error("Attempted to access member of non-object");
    }
    result = getValue(result, expr.member);
}

void ExprEvaluator::visit(const SubscriptExpr &expr) {
    expr.array->accept(*this);
    JSONValue arrayValue = result;

    expr.index->accept(*this);
    JSONValue indexValue = result;

    if (arrayValue.isArray()) {
        if (!indexValue.isNumber()) {
            throw std::runtime_error("Array index must be a number");
        }
        auto index = static_cast<size_t>(indexValue.asNumber());
        result = getValue(arrayValue, index);
    } else if (arrayValue.isObject()) {
        if (!indexValue.isString()) {
            throw std::runtime_error("Object index must be a string");
        }
        result = getValue(arrayValue, indexValue.asString());
    } else {
        throw std::runtime_error("Attempted to index non-array/non-object");
    }
}

void ExprEvaluator::visit(const CallExpr &expr) {
    auto itr = functions.find(expr.callee);
    if (itr == functions.end()) {
        throw std::runtime_error("Unknown function: " + expr.callee);
    }
    std::vector<JSONValue> args;
    for (const auto &arg: expr.arguments) {
        arg->accept(*this);
        args.push_back(result);
    }
    result = itr->second(args);
}

void ExprEvaluator::visit(const BinaryExpr &expr) {
    expr.left->accept(*this);
    JSONValue leftValue = result;

    expr.right->accept(*this);
    JSONValue rightValue = result;

    if (!leftValue.isNumber() || !rightValue.isNumber()) {
        throw std::runtime_error("Binary operations require numeric operands");
    }

    double leftNum = leftValue.asNumber();
    double rightNum = rightValue.asNumber();
    switch (expr.op) {
        case BinaryExpr::Operator::Add:
            result = leftNum + rightNum;
            break;
        case BinaryExpr::Operator::Subtract:
            result = leftNum - rightNum;
            break;
        case BinaryExpr::Operator::Multiply:
            result = leftNum * rightNum;
            break;
        case BinaryExpr::Operator::Divide:
            if (rightNum == 0) {
                throw std::runtime_error("Division by zero");
            }
            result = leftNum / rightNum;
            break;
    }
}

JSONValue ExprEvaluator::getValue(const JSONValue &value, const std::string &key) {
    if (!value.isObject()) {
        throw std::runtime_error("Attempted to access member of non-object");
    }
    const auto &obj = value.asObject();
    auto itr = obj.find(key);
    if (itr == obj.end()) {
        throw std::runtime_error("Key not found: " + key);
    }
    return itr->second;
}

JSONValue ExprEvaluator::getValue(const JSONValue &value, size_t index) {
    if (!value.isArray()) {
        throw std::runtime_error("Attempted to index non-array");
    }
    const auto &arr = value.asArray();
    if (index >= arr.size()) {
        throw std::runtime_error("Array index out of bounds");
    }
    return arr[index];
}

std::string ExprEvaluator::jsonValueToString(const JSONValue &value) const {
    if (value.isNull()) {
        return "null";
    }
    if (value.isBool()) {
        return value.asBool() ? "true" : "false";
    }
    if (value.isNumber()) {
        double num = value.asNumber();
        if (std::trunc(num) == num) {
            // Number is an integer
            std::ostringstream oss;
            oss << static_cast<int64_t>(num);
            return oss.str();
        }
        // Number is a floating-point value
        std::ostringstream oss;
        oss << num;
        return oss.str();
    }
    if (value.isString()) {
        return value.asString();
    }
    if (value.isArray()) {
        std::string result = "[ ";
        for (const auto &item: value.asArray()) {
            result += jsonValueToString(item) + ", ";
        }
        if (result.size() > 2) { result.resize(result.size() - 2); }
        result += " ]";
        return result;
    }
    if (value.isObject()) {
        std::string result = "{ ";
        for (const auto &[key, val]: value.asObject()) {
            result += "\"" + key + "\": " + jsonValueToString(val) + ", ";
        }
        if (result.size() > 2) { result.resize(result.size() - 2); }
        result += " }";
        return result;
    }
    return "";
}

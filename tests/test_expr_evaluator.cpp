#include "expr_evaluator.h"
#include "json_parser.h"
#include "expr_parser.h"
#include "gtest/gtest.h"

// clang-format off
class ExprEvaluatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::string json_content = "{\n"
                                   "  \"a\": {\n"
                                   "    \"b\": [\n"
                                   "      1,\n"
                                   "      2,\n"
                                   "      {\n"
                                   "        \"c\": \"test\"\n"
                                   "      },\n"
                                   "      [\n"
                                   "        11,\n"
                                   "        12\n"
                                   "      ]\n"
                                   "    ]\n"
                                   "  }\n"
                                   "}";
        JSONParser parser(json_content);
        jsonRoot = parser.parse();
    }

    JSONValue jsonRoot;
};

TEST_F(ExprEvaluatorTest, EvaluateIdentifier) {
    ExprParser parser("a");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isObject());
}

TEST_F(ExprEvaluatorTest, EvaluateMemberExpression) {
    ExprParser parser("a.b");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isArray());
}

TEST_F(ExprEvaluatorTest, EvaluateSubscriptExpression) {
    ExprParser parser("a.b[1]");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isNumber());
    EXPECT_EQ(evaluator.result.asNumber(), 2);
}

TEST_F(ExprEvaluatorTest, EvaluateNestedSubscript) {
    ExprParser parser("a.b[a.b[1]].c");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isString());
    EXPECT_EQ(evaluator.result.asString(), "test");
}

TEST_F(ExprEvaluatorTest, EvaluateIntrinsicFunctionAverage) {
ExprParser parser("average(a.b[3], 5)");
ExprPtr expr = parser.parse();
ExprEvaluator evaluator(jsonRoot);
expr->accept(evaluator);
EXPECT_TRUE(evaluator.result.isNumber());
EXPECT_EQ(static_cast<int>(evaluator.result.asNumber()), 9);
}

TEST_F(ExprEvaluatorTest, EvaluateIntrinsicFunctionMax) {
    ExprParser parser("max(a.b[0], a.b[1])");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isNumber());
    EXPECT_EQ(evaluator.result.asNumber(), 2);
}

TEST_F(ExprEvaluatorTest, EvaluateIntrinsicFunctionMin) {
    ExprParser parser("min(a.b[3])");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isNumber());
    EXPECT_EQ(evaluator.result.asNumber(), 11);
}

TEST_F(ExprEvaluatorTest, EvaluateIntrinsicFunctionSize) {
    ExprParser parser("size(a.b)");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isNumber());
    EXPECT_EQ(evaluator.result.asNumber(), 4);
}

TEST_F(ExprEvaluatorTest, EvaluateArithmeticExpression) {
    ExprParser parser("a.b[0] + a.b[1]");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isNumber());
    EXPECT_EQ(evaluator.result.asNumber(), 3);
}

TEST_F(ExprEvaluatorTest, EvaluateArithmeticExpressionModulo) {
    ExprParser parser("a.b[0] % a.b[1]");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isNumber());
    EXPECT_EQ(evaluator.result.asNumber(), 1);
}

TEST_F(ExprEvaluatorTest, EvaluateArithmeticExpressionParallel) {
    std::string expression_str = "min(a.b[3]) + size(a.b)";
    if (!expression_str.empty() && expression_str.front() == '"' && expression_str.back() == '"') {
        expression_str = expression_str.substr(1, expression_str.size() - 2);
    }
    ExprParser parser(expression_str);
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isNumber());
    EXPECT_EQ(evaluator.result.asNumber(), 15);
}

TEST_F(ExprEvaluatorTest, EvaluateNumberLiteral) {
    std::string expression_str = "max(a.b[0], 10, a.b[1], 15)";
    ExprParser parser(expression_str);
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    expr->accept(evaluator);
    EXPECT_TRUE(evaluator.result.isNumber());
    EXPECT_EQ(evaluator.result.asNumber(), 15);
}

TEST_F(ExprEvaluatorTest, EvaluateInvalidMemberAccess) {
    ExprParser parser("a.x");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    EXPECT_THROW(expr->accept(evaluator), std::runtime_error);
}

TEST_F(ExprEvaluatorTest, EvaluateDivisionByZero) {
    ExprParser parser("a.b[0] / 0");
    ExprPtr expr = parser.parse();
    ExprEvaluator evaluator(jsonRoot);
    EXPECT_THROW(expr->accept(evaluator), std::runtime_error);
}

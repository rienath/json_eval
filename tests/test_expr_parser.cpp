#include "expr_parser.h"
#include "gtest/gtest.h"

// clang-format off
TEST(ExprParserTest, ParseIdentifier) {
ExprParser parser("a");
ExprPtr expr = parser.parse();
EXPECT_NE(expr, nullptr);
auto idExpr = std::dynamic_pointer_cast<IdentifierExpr>(expr);
EXPECT_NE(idExpr, nullptr);
EXPECT_EQ(idExpr->name, "a");
}

TEST(ExprParserTest, ParseNumber) {
ExprParser parser("123");
ExprPtr expr = parser.parse();
EXPECT_NE(expr, nullptr);
auto numExpr = std::dynamic_pointer_cast<NumberExpr>(expr);
EXPECT_NE(numExpr, nullptr);
EXPECT_EQ(numExpr->value, 123);
}

TEST(ExprParserTest, ParseString) {
ExprParser parser("\"test\"");
ExprPtr expr = parser.parse();
EXPECT_NE(expr, nullptr);
auto strExpr = std::dynamic_pointer_cast<StringExpr>(expr);
EXPECT_NE(strExpr, nullptr);
EXPECT_EQ(strExpr->value, "test");
}

TEST(ExprParserTest, ParseMemberExpression) {
ExprParser parser("a.b");
ExprPtr expr = parser.parse();
EXPECT_NE(expr, nullptr);
auto memberExpr = std::dynamic_pointer_cast<MemberExpr>(expr);
EXPECT_NE(memberExpr, nullptr);
auto idExpr = std::dynamic_pointer_cast<IdentifierExpr>(memberExpr->object);
EXPECT_NE(idExpr, nullptr);
EXPECT_EQ(idExpr->name, "a");
EXPECT_EQ(memberExpr->member, "b");
}

TEST(ExprParserTest, ParseSubscriptExpression) {
ExprParser parser("a[0]");
ExprPtr expr = parser.parse();
EXPECT_NE(expr, nullptr);
auto subExpr = std::dynamic_pointer_cast<SubscriptExpr>(expr);
EXPECT_NE(subExpr, nullptr);
auto idExpr = std::dynamic_pointer_cast<IdentifierExpr>(subExpr->array);
EXPECT_NE(idExpr, nullptr);
EXPECT_EQ(idExpr->name, "a");
auto indexExpr = std::dynamic_pointer_cast<NumberExpr>(subExpr->index);
EXPECT_NE(indexExpr, nullptr);
EXPECT_EQ(indexExpr->value, 0);
}

TEST(ExprParserTest, ParseCallExpression) {
ExprParser parser("max(a, b)");
ExprPtr expr = parser.parse();
EXPECT_NE(expr, nullptr);
auto callExpr = std::dynamic_pointer_cast<CallExpr>(expr);
EXPECT_NE(callExpr, nullptr);
EXPECT_EQ(callExpr->callee, "max");
EXPECT_EQ(callExpr->arguments.size(), 2);
}

TEST(ExprParserTest, ParseBinaryExpression) {
ExprParser parser("a + b");
ExprPtr expr = parser.parse();
EXPECT_NE(expr, nullptr);
auto binExpr = std::dynamic_pointer_cast<BinaryExpr>(expr);
EXPECT_NE(binExpr, nullptr);
EXPECT_EQ(binExpr->op, BinaryExpr::Operator::Add);
}

TEST(ExprParserTest, InvalidExpression) {
ExprParser parser("a + ");
EXPECT_THROW(parser.parse(), std::runtime_error);
}

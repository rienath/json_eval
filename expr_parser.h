#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H

#include "expr.h"
#include <string>

class ExprParser {
public:
    ExprParser(const std::string &input);

    ExprPtr parse();

private:
    const std::string &input;
    size_t pos;

    void skipWhitespace();

    char peek() const;

    char get();

    bool match(char expected);

    ExprPtr parseExpression();

    ExprPtr parseTerm();

    ExprPtr parseFactor();

    ExprPtr parsePrimary();

    std::shared_ptr<IdentifierExpr> parseIdentifier();

    ExprPtr parseNumber();

    ExprPtr parseString();

    std::vector<ExprPtr> parseArguments();
};

#endif // EXPR_PARSER_H

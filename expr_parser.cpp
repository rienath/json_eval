#include "expr_parser.h"
#include <cctype>
#include <stdexcept>

ExprParser::ExprParser(const std::string &input) : input(input), pos(0) {}

void ExprParser::skipWhitespace() {
    while (pos < input.size() && std::isspace(static_cast<unsigned char>(input[pos]))) {
        ++pos;
    }
}

char ExprParser::peek() const {
    return pos < input.size() ? input[pos] : '\0';
}

char ExprParser::get() {
    return pos < input.size() ? input[pos++] : '\0';
}

bool ExprParser::match(char expected) {
    if (peek() == expected) {
        ++pos;
        return true;
    }
    return false;
}

ExprPtr ExprParser::parse() {
    ExprPtr expr = parseExpression();
    skipWhitespace();
    if (pos != input.size()) {
        throw std::runtime_error("Unexpected characters at end of expression");
    }
    return expr;
}

ExprPtr ExprParser::parseExpression() {
    ExprPtr expr = parseTerm();
    skipWhitespace();
    while (true) {
        if (match('+')) {
            ExprPtr right = parseTerm();
            expr = std::make_shared<BinaryExpr>(expr, BinaryExpr::Operator::Add, right);
        } else if (match('-')) {
            ExprPtr right = parseTerm();
            expr = std::make_shared<BinaryExpr>(expr, BinaryExpr::Operator::Subtract, right);
        } else {
            break;
        }
    }
    return expr;
}

ExprPtr ExprParser::parseTerm() {
    ExprPtr expr = parseFactor();
    skipWhitespace();
    while (true) {
        if (match('*')) {
            ExprPtr right = parseFactor();
            expr = std::make_shared<BinaryExpr>(expr, BinaryExpr::Operator::Multiply, right);
        } else if (match('/')) {
            ExprPtr right = parseFactor();
            expr = std::make_shared<BinaryExpr>(expr, BinaryExpr::Operator::Divide, right);
        } else {
            break;
        }
    }
    return expr;
}

ExprPtr ExprParser::parseFactor() {
    skipWhitespace();
    ExprPtr expr = parsePrimary();
    skipWhitespace();

    while (true) {
        if (match('.')) {
            std::string member = parseIdentifier()->name;
            expr = std::make_shared<MemberExpr>(expr, member);
        } else if (match('[')) {
            ExprPtr index = parseExpression();
            if (!match(']')) {
                throw std::runtime_error("Expected ']'");
            }
            expr = std::make_shared<SubscriptExpr>(expr, index);
        } else {
            break;
        }
    }
    return expr;
}

ExprPtr ExprParser::parsePrimary() {
    skipWhitespace();
    char ch = peek();

    if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '_') {
        ExprPtr id = parseIdentifier();
        skipWhitespace();
        if (match('(')) {
            std::vector<ExprPtr> args = parseArguments();
            if (!match(')')) {
                throw std::runtime_error("Expected ')'");
            }
            return std::make_shared<CallExpr>(std::static_pointer_cast<IdentifierExpr>(id)->name, args);
        }
        return id;
    } else if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '-') {
        return parseNumber();
    } else if (ch == '"') {
        return parseString();
    } else if (match('(')) {
        ExprPtr expr = parseExpression();
        if (!match(')')) {
            throw std::runtime_error("Expected ')'");
        }
        return expr;
    } else {
        throw std::runtime_error("Unexpected character in expression");
    }
}

std::shared_ptr<IdentifierExpr> ExprParser::parseIdentifier() {
    std::string name;
    while (pos < input.size() && (std::isalnum(static_cast<unsigned char>(input[pos])) || input[pos] == '_')) {
        name += input[pos++];
    }
    if (name.empty()) {
        throw std::runtime_error("Expected identifier");
    }
    return std::make_shared<IdentifierExpr>(name);
}

ExprPtr ExprParser::parseNumber() {
    size_t start = pos;
    if (match('-')) {}
    while (std::isdigit(static_cast<unsigned char>(peek()))) {
        get();
    }
    if (match('.')) {
        while (std::isdigit(static_cast<unsigned char>(peek()))) {
            get();
        }
    }
    double number = std::stod(input.substr(start, pos - start));
    return std::make_shared<NumberExpr>(number);
}

ExprPtr ExprParser::parseString() {
    std::string result;
    match('"');
    while (true) {
        char ch = get();
        if (ch == '"') {
            break;
        } else if (ch == '\\') {
            ch = get();
            switch (ch) {
                case '"':
                    result += '"';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case '/':
                    result += '/';
                    break;
                case 'b':
                    result += '\b';
                    break;
                case 'f':
                    result += '\f';
                    break;
                case 'n':
                    result += '\n';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case 't':
                    result += '\t';
                    break;
                default:
                    throw std::runtime_error("Invalid escape sequence in string");
            }
        } else if (ch == '\0') {
            throw std::runtime_error("Unterminated string literal");
        } else {
            result += ch;
        }
    }
    return std::make_shared<StringExpr>(result);
}

std::vector<ExprPtr> ExprParser::parseArguments() {
    std::vector<ExprPtr> args;
    skipWhitespace();
    if (peek() == ')') {
        return args;
    }
    while (true) {
        args.push_back(parseExpression());
        skipWhitespace();
        if (!match(',')) {
            break;
        }
        skipWhitespace();
    }
    return args;
}

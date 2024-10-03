// TODO speed up implementation using multithreading

#include <iostream>
#include <fstream>
#include <sstream>
#include "json_parser.h"
#include "expr_parser.h"
#include "expr_evaluator.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./json_eval <json_file> <expression>" << '\n';
        return 1;
    }

    const char *json_filename = argv[1]; // NOLINT
    std::string expression_str = argv[2]; // NOLINT

    // Remove leading and trailing quotation marks if present
    if (!expression_str.empty() && expression_str.front() == '"' && expression_str.back() == '"') {
        expression_str = expression_str.substr(1, expression_str.size() - 2);
    }

    // Read JSON file
    std::ifstream json_file(json_filename);
    if (!json_file) {
        std::cerr << "Failed to open JSON file: " << json_filename << '\n';
        return 1;
    }
    std::stringstream buffer;
    buffer << json_file.rdbuf();
    std::string json_content = buffer.str();

    // Parse JSON
    JSONParser json_parser(json_content);
    JSONValue root;
    try {
        root = json_parser.parse();
    } catch (const std::exception &ex) {
        std::cerr << "JSON parsing error: " << ex.what() << '\n';
        return 1;
    }

    // Parse expression
    // std::cout << "Expression to parse: \"" << expression_str << "\"" << '\n';
    ExprParser expr_parser(expression_str);
    ExprPtr expr;
    try {
        expr = expr_parser.parse();
    } catch (const std::exception &ex) {
        std::cerr << "Expression parsing error: " << ex.what() << '\n';
        return 1;
    }

    // Evaluate expression
    ExprEvaluator evaluator(root);
    try {
        expr->accept(evaluator);
        std::cout << evaluator.jsonValueToString(evaluator.result) << '\n';
    } catch (const std::exception &ex) {
        std::cerr << "Evaluation error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}

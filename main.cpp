// TODO fix ./json_eval test.json "a.b[1]" always returning doubles
// TODO fix ./json_eval test.json "max(a.b[0], 10, a.b[1], 15)"
// TODO add GTest
// TODO add clang-tidy checks
// TODO add readme
// TODO implement average() function
// TODO implement comparison operators
// TODO speed up implementation using multithreading

#include <iostream>
#include <fstream>
#include <sstream>
#include "json_parser.h"
#include "expr_parser.h"
#include "expr_evaluator.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./json_eval <json_file> <expression>" << std::endl;
        return 1;
    }

    const char *json_filename = argv[1];
    const char *expression_str = argv[2];

    // Read JSON file
    std::ifstream json_file(json_filename);
    if (!json_file) {
        std::cerr << "Failed to open JSON file: " << json_filename << std::endl;
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
        std::cerr << "JSON parsing error: " << ex.what() << std::endl;
        return 1;
    }

    // Parse expression
    ExprParser expr_parser(expression_str);
    ExprPtr expr;
    try {
        expr = expr_parser.parse();
    } catch (const std::exception &ex) {
        std::cerr << "Expression parsing error: " << ex.what() << std::endl;
        return 1;
    }

    // Evaluate expression
    ExprEvaluator evaluator(root);
    try {
        expr->accept(evaluator);
        std::cout << evaluator.jsonValueToString(evaluator.result) << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Evaluation error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

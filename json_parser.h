#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

class JSONValue;

using JSONArray = std::vector<JSONValue>;
using JSONObject = std::unordered_map<std::string, JSONValue>;

class JSONValue {
public:
    using ValueType = std::variant<
            std::nullptr_t,
            bool,
            double,
            std::string,
            JSONArray,
            JSONObject>;

    JSONValue() : value(nullptr) {}

    JSONValue(std::nullptr_t) : value(nullptr) {}

    JSONValue(bool b) : value(b) {}

    JSONValue(double d) : value(d) {}

    JSONValue(const std::string &s) : value(s) {}

    JSONValue(const char *s) : value(std::string(s)) {}

    JSONValue(const JSONArray &arr) : value(arr) {}

    JSONValue(const JSONObject &obj) : value(obj) {}

    ValueType value;

    // Type checking methods
    bool isNull() const { return std::holds_alternative<std::nullptr_t>(value); }

    bool isBool() const { return std::holds_alternative<bool>(value); }

    bool isNumber() const { return std::holds_alternative<double>(value); }

    bool isString() const { return std::holds_alternative<std::string>(value); }

    bool isArray() const { return std::holds_alternative<JSONArray>(value); }

    bool isObject() const { return std::holds_alternative<JSONObject>(value); }

    // Value access methods
    bool asBool() const { return std::get<bool>(value); }

    double asNumber() const { return std::get<double>(value); }

    const std::string &asString() const { return std::get<std::string>(value); }

    const JSONArray &asArray() const { return std::get<JSONArray>(value); }

    const JSONObject &asObject() const { return std::get<JSONObject>(value); }
};

class JSONParser {
public:
    JSONParser(const std::string &input);

    JSONValue parse();

private:
    const std::string &input;
    size_t pos;

    void skipWhitespace();

    char peek() const;

    char get();

    bool match(char expected);

    JSONValue parseValue();

    JSONValue parseObject();

    JSONValue parseArray();

    JSONValue parseString();

    JSONValue parseNumber();

    JSONValue parseTrue();

    JSONValue parseFalse();

    JSONValue parseNull();

    std::string parseRawString();
};

#endif // JSON_PARSER_H

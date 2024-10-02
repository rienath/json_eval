#include "json_parser.h"
#include <cctype>
#include <stdexcept>
#include <string>

constexpr size_t falseTokenLength = 5;
constexpr size_t trueTokenLength = 4;
constexpr size_t nullTokenLength = 4;

JSONParser::JSONParser(const std::string &input) : input(input), pos(0) {}

void JSONParser::skipWhitespace() {
    while (pos < input.size() && std::isspace(static_cast<unsigned char>(input[pos])) != 0) {
        ++pos;
    }
}

char JSONParser::peek() const {
    return pos < input.size() ? input[pos] : '\0';
}

char JSONParser::get() {
    return pos < input.size() ? input[pos++] : '\0';
}

bool JSONParser::match(char expected) {
    if (peek() == expected) {
        ++pos;
        return true;
    }
    return false;
}

JSONValue JSONParser::parse() {
    skipWhitespace();
    JSONValue value = parseValue();
    skipWhitespace();
    if (pos != input.size()) {
        throw std::runtime_error("Extra characters after parsing JSON value");
    }
    return value;
}

JSONValue JSONParser::parseValue() {
    skipWhitespace();
    char chr = peek();
    if (chr == '"') {
        return parseString();
    }
    if (chr == '{') {
        return parseObject();
    }
    if (chr == '[') {
        return parseArray();
    }
    if (chr == 't') {
        return parseTrue();
    }
    if (chr == 'f') {
        return parseFalse();
    }
    if (chr == 'n') {
        return parseNull();
    }
    if (chr == '-' || std::isdigit(static_cast<unsigned char>(chr)) != 0) {
        return parseNumber();
    }
    throw std::runtime_error(std::string("Unexpected character: ") + chr);
}

JSONValue JSONParser::parseObject() {
    JSONObject obj;
    match('{');
    skipWhitespace();
    if (match('}')) {
        return obj;
    }
    while (true) {
        skipWhitespace();
        if (peek() != '"') {
            throw std::runtime_error("Expected string key in object");
        }
        std::string key = parseRawString();
        skipWhitespace();
        if (!match(':')) {
            throw std::runtime_error("Expected ':' after key in object");
        }
        skipWhitespace();
        JSONValue value = parseValue();
        obj[key] = value;
        skipWhitespace();
        if (match('}')) {
            break;
        }
        if (!match(',')) {
            throw std::runtime_error("Expected ',' or '}' in object");
        }
    }
    return obj;
}

JSONValue JSONParser::parseArray() {
    JSONArray arr;
    match('[');
    skipWhitespace();
    if (match(']')) {
        return arr;
    }
    while (true) {
        skipWhitespace();
        arr.push_back(parseValue());
        skipWhitespace();
        if (match(']')) {
            break;
        }
        if (!match(',')) {
            throw std::runtime_error("Expected ',' or ']' in array");
        }
    }
    return arr;
}

std::string JSONParser::parseRawString() {
    std::string result;
    match('"');
    while (true) {
        char chr = get();
        if (chr == '"') {
            break;
        }
        if (chr == '\\') {
            chr = get();
            switch (chr) {
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
                    throw std::runtime_error(std::string("Invalid escape character: \\") + chr);
            }
        } else if (chr == '\0') {
            throw std::runtime_error("Unterminated string");
        } else {
            result += chr;
        }
    }
    return result;
}

JSONValue JSONParser::parseString() {
    return parseRawString();
}

JSONValue JSONParser::parseNumber() {
    size_t start = pos;
    if (match('-')) {}
    while (std::isdigit(static_cast<unsigned char>(peek())) != 0) {
        get();
    }
    if (match('.')) {
        while (std::isdigit(static_cast<unsigned char>(peek())) != 0) {
            get();
        }
    }
    if (peek() == 'e' || peek() == 'E') {
        get();
        if (peek() == '+' || peek() == '-') {
            get();
        }
        while (std::isdigit(static_cast<unsigned char>(peek())) != 0) {
            get();
        }
    }
    double number = std::stod(input.substr(start, pos - start));
    return number;
}

JSONValue JSONParser::parseTrue() {
    if (input.substr(pos, trueTokenLength) == "true") {
        pos += trueTokenLength;
        return true;
    }
    throw std::runtime_error("Invalid value, expected 'true'");
}

JSONValue JSONParser::parseFalse() {
    if (input.substr(pos, falseTokenLength) == "false") {
        pos += falseTokenLength;
        return false;
    }
    throw std::runtime_error("Invalid value, expected 'false'");
}

JSONValue JSONParser::parseNull() {
    if (input.substr(pos, nullTokenLength) == "null") {
        pos += nullTokenLength;
        return nullptr;
    }
    throw std::runtime_error("Invalid value, expected 'null'");
}

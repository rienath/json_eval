#include "json_parser.h"
#include <cctype>
#include <stdexcept>

JSONParser::JSONParser(const std::string &input) : input(input), pos(0) {}

void JSONParser::skipWhitespace() {
    while (pos < input.size() && std::isspace(static_cast<unsigned char>(input[pos]))) {
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
    char ch = peek();
    if (ch == '"') {
        return parseString();
    } else if (ch == '{') {
        return parseObject();
    } else if (ch == '[') {
        return parseArray();
    } else if (ch == 't') {
        return parseTrue();
    } else if (ch == 'f') {
        return parseFalse();
    } else if (ch == 'n') {
        return parseNull();
    } else if (ch == '-' || std::isdigit(static_cast<unsigned char>(ch))) {
        return parseNumber();
    } else {
        throw std::runtime_error(std::string("Unexpected character: ") + ch);
    }
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
                    throw std::runtime_error(std::string("Invalid escape character: \\") + ch);
            }
        } else if (ch == '\0') {
            throw std::runtime_error("Unterminated string");
        } else {
            result += ch;
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
    while (std::isdigit(static_cast<unsigned char>(peek()))) {
        get();
    }
    if (match('.')) {
        while (std::isdigit(static_cast<unsigned char>(peek()))) {
            get();
        }
    }
    if (peek() == 'e' || peek() == 'E') {
        get();
        if (peek() == '+' || peek() == '-') {
            get();
        }
        while (std::isdigit(static_cast<unsigned char>(peek()))) {
            get();
        }
    }
    double number = std::stod(input.substr(start, pos - start));
    return number;
}

JSONValue JSONParser::parseTrue() {
    if (input.substr(pos, 4) == "true") {
        pos += 4;
        return true;
    }
    throw std::runtime_error("Invalid value, expected 'true'");
}

JSONValue JSONParser::parseFalse() {
    if (input.substr(pos, 5) == "false") {
        pos += 5;
        return false;
    }
    throw std::runtime_error("Invalid value, expected 'false'");
}

JSONValue JSONParser::parseNull() {
    if (input.substr(pos, 4) == "null") {
        pos += 4;
        return nullptr;
    }
    throw std::runtime_error("Invalid value, expected 'null'");
}

#include "json_parser.h"
#include "gtest/gtest.h"

// clang-format off
TEST(JSONParserTest, ParseNull) {
    JSONParser parser("null");
    JSONValue value = parser.parse();
    EXPECT_TRUE(value.isNull());
}

TEST(JSONParserTest, ParseBoolean) {
    JSONParser parser("true");
    JSONValue value = parser.parse();
    EXPECT_TRUE(value.isBool());
    EXPECT_TRUE(value.asBool());

    JSONParser parser2("false");
    value = parser2.parse();
    EXPECT_TRUE(value.isBool());
    EXPECT_FALSE(value.asBool());
}

TEST(JSONParserTest, ParseNumber) {
    JSONParser parser("123");
    JSONValue value = parser.parse();
    EXPECT_TRUE(value.isNumber());
    EXPECT_EQ(value.asNumber(), 123);

    JSONParser parser2("-456.78");
    value = parser2.parse();
    EXPECT_TRUE(value.isNumber());
    EXPECT_EQ(value.asNumber(), -456.78);
}

TEST(JSONParserTest, ParseString) {
    JSONParser parser("\"hello\"");
    JSONValue value = parser.parse();
    EXPECT_TRUE(value.isString());
    EXPECT_EQ(value.asString(), "hello");
}

TEST(JSONParserTest, ParseArray) {
    JSONParser parser("[1, 2, 3]");
    JSONValue value = parser.parse();
    EXPECT_TRUE(value.isArray());
    EXPECT_EQ(value.asArray().size(), 3);
    EXPECT_EQ(value.asArray()[0].asNumber(), 1);
    EXPECT_EQ(value.asArray()[1].asNumber(), 2);
    EXPECT_EQ(value.asArray()[2].asNumber(), 3);
}

TEST(JSONParserTest, ParseObject) {
    JSONParser parser("{\"a\": 1, \"b\": \"test\"}");
    JSONValue value = parser.parse();
    EXPECT_TRUE(value.isObject());
    const auto &obj = value.asObject();
    EXPECT_EQ(obj.size(), 2);
    EXPECT_EQ(obj.at("a").asNumber(), 1);
    EXPECT_EQ(obj.at("b").asString(), "test");
}

TEST(JSONParserTest, ParseNestedStructure) {
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
    JSONValue value = parser.parse();
    EXPECT_TRUE(value.isObject());
    const auto &a = value.asObject().at("a");
    EXPECT_TRUE(a.isObject());
    const auto &b = a.asObject().at("b");
    EXPECT_TRUE(b.isArray());
    EXPECT_EQ(b.asArray().size(), 4);
}

TEST(JSONParserTest, InvalidJSON) {
    JSONParser parser("{invalid_json}");
    EXPECT_THROW(parser.parse(), std::runtime_error);
}

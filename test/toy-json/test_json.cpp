#include <gtest/gtest.h>
#include <toy-json/toy-json.h>
TEST(Value, PARSE_LITERAL)
{
    using namespace TOYJSON;
    Value v;
    EXPECT_EQ(PARSE_OK, v.parse("true")) << "true parse failed";
    EXPECT_EQ(JSON_TRUE, v.getType()) << "true parse error";
    EXPECT_EQ(PARSE_OK, v.parse("  true")) << "true parse failed";
    EXPECT_EQ(JSON_TRUE, v.getType()) << "true parse error";
    EXPECT_EQ(PARSE_OK, v.parse("false")) << "false parse failed";
    EXPECT_EQ(JSON_FALSE, v.getType()) << "false parse error";
    EXPECT_EQ(PARSE_OK, v.parse("null")) << "null parse failed";
    EXPECT_EQ(JSON_NULL, v.getType()) << "null parse error";
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("nul")) << "nul expect error";
    EXPECT_EQ(PARSE_EXPECT_SIGINAL_VALUE, v.parse("  null false"));
}

TEST(Value, PARSE_NUMBER)
{
    using namespace TOYJSON;
    Value v;
    EXPECT_EQ(PARSE_OK, v.parse("0"));
    EXPECT_DOUBLE_EQ(0.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-0"));
    EXPECT_DOUBLE_EQ(0.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-0.0"));
    EXPECT_DOUBLE_EQ(0.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1"));
    EXPECT_DOUBLE_EQ(1.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1"));
    EXPECT_DOUBLE_EQ(-1.0, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1.5"));
    EXPECT_DOUBLE_EQ(1.5, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1.5"));
    EXPECT_DOUBLE_EQ(-1.5, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("3.1416"));
    EXPECT_DOUBLE_EQ(3.1416, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1E10"));
    EXPECT_DOUBLE_EQ(1E10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1e10"));
    EXPECT_DOUBLE_EQ(1E10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1E+10"));
    EXPECT_DOUBLE_EQ(1E10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1E-10"));
    EXPECT_DOUBLE_EQ(1E-10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1E-10"));
    EXPECT_DOUBLE_EQ(-1E-10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1e-10"));
    EXPECT_DOUBLE_EQ(-1E-10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1E+10"));
    EXPECT_DOUBLE_EQ(-1E10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("-1E-10"));
    EXPECT_DOUBLE_EQ(-1E-10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1.234E+10"));
    EXPECT_DOUBLE_EQ(1.234E+10, v.getNumber());
    EXPECT_EQ(PARSE_OK, v.parse("1.234E-10"));
    EXPECT_DOUBLE_EQ(1.234E-10, v.getNumber());
}

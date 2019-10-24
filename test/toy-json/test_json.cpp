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

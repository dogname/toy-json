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
    /* 特殊值 */
    EXPECT_EQ(PARSE_OK, v.parse("1.0000000000000002"));        // 1 + 2^{-52}
    EXPECT_DOUBLE_EQ(1.0000000000000002, v.getNumber());       // 0x 3f f0 00 00 00 00 00 01
    EXPECT_EQ(PARSE_OK, v.parse("4.9406564584124654e-324"));   // 2^{-1022-52}
    EXPECT_DOUBLE_EQ(4.9406564584124654e-324, v.getNumber());  // 0x 00 00 00 00 00 00 00 01
    EXPECT_EQ(PARSE_OK, v.parse("2.2250738585072009e-308"));   // 2^{-1022}-2^{-1074}
    EXPECT_DOUBLE_EQ(2.2250738585072009e-308, v.getNumber());  // 0x 00 0f ff ff ff ff ff ff
    EXPECT_EQ(PARSE_OK, v.parse("1.7976931348623157e+308"));   // 2^{1024}-2^{971}
    EXPECT_DOUBLE_EQ(1.7976931348623157e+308, v.getNumber());  // 0x 7f ef ff ff ff ff ff ff
}

TEST(Value, PARSE_INVILID)
{
    using namespace TOYJSON;
    Value v;
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("+5.0"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("00123"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("?"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("/"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("trues"));
    EXPECT_EQ(PARSE_INVALID_VALUE, v.parse("fales"));
}

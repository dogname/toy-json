#include <string>

namespace TOYJSON {
enum data_type { JSON_NULL, JSON_TRUE, JSON_FALSE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT };
enum error_code { PARSE_OK = 0, PARSE_INVALID_VALUE, PARSE_NEED_VALUE, PARSE_EXPECT_SIGINAL_VALUE };
struct json_context {
    const char* json;
};
class Value {
private:
    data_type type;

public:
    Value();
    ~Value() {}

    /**
     *  @brief 返回JSON_VALUE 的数据类型
     *  @return return type
     */
    data_type getType() const;

    /**
     *  @brief 解析 JSON_VALUE
     *  @param 表示 JSON_VALUE 字符串
     *  @return 返回函数执行状况
     */
    error_code parse(const char* json);

private:
    void parse_whitespace(json_context& c);
    error_code parse_literal(json_context& c, const char* expect, data_type _type);
    error_code parse_value(json_context& c);
    error_code parse_number(json_context& c);
};

}  // namespace TOYJSON

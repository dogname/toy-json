#include <string>

namespace TOYJSON {
enum data_type { JSON_NULL, JSON_TRUE, JSON_FALSE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT };
enum error_code {
    PARSE_OK = 0,
    PARSE_INVALID_VALUE,
    PARSE_NEED_VALUE,
    PARSE_EXPECT_SIGINAL_VALUE,
    PARSE_STRING_MISS_QUOTATIONMARK,
    PARSE_STRING_INVALID_CHAR,
    PARSE_STRING_INVALID_ESCAPE,
    PARSE_STRING_INVALID_UNICODE_HEX,
    PARSE_STRING_INVALID_UNICODE_SURROGATE
};

struct json_context {
    const char* json;
    std::string buf;
};

class Value {
private:
    union {
        double num;
        struct {
            char* str;
            size_t length;
        };
    };
    data_type type;

public:
    Value();
    ~Value()
    {
        if (type == JSON_STRING) delete str;
    }

    /**
     *  @brief 返回JSON_VALUE 的数据类型
     *  @return return type
     */
    data_type getType() const;

    /**
     *  @brief 返回 JSON_NUMBER 的值
     *
     *  只有 data_type 是 JSON_NUMBER 时, 才能正常返回
     *
     *  @return double
     */
    double getNumber() const;

    /**
     *  @brief 解析 JSON_VALUE
     *  @param 表示 JSON_VALUE 字符串
     *  @return 返回函数执行状况
     */
    error_code parse(const char* json);

    /**
     *  @brief 返回 JSON_STRING 的值
     *
     *  只有 data_type 是 JSON_STRING 时, 才能正常返回
     *
     *  @return char*
     */
    const char* getString() const;
    size_t getStrLength() const;

    void setString(const char* _str, size_t _length);
    void setNumber(double n);
    void setBoolen(bool _BOOL);
    void setNull();

private:
    void parseWhitespace(json_context& c);
    error_code parseLiteral(json_context& c, const char* expect, data_type _type);
    error_code parseValue(json_context& c);
    error_code parseNumber(json_context& c);
    error_code parseString(json_context& c);
    const char* parseUnicodeHex(const char* p, unsigned& u);
    void encodeUTF8(json_context& c, unsigned u);
    void stringFree();
};

}  // namespace TOYJSON

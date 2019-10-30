#include <map>
#include <string>
#include <vector>

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
    PARSE_STRING_INVALID_UNICODE_SURROGATE,
    PARSE_ARRAY_MISS_COMMA_OR_SQUARE_BRACKET
};

struct json_context {
    const char* json;
    std::string buf;
};

struct arrList;
class Value {
private:
    union {
        double num;
        struct {
            char* str;
            size_t length;
        };
        struct {
            arrList* arr;
            arrList* tail;
        };
    };
    std::map<const char*, Value> object; /* 为了方便使用std::map, 不能有重复元素 */
    data_type type;

public:
    Value();
    ~Value()
    {
        valueFree();
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

    arrList* getArray(size_t index) const;
    arrList* getArrEnd() const;

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
    error_code parseStringX(json_context& c, const char** s, size_t& l);
    error_code parseArray(json_context& c);
    const char* parseUnicodeHex(const char* p, unsigned& u);
    void encodeUTF8(json_context& c, unsigned u);
    void valueFree();
};

struct arrList {
    Value v;
    arrList* next;
    arrList() : next(nullptr) {}
};

}  // namespace TOYJSON

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <toy-json/toy-json.h>

using namespace TOYJSON;

Value::Value()
{
    type = JSON_NULL;
}

inline bool is1to9(char ch)
{
    return ch >= '1' && ch <= '9';
}

inline bool isDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

error_code Value::parse(const char* json)
{
    json_context c;
    error_code ret;
    c.json = json;
    parseWhitespace(c);
    ret = parseValue(c);
    if (ret == PARSE_OK)
    {
        parseWhitespace(c);
        if (*c.json != '\0') return PARSE_EXPECT_SIGINAL_VALUE;
    }
    return ret;
}

data_type Value::getType() const
{
    return type;
}

error_code Value::parseValue(json_context& c)
{
    switch (c.json[0])
    {
    case 't': return parseLiteral(c, "true", JSON_TRUE);
    case 'f': return parseLiteral(c, "false", JSON_FALSE);
    case 'n': return parseLiteral(c, "null", JSON_NULL);
    case '\0': return PARSE_NEED_VALUE;
    case '\"': return parseString(c);
    default: return parseNumber(c);
    }
}

void Value::parseWhitespace(json_context& c)
{
    while (isblank(*c.json)) ++c.json;
}

error_code Value::parseLiteral(json_context& c, const char* expect, data_type _type)
{
    int i;
    for (i = 0; expect[i]; ++i)
    {
        if (c.json[i] != expect[i]) return PARSE_INVALID_VALUE;
    }
    c.json += i;
    type = _type;
    return PARSE_OK;
}

double Value::getNumber() const
{
    assert(type == JSON_NUMBER);
    return num;
}

error_code Value::parseNumber(json_context& c)
{
    // TODO: naive strtod
    /* json 数字不以 + 号开头 */
    const char* ptr = c.json;
    if (*ptr == '-') ++ptr;
    if (*ptr == '0')
    {
        ++ptr;
    }
    else
    {
        if (!is1to9(*ptr)) return PARSE_INVALID_VALUE;
        while (isDigit(*ptr)) ++ptr;
    }
    if (*ptr == '.')
    {
        ++ptr;
        if (!isDigit(*ptr)) return PARSE_INVALID_VALUE;
        ++ptr;
        while (isDigit(*ptr)) ++ptr;
    }
    if (*ptr == 'e' || *ptr == 'E')
    {
        ++ptr;
        if (*ptr == '+' || *ptr == '-') ++ptr;
        if (!isDigit(*ptr)) return PARSE_INVALID_VALUE;
        while (isDigit(*ptr)) ++ptr;
    }
    num    = strtod(c.json, nullptr);
    c.json = ptr;
    type   = JSON_NUMBER;
    return PARSE_OK;
}

const char* Value::getString() const
{
    assert(type == JSON_STRING);
    return str;
}

size_t Value::getStrLength() const
{
    assert(type == JSON_STRING);
    return length;
}

void Value::stringFree()
{
    if (type == JSON_STRING)
    {
        delete str;
    }
}

void Value::setNull()
{
    stringFree();
    type = JSON_NULL;
}

void Value::setBoolen(bool _BOOL)
{
    stringFree();
    type = _BOOL ? JSON_TRUE : JSON_FALSE;
}

void Value::setNumber(double n)
{
    stringFree();
    type = JSON_NUMBER;
    num  = n;
}

void Value::setString(const char* _str, size_t _length)
{
    stringFree();
    str    = new char[length + 1];
    length = _length;
    memcpy(str, _str, _length);
    str[length] = '\0';
}

error_code Value::parseString(json_context& c)
{
    const char* ptr = c.json + 1;
    while (true)
    {
        char ch = *(ptr++);
        switch (ch)
        {
        case '\"':
            length = c.buf.length();
            str    = new char[length + 1];
            c.buf.copy(str, length);
            str[length] = '\0';
            c.json      = ptr;
            type        = JSON_STRING;
            return PARSE_OK;
        case '\\':
            switch (*(ptr++))
            {
            case '\"': c.buf.push_back('\"'); break;
            case '\\': c.buf.push_back('\\'); break;
            case '/': c.buf.push_back('/'); break;
            case 'b': c.buf.push_back('\b'); break;
            case 'f': c.buf.push_back('\f'); break;
            case 'n': c.buf.push_back('\n'); break;
            case 'r': c.buf.push_back('\r'); break;
            case 't': c.buf.push_back('\t'); break;
            // TODO: \uXXXX
            default: c.json = ptr; return PARSE_STRING_INVALID_ESCAPE;
            }
            break;
        case '\0': return PARSE_STRING_MISS_QUOTATIONMARK;
        default: c.buf.push_back(ch);
        }
    }
}

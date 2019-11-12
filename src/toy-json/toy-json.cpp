#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <toy-json/toy-json.h>

using namespace TOYJSON;

Value::Value()
{
    num  = 0;
    type = JSON_NULL;
}

Value::Value(const Value& v)
{
    type = v.type;
    switch (type)
    {
    case JSON_NUMBER: num = v.num; break;
    case JSON_STRING: {
        length = v.length;
        str    = new char[length + 1];
        memcpy(str, v.str, length);
        str[length] = '\0';
        break;
    }
    case JSON_ARRAY: {
        const arrList* pt = v.arr;
        while (pt)
        {
            arrList* node = new arrList;
            node->v       = pt->v;
            if (!arr)
                arr = tail = node;
            else
            {
                tail->next = node;
                tail       = node;
            }
            pt = pt->next;
        }
        break;
    }
    case JSON_OBJECT: object = v.object; break;
    default: break;
    }
}

Value& Value::operator=(const Value& v)
{
    if (this != &v)
    {
        valueFree();
        type = v.type;
        switch (type)
        {
        case JSON_NUMBER: num = v.num; break;
        case JSON_STRING: {
            length = v.length;
            str    = new char[length + 1];
            memcpy(str, v.str, length);
            str[length] = '\0';
            break;
        }
        case JSON_ARRAY: {
            const arrList* pt = v.arr;
            while (pt)
            {
                arrList* node = new arrList;
                node->v       = pt->v;
                if (!arr)
                    arr = tail = node;
                else
                {
                    tail->next = node;
                    tail       = node;
                }
                pt = pt->next;
            }
            break;
        }
        case JSON_OBJECT: object = v.object; break;
        default: break;
        }
    }
    return *this;
}

Value::~Value()
{
    if (type == JSON_STRING)
    {
        delete[] str;
    }
    else if (type == JSON_ARRAY)
    {
        while (arr)
        {

            arrList* pt = arr->next;
            delete arr;
            arr = pt;
        }
    }
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
    parseWhitespace(c);
    switch (c.json[0])
    {
    case 't': return parseLiteral(c, "true", JSON_TRUE);
    case 'f': return parseLiteral(c, "false", JSON_FALSE);
    case 'n': return parseLiteral(c, "null", JSON_NULL);
    case '\0': return PARSE_NEED_VALUE;
    case '\"': return parseString(c);
    case '[': return parseArray(c);
    case '{': return parseObject(c);
    default: return parseNumber(c);
    }
}

void Value::parseWhitespace(json_context& c)
{
    while (isspace(*c.json)) ++c.json;
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

void Value::valueFree()
{
    if (type == JSON_STRING)
    {
        delete[] str;
    }
    else if (type == JSON_ARRAY)
    {
        while (arr)
        {

            arrList* pt = arr->next;
            delete arr;
            arr = pt;
        }
    }
    else if (type == JSON_OBJECT)
    {
        object.clear();
    }
}

void Value::setNull()
{
    valueFree();
    type = JSON_NULL;
}

void Value::setBoolen(bool _BOOL)
{
    valueFree();
    type = _BOOL ? JSON_TRUE : JSON_FALSE;
}

void Value::setNumber(double n)
{
    valueFree();
    type = JSON_NUMBER;
    num  = n;
}

void Value::setString(const char* _str, size_t _length)
{
    valueFree();
    length = _length;
    str    = new char[length + 1];
    type   = JSON_STRING;
    memcpy(str, _str, _length);
    str[length] = '\0';
}

error_code Value::parseStringX(json_context& c, const char** s, size_t& l)
{
    const char* ptr = c.json + 1;
    while (true)
    {
        char ch = *(ptr++);
        switch (ch)
        {
        case '\"':
            l      = c.buf.length();
            *s     = c.buf.data();
            c.json = ptr;
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
            case 'u':
                unsigned u, uL;
                if ((ptr = parseUnicodeHex(ptr, u)) == nullptr) return PARSE_STRING_INVALID_UNICODE_HEX;
                if (u >= 0xD800 && u <= 0xDB00) /* surrogate */
                {
                    if (*(ptr++) != '\\') return PARSE_STRING_INVALID_UNICODE_SURROGATE;
                    if (*(ptr++) != 'u') return PARSE_STRING_INVALID_UNICODE_SURROGATE;
                    if ((ptr = parseUnicodeHex(ptr, uL)) == nullptr) return PARSE_STRING_INVALID_UNICODE_HEX;
                    u = 0x10000 + ((u - 0xD800) << 10 | (uL - 0xDC00));
                }
                encodeUTF8(c, u);
                break;
            default: c.json = ptr; return PARSE_STRING_INVALID_ESCAPE;
            }
            break;
        case '\0': return PARSE_STRING_MISS_QUOTATIONMARK;
        default: c.buf.push_back(ch);
        }
    }
}

error_code Value::parseString(json_context& c)
{
    const char* s;
    error_code ret;
    size_t l;
    if ((ret = parseStringX(c, &s, l)) == PARSE_OK) setString(s, l);
    c.buf.clear();
    return ret;
}

const char* Value::parseUnicodeHex(const char* p, unsigned& u)
{
    u = 0;
    for (int i = 0; i < 4; ++i)
    {
        u <<= 4;
        char ch = *(p++);
        if (ch >= '0' && ch <= '9')
            u |= ch - '0';
        else if (ch >= 'A' && ch <= 'F')
            u |= ch - 'A' + 10;
        else if (ch >= 'a' && ch <= 'f')
            u |= ch - 'a' + 10;
        else
            return nullptr;
    }
    return p;
}

void Value::encodeUTF8(json_context& c, unsigned u)
{
    if (u <= 0x007F)
        c.buf.push_back(u & 0xFF);
    else if (u <= 0x07FF)
    {
        c.buf.push_back(0xC0 | ((u >> 6) & 0x1F));
        c.buf.push_back(0x80 | (u & 0x3F));
    }
    else if (u <= 0xFFFF)
    {
        c.buf.push_back(0xE0 | ((u >> 12) & 0x0F));
        c.buf.push_back(0x80 | ((u >> 6) & 0x3F));
        c.buf.push_back(0x80 | (u & 0x3F));
    }
    else
    {
        assert(u <= 0x10FFFF);
        c.buf.push_back(0xF0 | ((u >> 18) & 0x07));
        c.buf.push_back(0x80 | ((u >> 12) & 0x3F));
        c.buf.push_back(0x80 | ((u >> 6) & 0x3F));
        c.buf.push_back(0x80 | (u & 0x3F));
    }
}

arrList* Value::getArray(size_t index) const
{
    arrList* ret = arr;
    while (index > 0)
    {
        ret = ret->next;
    }
    return ret;
}

arrList* Value::getArrEnd() const
{
    return tail;
}

error_code Value::parseArray(json_context& c)
{
    c.json++;
    error_code ret;
    parseWhitespace(c);
    if (*c.json == ']')
    {
        c.json++;
        arr = tail = nullptr;
        type       = JSON_ARRAY;
        return PARSE_OK;
    }
    while (true)
    {
        parseWhitespace(c);
        arrList* node = new arrList;
        if (arr == nullptr)
            arr = tail = node;
        else
        {
            tail->next = node;
            tail       = node;
        }
        if ((ret = node->v.parseValue(c)) != PARSE_OK) return ret;
        parseWhitespace(c);
        if (*(c.json) == ',')
            c.json++;
        else if (*(c.json) == ']')
        {
            c.json++;
            type = JSON_ARRAY;
            return PARSE_OK;
        }
        else
        {
            return PARSE_ARRAY_MISS_COMMA_OR_SQUARE_BRACKET;
        }
    }
}

error_code Value::parseObject(json_context& c)
{
    error_code ret;
    c.json++;
    parseWhitespace(c);
    if (*c.json == '}')
    {
        c.json++;
        type = JSON_OBJECT;
        return PARSE_OK;
    }
    while (true)
    {
        const char* str;
        Value v;
        size_t l;
        if (*c.json != '\"') return PARSE_OBJECT_MISS_KEY;
        if ((ret = parseStringX(c, &str, l)) != PARSE_OK) return ret;
        std::string key(str, l);
        c.buf.clear();
        parseWhitespace(c);
        if (*c.json != ':') return PARSE_OBJECT_MISS_COLON;
        c.json++;
        parseWhitespace(c);
        if ((ret = v.parseValue(c)) != PARSE_OK) return ret;
        parseWhitespace(c);
        if (*c.json == ',')
        {
            c.json++;
            parseWhitespace(c);
            object[key] = v;
            c.buf.clear();
        }
        else if (*c.json == '}')
        {
            type = JSON_OBJECT;
            c.json++;
            object[key] = v;
            return PARSE_OK;
        }
        else
        {
            return PARSE_OBJECT_MISS_COMMA_OR_CURLY_BRACKET;
        }
    }
}

const Value& Value::getObject(const std::string& key)
{
    assert(object.count(key));
    return object[key];
}

void Value::stringifyValue(std::string& c) const
{
    switch (type)
    {
    case JSON_NULL: c += "null"; break;
    case JSON_TRUE: c += "true"; break;
    case JSON_FALSE: c += "false"; break;
    case JSON_NUMBER: {
        char buf[32];
        snprintf(buf, 32, "%.17g", num);
        c += buf;
        break;
    }
    case JSON_STRING: stringifyString(c, str, length); break;
    case JSON_ARRAY: {
        c += "[";
        arrList* p = arr;
        while (p)
        {
            p->v.stringifyValue(c);
            if (p != tail) c += ",";
            p = p->next;
        }
        c += "]";
        break;
    }
    case JSON_OBJECT: {
        c += "{";
        std::map<std::string, Value>::const_iterator iter;
        for (iter = object.begin(); iter != object.end(); iter++)
        {
            if (iter != object.begin()) c += ",";
            stringifyString(c, iter->first.c_str(), iter->first.length());
            c += ":";
            iter->second.stringifyValue(c);
        }
        c += "}";
        break;
    }
    }
}

void Value::stringifyString(std::string& c, const char* s, size_t l) const
{
    static const char hex_digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    c += "\"";
    for (size_t i = 0; i < l; ++i)
    {
        switch (s[i])
        {
        case '\"': c += "\\\""; break;
        case '\\': c += "\\\\"; break;
        case '\b': c += "\\b"; break;
        case '\f': c += "\\f"; break;
        case '\n': c += "\\n"; break;
        case '\r': c += "\\r"; break;
        case '\t': c += "\\t"; break;
        default:
            if (s[i] < 0x20)
            {
                char buf[7];
                snprintf(buf, 7, "\\u%04X", s[i]);
                c += buf;
            }
            else
            {
                c += s[i];
            }
        }
    }
    c += "\"";
}

void Value::save(const char* filename) const
{
    std::string c;
    stringifyValue(c);
    std::ofstream out(filename);
    if (!out) std::abort();
    out << c;
    out.close();
}

void Value::readFromFile(const char* filename)
{
    std::ifstream in(filename);
    if (in)
    {
        char ch;
        std::string c;
        while (in.get(ch)) c.push_back(ch);
        parse(c.c_str());
    }
    else
    {
        std::cout << "读取失败";
    }
}

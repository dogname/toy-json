#include <toy-json/toy-json.h>
using namespace TOYJSON;
Value::Value()
{
    type = JSON_NULL;
}

error_code Value::parse(const char* json)
{
    json_context c;
    error_code ret;
    c.json = json;
    parse_whitespace(c);
    ret = parse_value(c);
    if (ret == PARSE_OK)
    {
        parse_whitespace(c);
        if (*c.json != '\0') return PARSE_EXPECT_SIGINAL_VALUE;
    }
    return ret;
}

data_type Value::getType() const
{
    return type;
}

error_code Value::parse_value(json_context& c)
{
    switch (c.json[0])
    {
    case 't':
        return parse_literal(c, "true", JSON_TRUE);
    case 'f':
        return parse_literal(c, "false", JSON_FALSE);
    case 'n':
        return parse_literal(c, "null", JSON_NULL);
    case '\0':
        return PARSE_NEED_VALUE;
    default:
        return PARSE_INVALID_VALUE;
    }
}

void Value::parse_whitespace(json_context& c)
{
    while (isblank(*c.json)) ++c.json;
}

error_code Value::parse_literal(json_context& c, const char* expect, data_type _type)
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
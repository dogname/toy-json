#include <toy-json/toy-json.h>
using namespace TOYJSON;
Value::Value()
{
    type = JSON_NULL;
}

error_code Value::parse(const char* json)
{
    return PARSE_OK;
}

data_type Value::getType() const
{
    return type;
}

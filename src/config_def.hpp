#ifndef CONFIG_DEF_HPP_
#define CONFIG_DEF_HPP_

#include "./something_rgba.hpp"

enum class Config_Type: size_t {
    Float = 0,
    Color,
    Int,
    String,
    Bool,
    Count
};

struct Config_Value_Def {
    aids::String_View name;
    Config_Type type;
};

union Config_Value {
    float as_float;
    int as_int;
    RGBA as_color;
    bool as_bool;
    aids::String_View as_string;
};

static_assert(
    static_cast<size_t>(Config_Type::Count) == 5,
    "Config_Type definition has changed. "
    "Please update Config_Value definition accordingly");

#endif // CONFIG_DEF_HPP_

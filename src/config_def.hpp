#ifndef CONFIG_DEF_HPP_
#define CONFIG_DEF_HPP_

#include "./something_color.hpp"

enum class Config_Type: size_t {
    Float = 0,
    // TODO: Config_Type::Color should be probably renamed to Config_Type::RGBA
    // Because we introduced HSL colors, so for different types of color representations
    // it would make sense to have separate types
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

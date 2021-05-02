#ifndef CONFIG_DEF_HPP_
#define CONFIG_DEF_HPP_

enum class Config_Type: size_t {
    Float = 0,
    Count
};

struct Config_Def {
    aids::String_View name;
    Config_Type type;
};

union Config_Value {
    int as_int;
    float as_float;
    aids::String_View as_rgba;
};

#endif // CONFIG_DEF_HPP_

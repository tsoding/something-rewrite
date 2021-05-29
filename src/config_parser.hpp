#ifndef CONFIG_PARSER_HPP_
#define CONFIG_PARSER_HPP_

#include "./config_def.hpp"

const char *config_type_name(Config_Type type);
bool config_type_by_name(aids::String_View name, Config_Type *type);
bool parse_config_value(aids::String_View value_sv, Config_Type type, Config_Value *result);

struct Parser {
    enum Status {
        SUCCESS = 0,
        FINISHED,
        INVALID_TYPE,
        INVALID_VALUE,
    };

    struct Invalid_Type {
        Status status;
        aids::String_View name;
    };

    struct Invalid_Value {
        Status status;
        Config_Type expected_type;
        aids::String_View value;
    };

    aids::String_View content;
    size_t line_number;

    Status status;
    Config_Value_Def def;
    Config_Value value;
    Invalid_Type invalid_type;
    Invalid_Value invalid_value;

    Status next();
};

#endif // CONFIG_PARSER_HPP_

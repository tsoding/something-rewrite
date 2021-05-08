#ifndef CONFIG_PARSER_HPP_
#define CONFIG_PARSER_HPP_

const char *config_type_name(Config_Type type)
{
    switch (type) {
    case Config_Type::Float:
        return "float";
    case Config_Type::Color:
        return "color";
    case Config_Type::Int:
        return "int";
    case Config_Type::Count:
    default:
        aids::unreachable("config_type_name");
    }
}

bool config_type_by_name(aids::String_View name, Config_Type *type)
{
    static_assert(
        static_cast<size_t>(Config_Type::Count) == 3,
        "Config_Type defintion was changed. "
        "Adjust the code below and the condition "
        "above to reflect the changes.");

    using namespace aids;

    if (name == "float"_sv) {
        *type = Config_Type::Float;
        return true;
    }

    if (name == "color"_sv) {
        *type = Config_Type::Color;
        return true;
    }

    if (name == "int"_sv) {
        *type = Config_Type::Int;
        return true;
    }

    return false;
}

aids::Maybe<Config_Value> parse_config_value(aids::String_View value_sv, Config_Type type)
{
    Config_Value result = {};

    switch (type) {
    case Config_Type::Float: {
        char buffer[256] = {};

        if (value_sv.count >= sizeof(buffer) - 1) {
            return {};
        }

        memcpy(buffer, value_sv.data, value_sv.count);

        char *endptr = NULL;
        result.as_float = strtof(buffer, &endptr);

        if (endptr == buffer) {
            return {};
        }
    }
    break;

    case Config_Type::Color: {
        const auto color_u32 = value_sv.from_hex<uint32_t>();
        if (!color_u32.has_value) {
            return {};
        }

        result.as_color = RGBA::from_u32(color_u32.unwrap);
    }
    break;

    case Config_Type::Int: {
        const auto value = value_sv.as_integer<int>();

        if (!value.has_value) {
            return {};
        }

        result.as_int = value.unwrap;
    }
    break;

    case Config_Type::Count:
    default:
        aids::unreachable("parse_config_value");
    }

    return {true, result};
}

template <typename Callback>
void parse_config_content(aids::String_View lines,
                          const char *file_path,
                          Callback callback)
{
    using namespace aids;

    for (size_t line_number = 1; lines.count > 0; ++line_number) {
        String_View line = lines.chop_by_delim('\n');
        line = line.chop_by_delim('#').trim();
        if (line.count > 0) {
            Config_Def def = {};

            // Name
            def.name = line.chop_by_delim(':').trim();

            // Type
            {
                const String_View type_sv = line.chop_by_delim('=').trim();
                if (!config_type_by_name(type_sv, &def.type)) {
                    panic(file_path, ":", line_number, ": `", type_sv, "` is not a valid type");
                }
            }

            // Value
            const String_View value_sv = line.trim();
            auto value = unwrap_or_panic(
                             parse_config_value(value_sv, def.type),
                             file_path, ":", line_number,
                             ": Could not parse `", value_sv, "` as ", config_type_name(def.type));

            callback(def, value, line_number);
        }
    }
}

#endif // CONFIG_PARSER_HPP_


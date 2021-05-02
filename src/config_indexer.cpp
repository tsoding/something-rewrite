#include "aids.hpp"
#include "./config_def.hpp"
#include "./config_parser.hpp"

using namespace aids;

void print1(FILE *stream, Config_Type type)
{
    switch (type) {
    case Config_Type::Float:
        print(stream, "Config_Type::Float");
        break;
    default:
        unreachable("print1(Config_Type)");
    }
}

void print1(FILE *stream, Config_Def def)
{
    print(stream, "{\"", def.name, "\"_sv, ", def.type,"}");
}

Maybe<size_t> where_var_is_defined(const Dynamic_Array<Config_Def> &defs,
                                   const Dynamic_Array<size_t> &def_lines,
                                   String_View var_name)
{
    for (size_t i = 0; i < defs.size; ++i) {
        if (defs[i].name == var_name) {
            return {true, def_lines[i]};
        }
    }
    return {};
}

void usage(const char *program, FILE *stderr)
{
    println(stderr, "Usage: ", program, " [--bake] <vars.conf>");
}

int main(int argc, char **argv)
{
    Args args = {argc, argv};
    const auto program = args.shift();

    const char *vars_conf_path = NULL;
    bool bake = false;

    while (!args.empty()) {
        const char *flag = args.shift();
        if (strcmp(flag, "--bake") == 0) {
            bake = true;
        } else {
            if (vars_conf_path != NULL) {
                usage(program, stderr);
                panic("ERROR: only one path to vars.conf can be provided");
            }

            vars_conf_path = flag;
        }
    }

    if (vars_conf_path == NULL) {
        usage(program, stderr);
        panic("ERROR: path to vars.conf was not provided");
    }

    const String_View vars_conf_content =
        unwrap_or_panic(
            read_file_as_string_view(vars_conf_path),
            "Could not read file `", vars_conf_path, "`: ", strerror(errno));
    defer(destroy(vars_conf_content));

    Dynamic_Array<Config_Def> config_defs = {};
    defer(destroy(config_defs));

    Dynamic_Array<size_t> config_def_lines = {};
    defer(destroy(config_def_lines));

    Dynamic_Array<Config_Value> config_values = {};
    defer(destroy(config_values));

    const auto action = [&](Config_Def def, Config_Value value, size_t line_number) {
        auto place = where_var_is_defined(config_defs, config_def_lines, def.name);
        if (place.has_value) {
            panic(vars_conf_path, ":", line_number, ": `", def.name, "` is already defined", Newline(),
                  vars_conf_path, ":", place.unwrap, ": it was defined first time here");
        }

        config_values.push(value);
        config_defs.push(def);
        config_def_lines.push(line_number);
    };
    parse_config_content(vars_conf_content, vars_conf_path, action);

    println(stdout, "#ifndef CONFIG_INDEX_HPP_");
    println(stdout, "#define CONFIG_INDEX_HPP_");
    println(stdout);
    if (bake) {
        for (size_t i = 0; i < config_defs.size; ++i) {
            print(stdout, "#define ", config_defs[i].name, " ");
            switch (config_defs[i].type) {
            case Config_Type::Float:
                println(stdout, config_values[i].as_float);
                break;
            default:
                unreachable("baked config_values macros");
            }
        }
        println(stdout);
    } else {
        println(stdout, "#include \"./config_def.hpp\"");
        println(stdout);
        println(stdout, "const size_t CONFIG_CAPACITY = ", config_defs.size, ";");
        println(stdout, "const Config_Def config_defs[CONFIG_CAPACITY] = {");
        for (size_t i = 0; i < config_defs.size; ++i) {
            println(stdout, "   {\"", config_defs[i].name, "\"_sv, ", config_defs[i].type,"},");
        }
        println(stdout, "};");
        println(stdout);
        println(stdout, "Config_Value config_values[CONFIG_CAPACITY] = {};");
        println(stdout);
        for (size_t i = 0; i < config_defs.size; ++i) {
            print(stdout, "#define ", config_defs[i].name);
            switch (config_defs[i].type) {
            case Config_Type::Float:
                println(stdout, " config_values[", i, "].as_float");
                break;
            default:
                unreachable("config_values macros");
            }
        }
        println(stdout);
    }
    println(stdout, "#endif // CONFIG_INDEX_HPP_");

    return 0;
}

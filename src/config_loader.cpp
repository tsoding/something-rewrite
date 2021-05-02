#include "./config_loader.hpp"
#include "./config_index.hpp"
#include "./config_parser.hpp"

static String_View config_file_content_owner = {};

Maybe<size_t> config_index_by_name(String_View name)
{
    for (size_t i = 0; i < CONFIG_CAPACITY; ++i) {
        if (config_defs[i].name == name) {
            return {true, i};
        }
    }

    return {};
}

// TODO: reload_config_from_file panics instead of providing error recoverable interface
void reload_config_from_file(const char *file_path)
{

    const auto content =
        unwrap_or_panic(
            read_file_as_string_view(file_path),
            "ERROR: could not load file `", file_path, "`: ", strerror(errno));

    if (config_file_content_owner.data != nullptr) {
        destroy(config_file_content_owner);
    }

    config_file_content_owner = content;

    const auto callback = [&](Config_Def def, Config_Value value, size_t line_number) {
        const auto index =
            unwrap_or_panic(
                config_index_by_name(def.name),
                file_path, ":", line_number, ": The application knows nothing about `", def.name, "`. Please regenerate the config index by rebuilding the application.");

        if (config_defs[index].type != def.type) {
            panic(file_path, ":", line_number, "The application expects ", def.name, " to have type ", config_type_name(config_defs[index].type), " but the configuration file defines it as ", config_type_name(def.type), ". Please regenerate the config index by rebuilding the application.");
        }

        config_values[index] = value;
    };

    parse_config_content(config_file_content_owner, file_path, callback);
}

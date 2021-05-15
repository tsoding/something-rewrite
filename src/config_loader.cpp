#include "./config_loader.hpp"
#include "./config_index.hpp"
#include "./config_parser.hpp"

static String_View config_file_content_owner = {};

Maybe<size_t> config_index_by_name(String_View name)
{
    for (size_t i = 0; i < CONFIG_CAPACITY; ++i) {
        if (config_value_defs[i].name == name) {
            return {true, i};
        }
    }

    return {};
}

// TODO(#68): reload_config_from_file panics instead of providing error recoverable interface
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

    Parser parser = {};
    parser.content = config_file_content_owner;

    while (parser.next() == Parser::SUCCESS) {
        const auto index =
            unwrap_or_panic(
                config_index_by_name(parser.def.name),
                file_path, ":", parser.line_number, ": The application knows nothing about `", parser.def.name, "`. Please regenerate the config index by rebuilding the application.");

        if (config_value_defs[index].type != parser.def.type) {
            panic(file_path, ":", parser.line_number, "The application expects ", parser.def.name, " to have type ", config_type_name(config_value_defs[index].type), " but the configuration file defines it as ", config_type_name(parser.def.type), ". Please regenerate the config index by rebuilding the application.");
        }

        config_values[index] = parser.value;
    }

    switch (parser.status) {
    case Parser::INVALID_TYPE:
        panic(file_path, ":", parser.line_number, ": `", parser.invalid_type.name, "` is not a valid type");
    case Parser::INVALID_VALUE:
        panic(file_path, ":", parser.line_number, ": could not parse `", parser.invalid_value.value, "` as `", config_type_name(parser.invalid_value.expected_type), "`");
    case Parser::SUCCESS:
        unreachable("Parser::SUCCESS");
    case Parser::FINISHED:
    {}
    }
}

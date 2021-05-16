#include "./config_loader.hpp"
#include "./config_index.hpp"
#include "./config_parser.hpp"

static String_View config_file_content_owner = {};

bool config_index_by_name(String_View name, size_t *index)
{
    for (size_t i = 0; i < CONFIG_CAPACITY; ++i) {
        if (config_value_defs[i].name == name) {
            if (index) *index = i;
            return true;
        }
    }

    return false;
}

bool reload_config_from_file(const char *file_path)
{
    const auto maybe_content = read_file_as_string_view(file_path);

    if (!maybe_content.has_value) {
        println(stderr, "ERROR: could not load file `", file_path, "`: ", strerror(errno));
        return false;
    }
    const auto content = maybe_content.unwrap;

    if (config_file_content_owner.data != nullptr) {
        destroy(config_file_content_owner);
    }

    config_file_content_owner = content;

    Parser parser = {};
    parser.content = config_file_content_owner;

    while (parser.next() == Parser::SUCCESS) {
        size_t index = 0;
        if (!config_index_by_name(parser.def.name, &index)) {
            println(stderr, file_path, ":", parser.line_number, ": The application knows nothing about `", parser.def.name, "`. Please regenerate the config index by rebuilding the application.");
            return false;
        }

        if (config_value_defs[index].type != parser.def.type) {
            println(stderr, file_path, ":", parser.line_number, "The application expects ", parser.def.name, " to have type ", config_type_name(config_value_defs[index].type), " but the configuration file defines it as ", config_type_name(parser.def.type), ". Please regenerate the config index by rebuilding the application.");
            return false;
        }

        config_values[index] = parser.value;
    }

    switch (parser.status) {
    case Parser::INVALID_TYPE:
        println(stderr, file_path, ":", parser.line_number, ": `", parser.invalid_type.name, "` is not a valid type");
        return false;
    case Parser::INVALID_VALUE:
        println(stderr, file_path, ":", parser.line_number, ": could not parse `", parser.invalid_value.value, "` as `", config_type_name(parser.invalid_value.expected_type), "`");
        return false;
    case Parser::SUCCESS:
        unreachable("Task failed successfully");
    case Parser::FINISHED:
    default:
        return true;
    }
}

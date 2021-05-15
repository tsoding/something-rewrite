#ifndef CONFIG_LOADER_HPP_
#define CONFIG_LOADER_HPP_

bool reload_config_from_file(const char *file_path);
String_View config_reload_error_message();

#endif // CONFIG_LOADER_HPP_

#ifndef SOMETHING_CONFIG_HPP_
#define SOMETHING_CONFIG_HPP_

struct Config {
    static const size_t VALUES_CAPACITY = 1024;
    static const size_t FILE_BUFFER_CAPACITY = 1000 * 1000;

    union Value {
        float as_float;
        int as_int;
        RGBA as_rgba32;
    };

    Value values[VALUES_CAPACITY];
    Fixed_Region<FILE_BUFFER_CAPACITY> config_file_buffer;

    void load_file(const char *file_path);
};

extern Config config;

#endif  // SOMETHING_CONFIG_HPP_

#ifndef SOMETHING_CONSOLE_HPP_
#define SOMETHING_CONSOLE_HPP_

const size_t CONSOLE_BUFFER_COLS = 256;
const size_t CONSOLE_BUFFER_ROWS = 1024;

struct Row {
    size_t count;
    char chars[CONSOLE_BUFFER_COLS];

    void copy_from_sv(String_View sv);
    String_View as_sv() const;
};

struct Row_Ring {
    Row rows[CONSOLE_BUFFER_ROWS];
    size_t begin;
    size_t count;

    void push_line(String_View line);
    String_View get(size_t index);
};

#endif // SOMETHING_CONSOLE_HPP_

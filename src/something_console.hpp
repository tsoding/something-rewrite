#ifndef SOMETHING_CONSOLE_HPP_
#define SOMETHING_CONSOLE_HPP_

struct Console {
    static const size_t BUFFER_ROWS = 1024;
    static const size_t BUFFER_COLS = 256;

    struct Row {
        size_t count;
        char chars[BUFFER_COLS];

        void copy_from_sv(String_View sv);
        String_View as_sv() const;
    };

    Row rows[BUFFER_ROWS];
    size_t begin;
    size_t count;

    void push_line(String_View line);
};

#endif // SOMETHING_CONSOLE_HPP_

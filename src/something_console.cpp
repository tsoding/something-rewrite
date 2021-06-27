#include "something_console.hpp"

void Console::Row::copy_from_sv(String_View sv)
{
    count = min(sv.count, BUFFER_COLS);
    memcpy(chars, sv.data, count);
}

String_View Console::Row::as_sv() const
{
    return String_View {count, chars};
}

void Console::push_line(String_View line)
{
    begin = static_cast<size_t>(
                mod(static_cast<int>(begin) - 1,
                    static_cast<int>(BUFFER_ROWS)));
    rows[begin].copy_from_sv(line);

    if (count < BUFFER_ROWS) {
        count += 1;
    }
}


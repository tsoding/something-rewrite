#include "something_console.hpp"

void Row::copy_from_sv(String_View sv)
{
    count = min(sv.count, CONSOLE_BUFFER_COLS);
    memcpy(chars, sv.data, count);
}

String_View Row::as_sv() const
{
    return String_View {count, chars};
}

void Row_Ring::push_line(String_View line)
{
    begin = static_cast<size_t>(
                mod(static_cast<int>(begin) - 1,
                    static_cast<int>(CONSOLE_BUFFER_ROWS)));
    rows[begin].copy_from_sv(line);

    if (count < CONSOLE_BUFFER_ROWS) {
        count += 1;
    }
}

String_View Row_Ring::get(size_t index)
{
    return rows[(begin + index) % CONSOLE_BUFFER_ROWS].as_sv();
}

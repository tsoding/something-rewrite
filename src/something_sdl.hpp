#ifndef SOMETHING_SDL_HPP_
#define SOMETHING_SDL_HPP_

#include "./something_geo.hpp"

template <typename T>
T *sec(T *ptr)
{
    if (ptr == NULL) {
        panic("SDL ERROR: ", SDL_GetError());
    }

    return ptr;
}

void sec(int code)
{
    if (code < 0) {
        panic("SDL ERROR", SDL_GetError());
    }
}

#endif  // SOMETHING_SDL_HPP_

#ifndef SOMETHING_COLOR_HPP_
#define SOMETHING_COLOR_HPP_

#include "./aids.hpp"

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cmath>

const size_t RGBA_COMPONENTS = 4;

struct RGBA {
    float r, g, b, a;

    RGBA() = default;

    constexpr RGBA(float x):
        RGBA(x, x, x, x)
    {}

    constexpr RGBA(float r, float g, float b, float a):
        r(r), g(g), b(b), a(a)
    {}

    static constexpr RGBA from_u32(uint32_t rgba)
    {
        return RGBA(static_cast<float>(((rgba >> (3 * 8)) & 0xFF) / 255.0f),
                    static_cast<float>(((rgba >> (2 * 8)) & 0xFF) / 255.0f),
                    static_cast<float>(((rgba >> (1 * 8)) & 0xFF) / 255.0f),
                    static_cast<float>(((rgba >> (0 * 8)) & 0xFF) / 255.0f));
    }

    static constexpr RGBA RED()
    {
        return RGBA(1.0f, 0.0f, 0.0f, 1.0f);
    }

    static constexpr RGBA GREEN()
    {
        return RGBA(0.0f, 1.0f, 0.0f, 1.0f);
    }

    static constexpr RGBA BLUE()
    {
        return RGBA(0.0f, 0.0f, 1.0f, 1.0f);
    }

    static constexpr RGBA WHITE()
    {
        return RGBA(1.0f, 1.0f, 1.0f, 1.0f);
    }

    static constexpr RGBA from_abgr32(uint32_t hex)
    {
        return RGBA(static_cast<float>((hex >> (3 * 8)) & 0xFF) / 255.0f,
                    static_cast<float>((hex >> (2 * 8)) & 0xFF) / 255.0f,
                    static_cast<float>((hex >> (1 * 8)) & 0xFF) / 255.0f,
                    static_cast<float>((hex >> (0 * 8)) & 0xFF) / 255.0f);
    }

    RGBA with_alpha(float alpha) const
    {
        return RGBA(r, g, b, alpha);
    }

    RGBA invert() const
    {
        return RGBA(1.0f - r, 1.0f - g, 1.0f - b, a);
    }
};

inline void print1(FILE *stream, RGBA rgba)
{
    aids::print(stream, "RGBA(", rgba.r, "f,", rgba.g, "f,", rgba.b, "f,", rgba.a, "f)");
}

struct HSLA {
    float h, s, l, a;

    HSLA() = default;

    HSLA(float h, float s, float l, float a):
        h(h), s(s), l(l), a(a)
    {}

    // TODO: represent colors as 3-4 dimensional vectors
    // To enable the mathematical vector operations
    RGBA to_rgba() const
    {
        RGBA rgba = {};

        // NOTE: stolen from emoteJAM

        // vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0);
        rgba.r = aids::clamp(fabsf(fmodf(0.0f + h * 6.0f, 6.0f) - 3.0f) - 1.0f, 0.0f, 1.0f);
        rgba.g = aids::clamp(fabsf(fmodf(4.0f + h * 6.0f, 6.0f) - 3.0f) - 1.0f, 0.0f, 1.0f);
        rgba.b = aids::clamp(fabsf(fmodf(2.0f + h * 6.0f, 6.0f) - 3.0f) - 1.0f, 0.0f, 1.0f);
        rgba.a = a;

        //  return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
        rgba.r = l + s * (rgba.r - 0.5f) * (1.0f - fabsf(2.0f * l - 1.0f));
        rgba.g = l + s * (rgba.g - 0.5f) * (1.0f - fabsf(2.0f * l - 1.0f));
        rgba.b = l + s * (rgba.b - 0.5f) * (1.0f - fabsf(2.0f * l - 1.0f));

        return rgba;
    }
};

inline void print1(FILE *stream, HSLA hsla)
{
    aids::print(stream, "HSLA(", hsla.h, "f,", hsla.s, "f,", hsla.l, "f,", hsla.a, "f)");
}

#endif  // SOMETHING_COLOR_HPP_

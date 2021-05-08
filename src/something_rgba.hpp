#ifndef SOMETHING_RGBA_HPP_
#define SOMETHING_RGBA_HPP_

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
};

void print1(FILE *stream, RGBA rgba)
{
    print(stream, "RGBA(", rgba.r, ",", rgba.g, ",", rgba.b, ",", rgba.a, ")");
}

#endif  // SOMETHING_RGBA_HPP_

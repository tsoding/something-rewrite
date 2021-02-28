#ifndef SOMETHING_RGBA_HPP_
#define SOMETHING_RGBA_HPP_

struct RGBA {
    float r, g, b, a;

    RGBA() = default;

    RGBA(float r, float g, float b, float a):
        r(r), g(g), b(b), a(a)
    {}

    static const RGBA RED;
    static const RGBA GREEN;
    static const RGBA BLUE;

    static RGBA from_abgr32(uint32_t hex)
    {
        return RGBA(static_cast<float>((hex >> (3 * 8)) & 0xFF) / 255.0f,
                    static_cast<float>((hex >> (2 * 8)) & 0xFF) / 255.0f,
                    static_cast<float>((hex >> (1 * 8)) & 0xFF) / 255.0f,
                    static_cast<float>((hex >> (0 * 8)) & 0xFF) / 255.0f);
    }
};

const RGBA RGBA::RED   = RGBA(1.0f, 0.0f, 0.0f, 1.0f);
const RGBA RGBA::GREEN = RGBA(0.0f, 1.0f, 0.0f, 1.0f);
const RGBA RGBA::BLUE  = RGBA(0.0f, 0.0f, 1.0f, 1.0f);

#endif  // SOMETHING_RGBA_HPP_

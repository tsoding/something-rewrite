#ifndef SOMETHING_V2_HPP_
#define SOMETHING_V2_HPP_

float squaref(float x);

template <typename T, typename U>
T lerp(T a, T b, U f)
{
    return a + (b - a) * f;
}

template <typename T>
T inv_lerp(T a, T b, T c)
{
    return (c - a) / (b - a);
}

float random01()
{
    static_assert(RAND_MAX != 0);
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

// 2D Vector //////////////////////////////

const size_t V2_COMPONENTS = 2;

template <typename T>
struct V2 {
    T x, y;

    V2() = default;

    V2(T a):
        x(a), y(a)
    {}

    V2(T x, T y):
        x(x), y(y)
    {}


    template <typename U>
    V2<U> cast_to() const
    {
        return V2<U>(static_cast<U>(x),
                     static_cast<U>(y));
    }

    template <typename U>
    V2<U> map(U (*f)(T)) const
    {
        return V2<U>(f(x), f(y));
    }
};

V2<float> polar_v2(float angle, float mag);
V2<float> rotate_v2(V2<float> v, float angle);
float angle_v2(V2<float> v);
float length(V2<float> v);

template <typename T>
V2<T> operator+(V2<T> a, V2<T> b)
{
    return V2<T>(a.x + b.x, a.y + b.y);
}

template <typename T>
V2<T> &operator+=(V2<T> &a, V2<T> b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

template <typename T>
V2<T> &operator*=(V2<T> &a, T s)
{
    a.x *= s;
    a.y *= s;
    return a;
}

template <typename T>
V2<T> &operator-=(V2<T> &a, V2<T> b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

template <typename T>
V2<T> operator-(V2<T> a, V2<T> b)
{
    return V2<T>(a.x - b.x, a.y - b.y);
}

template <typename T>
V2<T> operator*(V2<T> a, T s)
{
    return V2<T>(a.x * s, a.y * s);
}

template <typename T>
V2<T> operator*(V2<T> a, V2<T> b)
{
    return V2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
V2<T> operator*(T s, V2<T> a)
{
    return V2<T>(a.x * s, a.y * s);
}

template <typename T>
V2<T> operator/(V2<T> a, T s)
{
    return V2<T>(a.x / s, a.y / s);
}

template <typename T>
V2<T> operator/(T s, V2<T> a)
{
    return V2<T>(a.x / s, a.y / s);
}

template <typename T>
void print1(FILE *stream, V2<T> v2)
{
    print(stream, "V2(", v2.x, ", ", v2.y, ")");
}

// Triangle //////////////////////////////

const size_t TRIANGLE_VERT_COUNT = 3;

template <typename T>
struct Triangle {
    V2<T> vs[TRIANGLE_VERT_COUNT];

    Triangle() = default;

    Triangle(V2<T> a, V2<T> b, V2<T> c):
        vs {a, b, c}
    {}
};

static_assert(
    sizeof(Triangle<GLfloat>) == sizeof(GLfloat) * V2_COMPONENTS * TRIANGLE_VERT_COUNT,
    "Looks like compiler did an oopsie-doopsie and padded something incorrectly in the Triangle structure");

Triangle<float> equilateral_triangle(V2<float> center, float radius, float angle);

Triangle<float> rotate_triangle(Triangle<float> tri, float angle, V2<float> pivot);

template <typename T>
Triangle<T> operator+(Triangle<T> tri, V2<T> pos)
{
    return Triangle<T>(tri.vs[0] + pos,
                       tri.vs[1] + pos,
                       tri.vs[2] + pos);
}

template <typename T>
Triangle<T> operator-(Triangle<T> tri, V2<T> pos)
{
    return Triangle<T>(tri.vs[0] - pos,
                       tri.vs[1] - pos,
                       tri.vs[2] - pos);
}

void split_triangle(const Triangle<float> &tri,
                    size_t side, float f,
                    Triangle<float> output[2]);

void split_triangle(const Triangle<float> &tri,
                    const float fs[TRIANGLE_VERT_COUNT],
                    Triangle<float> output[TRIANGLE_VERT_COUNT + 1]);

template <typename T>
void print1(FILE *stream, Triangle<T> tri)
{
    print(stream, "Triangle(", tri.a, ", ", tri.b, ", ", tri.c, ")");
}

// Axis-Aligned Bounding Box //////////////////////////////

template <typename T>
struct AABB {
    V2<T> pos;
    V2<T> size;

    AABB() = default;

    AABB(V2<T> pos, V2<T> size):
        pos(pos), size(size)
    {}

    template <typename U>
    AABB<U> cast_to()
    {
        return AABB<U>(pos.template cast_to<U>(),
                       size.template cast_to<U>());
    }

    template <typename U>
    AABB<U> map(U (*f)(T))
    {
        return AABB(pos.map(f),
                    size.map(f));
    }

    void split_into_triangles(Triangle<T> *lower, Triangle<T> *upper)
    {
        if (lower) {
            *lower = Triangle(pos, pos + size * V2(0, 1).template cast_to<T>(), pos + size * V2(1, 0).template cast_to<T>());
        }

        if (upper) {
            *upper = Triangle(pos + size * V2(0, 1).template cast_to<T>(), pos + size * V2(1, 0).template cast_to<T>(), pos + size);
        }
    }

    AABB<T> flip_vertically() const
    {
        const auto new_pos = V2(pos.x, pos.y + size.y);
        const auto new_size = V2(size.x, -size.y);
        return AABB(new_pos, new_size);
    }

    AABB<T> flip_horizontally() const
    {
        const auto new_pos = V2(pos.x + size.x, pos.y);
        const auto new_size = V2(-size.x, size.y);
        return AABB(new_pos, new_size);
    }

    bool overlaps_with(const AABB<T> &that) const
    {
        const auto a1 = this->pos;
        const auto a2 = this->pos + this->size;

        const auto b1 = that.pos;
        const auto b2 = that.pos + that.size;

        return a2.x >= b1.x &&
               b2.x >= a1.x &&
               a2.y >= b1.y &&
               b2.y >= a1.y;
    }

};

// TODO: t == 0 in aabb_stretch() should mean no stretch
// It makes default zero initialization of the structures easier if you store the "stretch factor" as the field.
AABB<float> aabb_stretch(AABB<float> aabb, float t)
{
    const auto bottom_pos = aabb.pos + aabb.size * V2(0.5f, 0.0f);
    const auto new_size = aabb.size * V2(2.0f - t, t);
    const auto new_pos = bottom_pos - new_size * V2(0.5f, 0.0f);
    return AABB<float>(new_pos, new_size);
}

template <typename T>
void print1(FILE *stream, AABB<T> aabb)
{
    print(stream, "AABB(", aabb.pos, ",", aabb.size, ")");
}

float fmodulof(float a, float b)
{
    return fmodf(fmodf(a, b) + b, b);
}

#endif  // SOMETHING_V2_HPP_

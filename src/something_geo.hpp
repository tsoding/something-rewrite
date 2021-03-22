#ifndef SOMETHING_V2_HPP_
#define SOMETHING_V2_HPP_

float random01()
{
    static_assert(RAND_MAX != 0);
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

// 2D Vector //////////////////////////////

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

    static V2<T> polar(float angle, float mag)
    {
        return V2<T>(cos(angle), sin(angle)) * mag;
    }

    template <typename U>
    V2<U> cast_to()
    {
        return V2<U>(static_cast<U>(x),
                     static_cast<U>(y));
    }

    template <typename U>
    V2<U> map(U (*f)(T))
    {
        return V2<U>(f(x), f(y));
    }
};

V2<float> rotate_v2(V2<float> v, float angle);
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
void print1(FILE *stream, V2<T> v2)
{
    print(stream, "V2(", v2.x, ", ", v2.y, ")");
}

// Triangle //////////////////////////////

static constexpr size_t TRIANGLE_VERT_COUNT = 3;

template <typename T>
struct Triangle {
    V2<T> vs[TRIANGLE_VERT_COUNT];

    Triangle() = default;

    Triangle(V2<T> a, V2<T> b, V2<T> c):
        vs {a, b, c}
    {}
};

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
                    Triangle<float> *a,
                    Triangle<float> *b);

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
};

#endif  // SOMETHING_V2_HPP_

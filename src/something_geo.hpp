#ifndef SOMETHING_V2_HPP_
#define SOMETHING_V2_HPP_

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

template <typename T>
struct Triangle {
    V2<T> a;
    V2<T> b;
    V2<T> c;

    Triangle() = default;

    Triangle(V2<T> a, V2<T> b, V2<T> c):
        a(a), b(b), c(c)
    {}
};

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
};

#endif  // SOMETHING_V2_HPP_

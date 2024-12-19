#pragma once
#include <bits/stdc++.h>
using namespace std;

#define CEIL1 8
#define CEIL2 8
#define CEIL3 8
#define CEIL4 8
#define CEIL5 8
#define CEIL6 8
#define CEIL7 8
#define CEIL8 8
#define CEIL9 16
#define CEIL10 16
#define CEIL11 16
#define CEIL12 16
#define CEIL13 16
#define CEIL14 16
#define CEIL15 16
#define CEIL16 16
#define CEIL17 32
#define CEIL18 32
#define CEIL19 32
#define CEIL20 32
#define CEIL21 32
#define CEIL22 32
#define CEIL23 32
#define CEIL24 32
#define CEIL25 32
#define CEIL26 32
#define CEIL27 32
#define CEIL28 32
#define CEIL29 32
#define CEIL30 32
#define CEIL31 32
#define CEIL32 32
#define CEIL33 64
#define CEIL34 64
#define CEIL35 64
#define CEIL36 64
#define CEIL37 64
#define CEIL38 64
#define CEIL39 64
#define CEIL40 64
#define CEIL41 64
#define CEIL42 64
#define CEIL43 64
#define CEIL44 64
#define CEIL45 64
#define CEIL46 64
#define CEIL47 64
#define CEIL48 64
#define CEIL49 64
#define CEIL50 64
#define CEIL51 64
#define CEIL52 64
#define CEIL53 64
#define CEIL54 64
#define CEIL55 64
#define CEIL56 64
#define CEIL57 64
#define CEIL58 64
#define CEIL59 64
#define CEIL60 64
#define CEIL61 64
#define CEIL62 64
#define CEIL63 64
#define CEIL64 64

template <typename T, size_t S> class FastFixed;
template <typename T, size_t S> double to_double(FastFixed<T, S> x);

template <typename T, size_t S> class FastFixed
{
  public:
    constexpr FastFixed(int v) : v(v << S)
    {
    }
    constexpr FastFixed(float f) : v(f * (1 << S))
    {
    }
    constexpr FastFixed(double f) : v(f * (1 << S))
    {
    }
    constexpr FastFixed() : v(0)
    {
    }

    template <typename OTHER> constexpr FastFixed(OTHER x) : v(to_double(x) * (1 << S))
    {
    }

    static constexpr FastFixed from_raw(T x)
    {
        FastFixed ret;
        ret.v = x;
        return ret;
    }

    T v;

    auto operator<=>(const FastFixed &) const = default;
    bool operator==(const FastFixed &) const = default;

    template <typename OTHER> FastFixed &operator=(OTHER x)
    {
        v = to_double(x) * (1 << S);
        return *this;
    }

    template <typename OTHER> auto operator<=>(OTHER x) const
    {
        return v <=> to_double(x) * (1 << S);
    }

    template <typename OTHER> bool operator==(OTHER x) const
    {
        return v == to_double(x) * (1 << S);
    }

    static constexpr FastFixed get_epsilon()
    {
        return FastFixed::from_raw(4);
    }

    static constexpr FastFixed get_infinity()
    {
        return FastFixed::from_raw(std::numeric_limits<T>::max());
    }

    static FastFixed random01()
    {
        uint64_t r = rnd()%1'000'000'000;
        if (r < 0)
            r = -r;
        return FastFixed((double)r / 1'000'000'000);
    }
};

template <typename T, size_t S>
double to_double(FastFixed<T, S> x)
{
    return x.v / (double)(1 << S);
}

template <typename T, size_t S> FastFixed<T, S> operator+(FastFixed<T, S> a, FastFixed<T, S> b)
{
    return FastFixed<T, S>::from_raw(a.v + b.v);
}

template <typename OTHER, typename T, size_t S> FastFixed<T, S> operator+(FastFixed<T, S> a, OTHER b)
{
    return a + FastFixed<T, S>(to_double(b));
}

template <typename T, size_t S> FastFixed<T, S> operator-(FastFixed<T, S> a, FastFixed<T, S> b)
{
    return FastFixed<T, S>::from_raw(a.v - b.v);
}

template <typename OTHER, typename T, size_t S> FastFixed<T, S> operator-(FastFixed<T, S> a, OTHER b)
{
    return a - FastFixed<T, S>(to_double(b));
}

template <typename T, size_t S> FastFixed<T, S> operator*(FastFixed<T, S> a, FastFixed<T, S> b)
{
    return FastFixed<T, S>::from_raw(((int64_t)a.v * b.v) >> S);
}

template <typename OTHER, typename T, size_t S> FastFixed<T, S> operator*(FastFixed<T, S> a, OTHER b)
{
    return a * FastFixed<T, S>(to_double(b));
}

template <typename T, size_t S> FastFixed<T, S> operator/(FastFixed<T, S> a, FastFixed<T, S> b)
{
    return FastFixed<T, S>::from_raw(((int64_t)a.v << S) / b.v);
}

template <typename OTHER, typename T, size_t S> FastFixed<T, S> operator/(FastFixed<T, S> a, OTHER b)
{
    return a / FastFixed<T, S>(to_double(b));
}

template <typename T, size_t S> FastFixed<T, S> &operator+=(FastFixed<T, S> &a, FastFixed<T, S> b)
{
    return a = a + b;
}

template <typename OTHER, typename T, size_t S> FastFixed<T, S> &operator+=(FastFixed<T, S> &a, OTHER b)
{
    return a = a + FastFixed<T, S>(to_double(b));
}

template <typename T, size_t S> FastFixed<T, S> &operator-=(FastFixed<T, S> &a, FastFixed<T, S> b)
{
    return a = a - b;
}

template <typename OTHER, typename T, size_t S> FastFixed<T, S> &operator-=(FastFixed<T, S> &a, OTHER b)
{
    return a = a - FastFixed<T, S>(to_double(b));
}

template <typename T, size_t S> FastFixed<T, S> &operator*=(FastFixed<T, S> &a, FastFixed<T, S> b)
{
    return a = a * b;
}

template <typename OTHER, typename T, size_t S> FastFixed<T, S> &operator*=(FastFixed<T, S> &a, OTHER b)
{
    return a = a * FastFixed<T, S>(to_double(b));
}

template <typename T, size_t S> FastFixed<T, S> &operator/=(FastFixed<T, S> &a, FastFixed<T, S> b)
{
    return a = a / b;
}

template <typename OTHER, typename T, size_t S> FastFixed<T, S> &operator/=(FastFixed<T, S> &a, OTHER b)
{
    return a = a / FastFixed<T, S>(to_double(b));
}

template <typename T, size_t S> FastFixed<T, S> operator-(FastFixed<T, S> x)
{
    return FastFixed<T, S>::from_raw(-x.v);
}

template <typename T, size_t S> FastFixed<T, S> abs(FastFixed<T, S> x)
{
    if (x.v < 0)
    {
        x.v = -x.v;
    }
    return x;
}

template <typename T, size_t S> ostream &operator<<(ostream &out, FastFixed<T, S> x)
{
    return out << x.v / (double)(1 << S);
}
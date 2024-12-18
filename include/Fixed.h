#pragma once
#include <bits/stdc++.h>
using namespace std;

template <typename T, size_t S> class Fixed;
template <typename T, size_t S> double to_double(Fixed<T, S> x);

template <typename T, size_t S> class Fixed
{
  public:
    constexpr Fixed(int v) : v(v << S)
    {
    }
    constexpr Fixed(float f) : v(f * (1 << S))
    {
    }
    constexpr Fixed(double f) : v(f * (1 << S))
    {
    }
    constexpr Fixed() : v(0)
    {
    }

    template <typename OTHER> constexpr Fixed(OTHER x) : v(to_double(x) * (1 << S))
    {
    }

    static constexpr Fixed from_raw(T x)
    {
        Fixed ret;
        ret.v = x;
        return ret;
    }

    T v;

    auto operator<=>(const Fixed &) const = default;
    bool operator==(const Fixed &) const = default;

    template <typename OTHER> Fixed &operator=(OTHER x)
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

    static constexpr Fixed get_epsilon()
    {
        return Fixed::from_raw(4);
    }

    static constexpr Fixed get_infinity()
    {
        return Fixed::from_raw(std::numeric_limits<T>::max());
    }

    static Fixed random01()
    {
        uint64_t r = rnd();
        if (r < 0)
            r = -r;
        return Fixed::from_raw((double)r / ULLONG_MAX);
    }
};

template <typename T, size_t S>
double to_double(Fixed<T, S> x)
{
    return x.v / (double)(1 << S);
}

template <typename T, size_t S> Fixed<T, S> operator+(Fixed<T, S> a, Fixed<T, S> b)
{
    return Fixed<T, S>::from_raw(a.v + b.v);
}

template <typename OTHER, typename T, size_t S> Fixed<T, S> operator+(Fixed<T, S> a, OTHER b)
{
    return a + Fixed<T, S>(to_double(b));
}

template <typename T, size_t S> Fixed<T, S> operator-(Fixed<T, S> a, Fixed<T, S> b)
{
    return Fixed<T, S>::from_raw(a.v - b.v);
}

template <typename OTHER, typename T, size_t S> Fixed<T, S> operator-(Fixed<T, S> a, OTHER b)
{
    return a - Fixed<T, S>(to_double(b));
}

template <typename T, size_t S> Fixed<T, S> operator*(Fixed<T, S> a, Fixed<T, S> b)
{
    return Fixed<T, S>::from_raw(((int64_t)a.v * b.v) >> S);
}

template <typename OTHER, typename T, size_t S> Fixed<T, S> operator*(Fixed<T, S> a, OTHER b)
{
    return a * Fixed<T, S>(to_double(b));
}

template <typename T, size_t S> Fixed<T, S> operator/(Fixed<T, S> a, Fixed<T, S> b)
{
    return Fixed<T, S>::from_raw(((int64_t)a.v << S) / b.v);
}

template <typename OTHER, typename T, size_t S> Fixed<T, S> operator/(Fixed<T, S> a, OTHER b)
{
    return a / Fixed<T, S>(to_double(b));
}

template <typename T, size_t S> Fixed<T, S> &operator+=(Fixed<T, S> &a, Fixed<T, S> b)
{
    return a = a + b;
}

template <typename OTHER, typename T, size_t S> Fixed<T, S> &operator+=(Fixed<T, S> &a, OTHER b)
{
    return a = a + Fixed<T, S>(to_double(b));
}

template <typename T, size_t S> Fixed<T, S> &operator-=(Fixed<T, S> &a, Fixed<T, S> b)
{
    return a = a - b;
}

template <typename OTHER, typename T, size_t S> Fixed<T, S> &operator-=(Fixed<T, S> &a, OTHER b)
{
    return a = a - Fixed<T, S>(to_double(b));
}

template <typename T, size_t S> Fixed<T, S> &operator*=(Fixed<T, S> &a, Fixed<T, S> b)
{
    return a = a * b;
}

template <typename OTHER, typename T, size_t S> Fixed<T, S> &operator*=(Fixed<T, S> &a, OTHER b)
{
    return a = a * Fixed<T, S>(to_double(b));
}

template <typename T, size_t S> Fixed<T, S> &operator/=(Fixed<T, S> &a, Fixed<T, S> b)
{
    return a = a / b;
}

template <typename OTHER, typename T, size_t S> Fixed<T, S> &operator/=(Fixed<T, S> &a, OTHER b)
{
    return a = a / Fixed<T, S>(to_double(b));
}

template <typename T, size_t S> Fixed<T, S> operator-(Fixed<T, S> x)
{
    return Fixed<T, S>::from_raw(-x.v);
}

template <typename T, size_t S> Fixed<T, S> abs(Fixed<T, S> x)
{
    if (x.v < 0)
    {
        x.v = -x.v;
    }
    return x;
}

template <typename T, size_t S> ostream &operator<<(ostream &out, Fixed<T, S> x)
{
    return out << x.v / (double)(1 << S);
}
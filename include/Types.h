#include <bits/stdc++.h>
using namespace std;

mt19937_64 rnd(1337);

double to_double(int x)
{
    return x;
}

double to_double(float x)
{
    return x;
}

double to_double(double x)
{
    return x;
}

#include "Fixed.h"
#include "Double.h"
#include "Float.h"
#include "FastFixed.h"

#define FIXED(x, y) (Fixed<int##x##_t, y>)
#define FAST_FIXED__(x, y) (FastFixed<int_fast##x##_t, y>)
#define FAST_FIXED_(x, y) FAST_FIXED__(x, y)
#define FAST_FIXED(x, y) FAST_FIXED_(CEIL##x, y)
#define DOUBLE (Double)
#define FLOAT (Float)

std::string prettify(std::string s)
{
    if (s == "FLOAT")
    {
        return "(Float)";
    }
    if (s == "DOUBLE")
    {
        return "(Double)";
    }
    std::string f = "FIXED";
    if (s.substr(0, f.size()) == f)
    {
        int posOpen = s.find('(', 0);
        int posClose = s.find(')', 0);
        int posComma = s.find(',', 0);
        return "(Fixed<int" + s.substr(posOpen + 1, posComma - posOpen - 1) + "_t, " + s.substr(posComma + 1, posClose - posComma - 1) + ">)";
    }
    f = "FAST_FIXED";
    if (s.substr(0, f.size()) == f)
    {
        int posOpen = s.find('(', 0);
        int posClose = s.find(')', 0);
        int posComma = s.find(',', 0);

        int size = stoi(s.substr(posOpen + 1, posComma - posOpen - 1));
        size = size <= 8 ? 8 : size <= 16 ? 16 : size <= 32 ? 32 : 64;

        return "(FastFixed<int_fast" + to_string(size) + "_t, " + s.substr(posComma + 1, posClose - posComma - 1) + ">)";
    }

    throw std::runtime_error("Unknown type: " + s);
}


#include <bits/stdc++.h>

constexpr size_t T = 1'000'000;

#include "Simulator.h"
#include "Simulator_specialization.h"
#include "Types.h"

using namespace std;

#define S(a, b) a, b

#ifndef TYPES
#error "TYPES are not defined"
#endif

#ifndef SIZES
#define SIZES
#endif

#define EXPAND(...) EXPAND5(EXPAND5(EXPAND5(EXPAND5(__VA_ARGS__))))
#define EXPAND5(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__
#define PARENS ()
#define COMMA ,
#define UNPACK(...) __VA_ARGS__

#define EXPAND_SIZES(...) __VA_OPT__(EXPAND(EXPAND_SIZES_HELPER(__VA_ARGS__)))

#define EXPAND_SIZES_HELPER(first, second, ...) first, second __VA_OPT__(, EXPAND_SIZES_AGAIN PARENS(__VA_ARGS__))

#define EXPAND_SIZES_AGAIN() EXPAND_SIZES_HELPER

size_t sizes[] = {EXPAND_SIZES(SIZES)};

bool checkSizes(size_t x, size_t y)
{
    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i += 2)
    {
        if (sizes[i] == x && sizes[i + 1] == y)
        {
            return true;
        }
    }
    return false;
}

#define RUN_STATIC(t1, t2, t3, s1, s2)                                                                                 \
    if (givenX == s1 && givenY == s2 && #t1 == givenP && #t2 == givenV && #t3 == givenVFLOW)                           \
    {                                                                                                                  \
        Simulator<UNPACK t1, UNPACK t2, UNPACK t3, s1, s2> f;                                                          \
        f.run(source);                                                                                                 \
        flag = true;                                                                                                   \
    }

#define RUN_DYNAMIC(t1, t2, t3)                                                                                        \
    if (#t1 == givenP && #t2 == givenV && #t3 == givenVFLOW && !checkSizes(givenX, givenY))                            \
    {                                                                                                                  \
        Simulator<UNPACK t1, UNPACK t2, UNPACK t3, 0, 0> f(givenX, givenY);                                            \
        f.run(source);                                                                                                 \
        flag = true;                                                                                                   \
    }

#define PRINT(t1, t2, t3) std::cout << t1 << ' ' << t2 << ' ' << t3 << std::endl;

#define FOR_STATIC(commandStatic, t1, t2, t3, ...) __VA_OPT__(FOR_STATIC_HELPER(commandStatic, t1, t2, t3, __VA_ARGS__))

#define FOR_STATIC_HELPER(commandStatic, t1, t2, t3, first, second, ...)                                               \
    commandStatic(t1, t2, t3, first, second) __VA_OPT__(FOR_STATIC_AGAIN PARENS(commandStatic, t1, t2, t3, __VA_ARGS__))

#define FOR_STATIC_AGAIN() FOR_STATIC_HELPER

#define CREATE_CLASSES(commandStatic, commandDynamic, t1, t2, t3)                                                      \
    FOR_STATIC(commandStatic, t1, t2, t3, SIZES)                                                                       \
    commandDynamic(t1, t2, t3)

#define COMBINE1(commandStatic, commandDynamic, t1, t2, ...)                                                           \
    __VA_OPT__(COMBINE1_HELPER(commandStatic, commandDynamic, t1, t2, __VA_ARGS__))

#define COMBINE1_HELPER(commandStatic, commandDynamic, t1, t2, first, ...)                                             \
    CREATE_CLASSES(commandStatic, commandDynamic, t1, t2, first)                                                       \
    CREATE_CLASSES(commandStatic, commandDynamic, t1, first, t2)                                                       \
    CREATE_CLASSES(commandStatic, commandDynamic, t2, t1, first)                                                       \
    CREATE_CLASSES(commandStatic, commandDynamic, t2, first, t1)                                                       \
    CREATE_CLASSES(commandStatic, commandDynamic, first, t1, t2)                                                       \
    CREATE_CLASSES(commandStatic, commandDynamic, first, t2, t1)                                                       \
    __VA_OPT__(COMBINE1_AGAIN PARENS(commandStatic, commandDynamic, t1, t2, __VA_ARGS__))

#define COMBINE1_AGAIN() COMBINE1_HELPER

#define COMBINE2(commandStatic, commandDynamic, t1, ...)                                                               \
    __VA_OPT__(COMBINE2_HELPER(commandStatic, commandDynamic, t1, __VA_ARGS__))

#define COMBINE2_HELPER(commandStatic, commandDynamic, t1, first, ...)                                                 \
    CREATE_CLASSES(commandStatic, commandDynamic, t1, first, first)                                                    \
    CREATE_CLASSES(commandStatic, commandDynamic, first, t1, first)                                                    \
    CREATE_CLASSES(commandStatic, commandDynamic, first, first, t1)                                                    \
    CREATE_CLASSES(commandStatic, commandDynamic, first, t1, t1)                                                       \
    CREATE_CLASSES(commandStatic, commandDynamic, t1, first, t1)                                                       \
    CREATE_CLASSES(commandStatic, commandDynamic, t1, t1, first)                                                       \
    COMBINE1(commandStatic, commandDynamic, t1, first, __VA_ARGS__)                                                    \
    __VA_OPT__(COMBINE2_AGAIN PARENS(commandStatic, commandDynamic, t1, __VA_ARGS__))

#define COMBINE2_AGAIN() COMBINE2_HELPER

#define COMBINE3(commandStatic, commandDynamic, ...)                                                                   \
    __VA_OPT__(EXPAND(COMBINE3_HELPER(commandStatic, commandDynamic, __VA_ARGS__)))

#define COMBINE3_HELPER(commandStatic, commandDynamic, first, ...)                                                     \
    CREATE_CLASSES(commandStatic, commandDynamic, first, first, first)                                                 \
    COMBINE2(commandStatic, commandDynamic, first, __VA_ARGS__)                                                        \
    __VA_OPT__(COMBINE3_AGAIN PARENS(commandStatic, commandDynamic, __VA_ARGS__))

#define COMBINE3_AGAIN() COMBINE3_HELPER

int main(int argc, char **argv)
{
    bool flag = false;

    std::string givenP;
    std::string givenV;
    std::string givenVFLOW;
    std::string source;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        int pos = arg.find("=");
        std::string key = arg.substr(0, pos);
        std::string value = arg.substr(pos + 1);
        cout << key << ' ' << value << endl;
        if (key == "--p-type")
            givenP = value;
        else if (key == "--v-type")
            givenV = value;
        else if (key == "--v-flow-type")
            givenVFLOW = value;
        else if (key == "--source")
            source = value;
    }

    FILE *f = fopen(source.c_str(), "r");
    size_t givenX, givenY;
    fscanf(f, "%zu", &givenX);
    fscanf(f, "%zu", &givenY);
    fclose(f);

    givenP = prettify(givenP);
    givenV = prettify(givenV);
    givenVFLOW = prettify(givenVFLOW);

    COMBINE3(RUN_STATIC, RUN_DYNAMIC, TYPES)

    if (!flag)
    {
        cerr << "No matching simulator found" << endl;
        exit(1);
    }

    return 0;
}
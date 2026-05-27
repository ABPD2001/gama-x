#ifndef USER_HPP
#define USER_HPP
#include <cstdint>

using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

struct _GXA_snippet_t
{
    // <--- USER CUSTOM ---> //
    uint64_t t;
    uint32_t x;
    uint32_t y;
};

typedef long int logictype_t;
#endif
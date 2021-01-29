#include "rus.hpp"
#include "utf.hpp"

#include <numeric>
#include <stdexcept>

namespace hse {

using namespace std;

constexpr inline uint32_t  A_code = 0x0410;
constexpr inline uint32_t  a_code = 0x0430;
constexpr inline uint32_t YE_code = 0x0415;
constexpr inline uint32_t ye_code = 0x0435;
constexpr inline uint32_t YO_code = 0x0401;
constexpr inline uint32_t yo_code = 0x0451;
constexpr inline uint32_t YA_code = 0x042F;
constexpr inline uint32_t ya_code = 0x044F;


bool is_litte_russian(uint32_t c) noexcept {
    return ((a_code <= c && c <= ya_code) || c == yo_code);
}

bool is_big_russian(uint32_t c) noexcept {
    return ((A_code <= c && c <= YA_code) || c == YO_code);
}


uint32_t little_russia_to_int(uint32_t c)
{
    if (a_code <= c && c <= ye_code)
        return c-a_code;

    if (c == yo_code)
        return ye_code - a_code + 1;

    if (ye_code < c && c <= ya_code)
        return c-a_code+1;

    throw std::logic_error("Not russian letter in little_russia_to_int");
}

uint32_t int_to_little_russia(uint32_t c)
{
    static uint32_t
             a  = little_russia_to_int(a_code ),
             ya = little_russia_to_int(ya_code),
             ye = little_russia_to_int(ye_code),
             yo = little_russia_to_int(yo_code);

    if (a <= c && c <= ye)
        return c+a_code;

    if (c == yo)
        return yo_code;

    if (ye < c && c <= ya)
        return c-1+a_code;
    throw std::logic_error("Not russian letter in int_to_little_russia");
}

uint32_t big_russia_to_int(uint32_t c)
{
    if (A_code <= c && c <= YE_code)
        return c-A_code;

    if (c == YO_code)
        return YE_code - A_code + 1;

    if (YE_code < c && c <= YA_code)
        return c-A_code+1;
    throw std::logic_error("Not russian letter in big_russia_to_int");
}

uint32_t int_to_big_russia(uint32_t c)
{
    static uint32_t
             A  = big_russia_to_int(A_code ),
             YA = big_russia_to_int(YA_code),
             YE = big_russia_to_int(YE_code),
             YO = big_russia_to_int(YO_code);

    if (A <= c && c <= YE)
        return c+A_code;

    if (c == YO)
        return YO_code;

    if (YE < c && c <= YA)
        return c-1+A_code;
    throw std::logic_error("Not russian letter in int_to_big_russia");
}


}

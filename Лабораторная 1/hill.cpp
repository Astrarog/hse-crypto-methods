#include "hill.hpp"
#include "utf.hpp"

#include <string>
#include <vector>
#include <numeric>
#include <functional>

namespace hse {

using namespace std;

namespace  {

inline uint32_t  a_code(){ return utf8_to_unicode(u8"а")[0]; }
inline uint32_t  A_code(){ return utf8_to_unicode(u8"А")[0]; }
inline uint32_t ya_code(){ return utf8_to_unicode(u8"я")[0]; }
inline uint32_t YA_code(){ return utf8_to_unicode(u8"Я")[0]; }
inline uint32_t ye_code(){ return utf8_to_unicode(u8"е")[0]; }
inline uint32_t YE_code(){ return utf8_to_unicode(u8"Е")[0]; }
inline uint32_t yo_code(){ return utf8_to_unicode(u8"ё")[0]; }
inline uint32_t YO_code(){ return utf8_to_unicode(u8"Ё")[0]; }

}


inline bool is_litte_russian(uint32_t c){
    return ((a_code() <= c && c <= ya_code()) || c == yo_code());
}

inline bool is_big_russian(uint32_t c){
    return ((A_code() <= c && c <= YA_code()) || c == YO_code());
}



hill_cipher::hill_cipher(uint32_t a_, uint32_t b_): a(a_), b(b_)
{
    for (uint32_t inv = 1; inv<alphabet_size; ++inv){
        if ((inv * a) % alphabet_size == 1){
            inv_a = inv;
            return;
        }
    }
}

uint32_t little_russia_to_int(uint32_t c)
{
    if (a_code() <= c && c <= ye_code())
        return c-a_code();

    if (c == yo_code())
        return ye_code() - a_code() + 1;

    if (ye_code() < c && c <= ya_code())
        return c-a_code()+1;

    throw std::logic_error("Not russian letter in little_russia_to_int");
}

uint32_t int_to_little_russia(uint32_t c)
{
    uint32_t a  = little_russia_to_int(a_code() ),
             ya = little_russia_to_int(ya_code()),
             ye = little_russia_to_int(ye_code()),
             yo = little_russia_to_int(yo_code());

    if (a <= c && c <= ye)
        return c+a_code();

    if (c == yo)
        return yo_code();

    if (ye < c && c <= ya)
        return c-1+a_code();
    throw std::logic_error("Not russian letter in int_to_little_russia");
}

uint32_t big_russia_to_int(uint32_t c)
{
    if (A_code() <= c && c <= YE_code())
        return c-A_code();

    if (c == YO_code())
        return YE_code() - A_code() + 1;

    if (YE_code() < c && c <= YA_code())
        return c-A_code()+1;
    throw std::logic_error("Not russian letter in big_russia_to_int");
}

uint32_t int_to_big_russia(uint32_t c)
{
    uint32_t A  = big_russia_to_int(A_code() ),
             YA = big_russia_to_int(YA_code()),
             YE = big_russia_to_int(YE_code()),
             YO = big_russia_to_int(YO_code());

    if (A <= c && c <= YE)
        return c+A_code();

    if (c == YO)
        return YO_code();

    if (YE < c && c <= YA)
        return c-1+A_code();
    throw std::logic_error("Not russian letter in int_to_big_russia");
}

string hill_cipher::transform(string text,
                 function<uint32_t(uint32_t)> transformation)
{
    vector<uint32_t> text_symbols = utf8_to_unicode(move(text));
    for(auto& c: text_symbols)
    {
        if (is_litte_russian(c))
        {
            c = int_to_little_russia(
                    transformation(
                            little_russia_to_int(c)
                            )
                        );
        } else if(is_big_russian(c))
        {
            c = int_to_big_russia(
                    transformation(
                            big_russia_to_int(c)
                            )
                        );
        }
    }
    text = unicode_to_utf8(move(text_symbols));
    return text;
}

string hill_cipher::encode(string text)
{
    return transform(move(text),
                     [this]
                     (uint32_t x) { return (a * x + b) % alphabet_size; }
    );
}

string hill_cipher::decode(string text)
{
    return transform(move(text),
                     [this]
                     (uint32_t y){
                        if(y < b) y+=alphabet_size;
                        return (inv_a * (y - b)) % alphabet_size;
                     }
    );
}

}

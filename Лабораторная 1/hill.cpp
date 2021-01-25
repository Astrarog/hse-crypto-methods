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

}


inline bool is_litte_russian(uint32_t c){
    return (a_code() <= c && c <= ya_code());
}

inline bool is_big_russian(uint32_t c){
    return (A_code() <=  c  && c <= YA_code());
}



hill_cipher::hill_cipher(int a_, int b_): a(a_), b(b_)
{
    for (inv_a = 1; inv_a<alphabet_size; ++inv_a){
        if (inv_a * a % alphabet_size == 1)
            return;
    }
}

string hill_cipher::transform(string text,
                 function<uint32_t(uint32_t)> transformation)
{
    vector<uint32_t> text_symbols = utf8_to_unicode(move(text));
    for(auto& c: text_symbols)
    {
        if (is_litte_russian(c))
        {
            c = transformation(c - a_code()) + a_code();
        } else if(is_big_russian(c))
        {
            c = transformation(c - A_code()) + A_code();
        }
    }
    text = unicode_to_utf8(move(text_symbols));
    return text;
}

string hill_cipher::encode(string text)
{
    return transform(move(text),
                     [this]
                     (uint32_t c) { return (a * c + b) % alphabet_size;}
    );
}

string hill_cipher::decode(string text)
{
    return transform(move(text),
                     [this]
                     (uint32_t c){ return inv_a * (c - b) % alphabet_size; }
    );
}

}

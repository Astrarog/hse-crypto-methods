#include "hill.hpp"
#include "utf.hpp"
#include "rus.hpp"

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <stdexcept>

namespace hse {

using namespace std;

hill_cipher::hill_cipher(uint32_t a_, uint32_t b_, uint32_t n_) noexcept: a(a_), b(b_), n(n_)
{
    for (uint32_t inv = 1; inv<n; ++inv){
        if ((inv * a) % n == 1){
            inv_a = inv;
            return;
        }
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
                     (uint32_t x) { return (a * x + b) % n; }
    );
}

string hill_cipher::decode(string text)
{
    return transform(move(text),
                     [this]
                     (uint32_t y){
                        if(y < b) y+=n;
                        return (inv_a * (y - b)) % n;
                     }
    );
}

}

#ifndef HILL_HPP
#define HILL_HPP

#include <string>
#include <vector>
#include <numeric>
#include <functional>


namespace hse {

constexpr int alphabet_size = 33;

class hill_cipher
{
    int a, b, inv_a=-1;

    std::string transform(std::string text,
                     std::function<uint32_t(uint32_t)> transformation);

public:

    hill_cipher(int a_, int b_);

    std::string encode(std::string text);
    std::string decode(std::string text);

};

uint32_t little_russia_to_int(uint32_t c);
uint32_t int_to_little_russia(uint32_t c);
uint32_t big_russia_to_int(uint32_t c);
uint32_t int_to_big_russia(uint32_t c);
}

#endif // HILL_HPP

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

    constexpr hill_cipher(int a_, int b_);

    std::string encode(std::string text);
    std::string decode(std::string text);

};

}

#endif // HILL_HPP

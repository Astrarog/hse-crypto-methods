#ifndef HILL_HPP
#define HILL_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <functional>


namespace hse {


class hill_cipher
{
    uint32_t a, b, n, inv_a=0;

    std::string transform(std::string text,
                     std::function<uint32_t(uint32_t)> transformation);

public:

    hill_cipher(uint32_t a_, uint32_t b_, uint32_t n_) noexcept;

    std::string encode(std::string text);
    std::string decode(std::string text);

};

}

#endif // HILL_HPP

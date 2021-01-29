#ifndef UTF_HPP
#define UTF_HPP
#include <vector>
#include <string>
#include <numeric>

namespace hse {

// can be used as shifts for prefixes respectively
enum UNICODE_POSITION
{
    SINGLE_BYTE=7,
    DOUBLE_BEGINNING=5,
    TRIPPLE_BEGINNING=4,
    QUATRO_BEGINNING=3,
    MIDDLE_POSTITION=6
};


std::vector<uint32_t> utf8_to_unicode(const std::string& text);
std::string unicode_to_utf8(const std::vector<uint32_t>& data);

constexpr UNICODE_POSITION utf8_symbol_type(char c);
constexpr unsigned int needed_ut8_otcets(uint32_t letter_code);
constexpr bool check_prefix(char letter, char prefix) noexcept;

}

#endif // UTF_HPP

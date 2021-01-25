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

UNICODE_POSITION utf8_symbol_type(char c);
unsigned int needed_ut8_otcets(uint32_t letter_code);
bool check_prefix(char letter, char prefix);

extern inline constexpr uint32_t  single_mask = 0b01111111;
extern inline constexpr uint32_t  double_mask = 0b00011111;
extern inline constexpr uint32_t tripple_mask = 0b00001111;
extern inline constexpr uint32_t  quatro_mask = 0b00000111;
extern inline constexpr uint32_t  middle_mask = 0b00111111;

extern inline constexpr uint32_t  single_prefix = 0b0;
extern inline constexpr uint32_t  double_prefix = 0b110;
extern inline constexpr uint32_t tripple_prefix = 0b1110;
extern inline constexpr uint32_t  quatro_prefix = 0b11110;
extern inline constexpr uint32_t  middle_prefix = 0b10;

extern inline constexpr uint32_t data_shift = 6;

}

#endif // UTF_HPP

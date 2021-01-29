#include "utf.hpp"

#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>

namespace hse {

using namespace std;

constexpr inline uint32_t  single_mask = 0b01111111;
constexpr inline uint32_t  double_mask = 0b00011111;
constexpr inline uint32_t tripple_mask = 0b00001111;
constexpr inline uint32_t  quatro_mask = 0b00000111;
constexpr inline uint32_t  middle_mask = 0b00111111;

constexpr inline uint32_t  single_prefix = 0b0;
constexpr inline uint32_t  double_prefix = 0b110;
constexpr inline uint32_t tripple_prefix = 0b1110;
constexpr inline uint32_t  quatro_prefix = 0b11110;
constexpr inline uint32_t  middle_prefix = 0b10;

constexpr inline uint32_t data_shift = 6;

constexpr bool check_prefix(char letter, char prefix) noexcept
{
    unsigned int prefix_shift = [&prefix]{
        unsigned int first_nonzero_shift = 7;
        for(; first_nonzero_shift && !(0x1<<first_nonzero_shift & prefix);
            --first_nonzero_shift) {}
        return 7-first_nonzero_shift;
    }();

    unsigned int mask = [prefix_shift]{
        unsigned int mask=0x1;
        for(int count_shifts = 7-prefix_shift; count_shifts; --count_shifts){
            mask=(mask<<1)^0x1;
        }
        return mask<<prefix_shift;
    }();

    return (prefix << prefix_shift) == (letter & mask);

}

constexpr UNICODE_POSITION utf8_symbol_type(char c)
{
    if(check_prefix(c, single_prefix)) { //case 0b0xxxxxxx (complite)
        return SINGLE_BYTE;

    } else if (check_prefix(c, double_prefix)) { //case 0b110xxxxx (beginning)
        return DOUBLE_BEGINNING;

    } else if (check_prefix(c, tripple_prefix)) { //case 0b1110xxxx (beginning)
        return TRIPPLE_BEGINNING;

    } else if (check_prefix(c, quatro_prefix)) { //case 0b11110xxx (beginning)
        return QUATRO_BEGINNING;

    } else if (check_prefix(c, middle_prefix)) { //case 0b10xxxxxx (middle of data)
        return MIDDLE_POSTITION;

    } else {
        throw runtime_error("It is not utf-8 symbol");
    }
}

vector<uint32_t> utf8_to_unicode(const string& text)
{
    vector<uint32_t> data;
    data.reserve(text.size());
    for(size_t i = 0; i<text.size(); ++i)
    {
        uint32_t letter=0;
        switch (utf8_symbol_type(text[i])) {
        case SINGLE_BYTE:
            letter ^= text[i];
            break;
        case DOUBLE_BEGINNING:
            letter ^= (text[i]  & double_mask); letter<<=data_shift;
            letter ^= (text[i+1]& middle_mask);
            i+=1;
            break;
        case TRIPPLE_BEGINNING:
            letter ^= (text[i]  & tripple_mask); letter<<=data_shift;
            letter ^= (text[i+1]& middle_mask);  letter<<=data_shift;
            letter ^= (text[i+2]& middle_mask);
            i+=2;
            break;
        case QUATRO_BEGINNING:
            letter ^= (text[i]  & quatro_mask); letter<<=data_shift;
            letter ^= (text[i+1]& middle_mask); letter<<=data_shift;
            letter ^= (text[i+2]& middle_mask); letter<<=data_shift;
            letter ^= (text[i+3]& middle_mask);
            i+=3;
            break;
        case MIDDLE_POSTITION:
            // should not be here casue have taken into account in all other cases
            throw std::logic_error("Get middle position while ut8 to unocode encoding");
        }
        data.push_back(letter);
    }
    return data;
}

constexpr unsigned int needed_ut8_otcets(uint32_t letter_code)
{
    if(letter_code <= 0x7F)
        return 1;
    if(letter_code<=0x7FF)
        return 2;
    if(letter_code<=0xFFFF)
        return 3;
    if(letter_code<=0x10FFFF)
        return 4;
    throw logic_error("No symbol for such code (unicode)");
}

string unicode_to_utf8(const vector<uint32_t>& data)
{
    string text;
    text.reserve(data.size() * sizeof(uint32_t));
    for(auto letter: data)
    {
        unsigned int need_octets = needed_ut8_otcets(letter);

        string reverse_symol;
        reverse_symol.reserve(sizeof(uint32_t));

        for(unsigned int i = 1; i<need_octets; ++i)
        {
            reverse_symol.push_back((middle_prefix << MIDDLE_POSTITION) ^ (letter & middle_mask));
            letter >>= MIDDLE_POSTITION;
        }

        uint32_t prefix, prefix_shift, mask;
        switch (need_octets) {

            case 1:
                prefix = single_prefix;
          prefix_shift = SINGLE_BYTE;
                  mask = single_mask;
                break;

            case 2:
                prefix = double_prefix;
          prefix_shift = DOUBLE_BEGINNING;
                  mask = double_mask;
                break;

            case 3:
                prefix = tripple_prefix;
          prefix_shift = TRIPPLE_BEGINNING;
                  mask = tripple_mask;
                break;

            case 4:
                prefix = quatro_prefix;
          prefix_shift = QUATRO_BEGINNING;
                  mask = quatro_mask;
                break;
        }
        reverse_symol.push_back((prefix << prefix_shift) ^ (letter & mask));
        copy(reverse_symol.rbegin(), reverse_symol.rend(), back_inserter(text));
    }
    return text;
}


}

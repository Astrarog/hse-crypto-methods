#ifndef RUS_HPP
#define RUS_HPP
#include <cstdint>

namespace hse {

bool is_litte_russian(uint32_t c) noexcept;
bool is_big_russian(uint32_t c) noexcept;
bool is_russian(uint32_t c) noexcept;

uint32_t little_russia_to_int(uint32_t c);
uint32_t int_to_little_russia(uint32_t c);
uint32_t big_russia_to_int(uint32_t c);
uint32_t int_to_big_russia(uint32_t c);

uint32_t russia_to_int(uint32_t c);

extern constexpr inline uint32_t alphabet_size = 33;

}

#endif // RUS_HPP

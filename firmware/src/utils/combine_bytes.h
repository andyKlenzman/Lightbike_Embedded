#include <stdint.h>

uint16_t combine_bytes(uint8_t msb, uint8_t lsb) {
    // Shift the most significant byte by 8 bits to the left and combine with the least significant byte
    return ((uint16_t)msb << 8) | lsb;
}
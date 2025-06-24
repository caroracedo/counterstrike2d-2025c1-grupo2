#ifndef BYTE_CONVERTER_H
#define BYTE_CONVERTER_H

#include <cstdint>
#include <vector>

class ByteConverter {
public:
    /* Manipulación de bytes */
    uint16_t hex_big_endian_to_uint_16(const std::vector<uint8_t>& hex_big_endian);
    float hex_big_endian_to_float(const std::vector<uint8_t>& hex_big_endian);
    void push_uint_16_to(uint16_t valor, std::vector<uint8_t>& vector);
    void push_float_to(float valor, std::vector<uint8_t>& vector);

private:
    /* Manipulación de bytes */
    std::vector<uint8_t> uint_16_to_hex_big_endian(uint16_t int_16);
    std::vector<uint8_t> float_to_hex_big_endian(float valor);
    void push_hexa_to(const std::vector<uint8_t>& hexa, std::vector<uint8_t>& vector);
};

#endif  // BYTE_CONVERTER_H

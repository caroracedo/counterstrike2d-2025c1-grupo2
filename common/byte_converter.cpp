#include "byte_converter.h"

#include <cstring>

#include <arpa/inet.h>

/* Manipulación de bytes */
std::vector<uint8_t> ByteConverter::uint_16_to_hex_big_endian(uint16_t int_16) {
    uint16_t htons_int_16 = htons(int_16);
    std::vector<uint8_t> hex_big_endian(sizeof(htons_int_16));
    std::memcpy(hex_big_endian.data(), &htons_int_16, sizeof(htons_int_16));
    return hex_big_endian;
}

std::vector<uint8_t> ByteConverter::float_to_hex_big_endian(float valor) {
    uint32_t raw;
    std::memcpy(&raw, &valor, sizeof(raw));
    raw = htonl(raw);
    std::vector<uint8_t> hex_big_endian(sizeof(raw));
    std::memcpy(hex_big_endian.data(), &raw, sizeof(raw));
    return hex_big_endian;
}

void ByteConverter::push_hexa_to(const std::vector<uint8_t>& hexa, std::vector<uint8_t>& vector) {
    vector.insert(vector.end(), hexa.begin(), hexa.end());
}

uint16_t ByteConverter::hex_big_endian_to_uint_16(const std::vector<uint8_t>& hex_big_endian) {
    uint16_t int_16;
    std::memcpy(&int_16, hex_big_endian.data(), sizeof(int_16));
    return ntohs(int_16);
}

float ByteConverter::hex_big_endian_to_float(const std::vector<uint8_t>& hex_big_endian) {
    uint32_t int_32;
    std::memcpy(&int_32, hex_big_endian.data(), sizeof(int_32));
    int_32 = ntohl(int_32);
    float valor;
    std::memcpy(&valor, &int_32, sizeof(valor));
    return valor;
}

void ByteConverter::push_uint_16_to(uint16_t valor, std::vector<uint8_t>& vector) {
    push_hexa_to(uint_16_to_hex_big_endian(valor), vector);
}

void ByteConverter::push_float_to(float valor, std::vector<uint8_t>& vector) {
    push_hexa_to(float_to_hex_big_endian(valor), vector);
}

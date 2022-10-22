
#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include <stdint.h>

/* 16 General purpose registers */
#define CHIP8_REGISTER_COUNT 16

struct Chip8Registers {
    uint8_t V[CHIP8_REGISTER_COUNT];
    uint16_t I;
    uint16_t PC;
    uint8_t SP;
    uint8_t DT;
    uint8_t ST;
};

#endif /* CHIP8_REGISTERS_H */

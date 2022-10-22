
#ifndef CHIP8_H
#define CHIP8_H

#include <chip8registers.h>
#include <stdint.h>

/* CHIP-8 Specification */
#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_STACK_SIZE 16
#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32
#define CHIP8_DISPLAY_SIZE (CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT)
#define CHIP8_KEY_COUNT 16

struct Chip8 {
    struct Chip8Registers registers;
    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint16_t stack[CHIP8_STACK_SIZE];
    uint8_t display[CHIP8_DISPLAY_SIZE];
    uint8_t keys[CHIP8_KEY_COUNT];
    uint8_t delay_timer;
    uint8_t sound_timer;
};

#endif /* CHIP8_H */

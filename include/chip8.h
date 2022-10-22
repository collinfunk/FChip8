
#ifndef CHIP8_H
#define CHIP8_H

#include <chip8registers.h>
#include <stdint.h>
#include <stdio.h>

/* CHIP-8 Specification */
#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_STACK_SIZE 16
#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32
#define CHIP8_DISPLAY_SIZE (CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT)
#define CHIP8_KEY_COUNT 16

#define CHIP8_PROGRAM_START 0x200

struct Chip8 {
    struct Chip8Registers regs;
    uint8_t *memory;
    uint16_t stack[CHIP8_STACK_SIZE];
    uint8_t display[CHIP8_DISPLAY_SIZE];
    uint8_t keys[CHIP8_KEY_COUNT];
    uint8_t delay_timer;
    uint8_t sound_timer;
};

/* Set up the CHIP-8 and load the rom */
void chip8_init(struct Chip8 *chip8, const char *filename);
/* Free the dynamically allocated memory from chip8_init */
void chip8_free_memory(struct Chip8 *chip8);
/* Disassemble the rom loaded into memory */
extern void chip8_disassemble(const char* filename, FILE* output_stream);

#endif /* CHIP8_H */

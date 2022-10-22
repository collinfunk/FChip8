
#ifndef CHIP8_H_
#define CHIP8_H_

#include <SDL2/SDL.h>
#include <chip8registers.h>
#include <stdbool.h>
#include <stdint.h>

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_START_ADDRESS 0x200
#define CHIP8_FONTSET_SIZE 80

#define CHIP8_STACK_SIZE 16
#define CHIP8_KEYPAD_SIZE 16

#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32
#define CHIP8_DISPLAY_SIZE CHIP8_DISPLAY_WIDTH *CHIP8_DISPLAY_HEIGHT

struct Chip8 {
    struct Chip8Registers regs;
    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint16_t stack[CHIP8_STACK_SIZE];
    uint8_t keypad[CHIP8_KEYPAD_SIZE];
    uint8_t display[CHIP8_DISPLAY_SIZE];
    bool draw_flag;
};

void chip8_init(struct Chip8 *chip8);
void chip8_load(struct Chip8 *chip8, const char *filename);
extern void chip8_disassemble(const char *filename, FILE *output_stream);
void chip8_cycle(struct Chip8 *chip8);
void chip8_draw(struct Chip8 *chip8, SDL_Renderer *renderer,
                SDL_Texture *texture, uint8_t window_scale);

#endif /* CHIP8_H_ */

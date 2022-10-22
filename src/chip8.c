
#include <chip8.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CHIP8_FONTSET_MEMORY 80

static const uint8_t chip8_fontset[CHIP8_FONTSET_MEMORY] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
        0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
        0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
        0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
        0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
        0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
        0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
        0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
        0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
        0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
        0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
        0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
        0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
        0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
        0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
        0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

/* Set up the CHIP-8 and load the rom */
void chip8_init(struct Chip8 *chip8, const char *filename) {

    /* Open file, check for errors */
    FILE *file_descriptor = fopen(filename, "rb");
    if (file_descriptor == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Get file size */
    fseek(file_descriptor, 0, SEEK_END);
    long file_size = ftell(file_descriptor);
    rewind(file_descriptor);

    /* Check if file is too large */
    if (file_size > 0x1000 - 0x200) {
        fprintf(stderr, "Error: File %s is too large\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Allocate memory and load the rom into memory */
    chip8->memory = malloc(sizeof(uint8_t) * file_size);

    if (chip8->memory == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fread(chip8->memory, sizeof(uint8_t), file_size, file_descriptor);

    /* Close file */
    if (fclose(file_descriptor) != 0) {
        fprintf(stderr, "Error: Could not close file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Load fontset into memory */
    memcpy(chip8->memory, chip8_fontset, CHIP8_FONTSET_MEMORY);

    /* Zero out registers */
    chip8->regs.PC = CHIP8_PROGRAM_START;
    chip8->regs.SP = 0;
    chip8->regs.I = 0;
    memset(chip8->regs.V, 0, sizeof(chip8->regs.V));

    /* Stack, keys, display, timers */
    memset(chip8->stack, 0, CHIP8_STACK_SIZE);
    memset(chip8->display, 0, CHIP8_DISPLAY_SIZE);
    memset(chip8->keys, 0, CHIP8_KEY_COUNT);
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

}

/* Make sure to free the memory allocated (I love C :3) */
void chip8_free_memory(struct Chip8 *chip8) {
    free(chip8->memory);
}

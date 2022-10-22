
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chip8.h>

/* Macros so I don't get annoyed :P */
#define CHIP8_INSTRUCTION(opcode) ((opcode & 0xF000) >> 12)
#define CHIP8_INSTRUCTION_X(opcode) ((opcode & 0x0F00) >> 8)
#define CHIP8_INSTRUCTION_Y(opcode) ((opcode & 0x00F0) >> 4)
#define CHIP8_INSTRUCTION_N(opcode) (opcode & 0x000F)
#define CHIP8_INSTRUCTION_NN(opcode) (opcode & 0x00FF)
#define CHIP8_INSTRUCTION_NNN(opcode) (opcode & 0x0FFF)

/* Fontset */
static const uint8_t chip8_fontset[CHIP8_FONTSET_SIZE] = {
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

/* Sets all the CHIP8 data to 0 and loads fontset */
void chip8_init(struct Chip8 *chip8) {
    /* Clear all the arrays */
    memset(chip8->memory, 0, CHIP8_MEMORY_SIZE);
    memset(chip8->stack, 0, CHIP8_STACK_SIZE);
    memset(chip8->keypad, 0, CHIP8_KEYPAD_SIZE);
    memset(chip8->display, 0, CHIP8_DISPLAY_SIZE);
    memset(chip8->regs.V, 0, 16);

    /* Load the fontset */
    memcpy(chip8->memory, chip8_fontset, CHIP8_FONTSET_SIZE);

    /* Reset the registers */
    chip8->regs.I = 0;
    chip8->regs.PC = CHIP8_START_ADDRESS;
    chip8->regs.SP = 0;
    chip8->regs.DT = 0;
    chip8->regs.ST = 0;
}

/* Load a ROM into the memory */
void chip8_load(struct Chip8 *chip8, const char *filename) {
    FILE *file_descriptor = fopen(filename, "rb");
    if (file_descriptor == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(1);
    }

    /* Get the file size */
    fseek(file_descriptor, 0, SEEK_END);
    long file_size = ftell(file_descriptor);
    rewind(file_descriptor);

    /* Read the file into memory */
    fread(chip8->memory + CHIP8_START_ADDRESS, 1, file_size, file_descriptor);
    fclose(file_descriptor);

    printf("Loaded %s into memory\n", filename);
}
/* Fetches an opcode */
static inline uint16_t fetch_opcode(struct Chip8 *chip8) {
    uint16_t opcode =
            chip8->memory[chip8->regs.PC] << 8 | chip8->memory[chip8->regs.PC + 1];
    chip8->regs.PC += 2;
    return opcode;
}

/* A single cycle */
void chip8_cycle(struct Chip8 *chip8) {
    uint16_t opcode = fetch_opcode(chip8);
    uint8_t x = CHIP8_INSTRUCTION_X(opcode);
    uint8_t y = CHIP8_INSTRUCTION_Y(opcode);
    uint8_t n = CHIP8_INSTRUCTION_N(opcode);
    uint8_t nn = CHIP8_INSTRUCTION_NN(opcode);
    uint16_t nnn = CHIP8_INSTRUCTION_NNN(opcode);

    switch (CHIP8_INSTRUCTION(opcode)) {
        case 0x0: {
            switch (nn) {
                case 0xE0: /* CLS */
                    memset(chip8->display, 0, sizeof(chip8->display));
                    break;
                case 0xEE: /* RET */
                    chip8->regs.PC = chip8->stack[--chip8->regs.SP];
                    break;
                default:
                    break; /* Unknown instruction, nop */
            } /* switch (nn) */
            break;
        }         /* case: 0x0 */
        case 0x1: /* JP addr */
            chip8->regs.PC = nnn;
            break;
        case 0x2: /* CALL addr */
            chip8->stack[chip8->regs.SP++] = chip8->regs.PC;
            chip8->regs.PC = nnn;
            break;
        case 0x3: /* SE Vx, byte */
            if (chip8->regs.V[x] == nn) {
                chip8->regs.PC += 2;
            }
            break;
        case 0x4: /* SNE Vx, byte */
            if (chip8->regs.V[x] != nn) {
                chip8->regs.PC += 2;
            }
            break;
        case 0x5: /* SE Vx, Vy */
            if (chip8->regs.V[x] == chip8->regs.V[y]) {
                chip8->regs.PC += 2;
            }
            break;
        case 0x6: /* LD Vx, byte */
            chip8->regs.V[x] = nn;
            break;
        case 0x7: /* ADD Vx, byte */
            chip8->regs.V[x] += nn;
            break;
        case 0x8: {
            switch (n) {
                case 0x0: /* LD Vx, Vy */
                    chip8->regs.V[x] = chip8->regs.V[y];
                    break;
                case 0x1: /* OR Vx, Vy */
                    chip8->regs.V[x] |= chip8->regs.V[y];
                    break;
                case 0x2: /* AND Vx, Vy */
                    chip8->regs.V[x] &= chip8->regs.V[y];
                    break;
                case 0x3: /* XOR Vx, Vy */
                    chip8->regs.V[x] ^= chip8->regs.V[y];
                    break;
                case 0x4: /* ADD Vx, Vy */
                    chip8->regs.V[0xF] = (chip8->regs.V[x] + chip8->regs.V[y] > 0xFF);
                    chip8->regs.V[x] += chip8->regs.V[y];
                    break;
                case 0x5: /* SUB Vx, Vy */
                    chip8->regs.V[0xF] = (chip8->regs.V[x] > chip8->regs.V[y]);
                    chip8->regs.V[x] -= chip8->regs.V[y];
                    break;
                case 0x6: /* SHR Vx {, Vy} */
                    chip8->regs.V[0xF] = chip8->regs.V[x] & 0x1;
                    chip8->regs.V[x] >>= 1;
                    break;
                case 0x7: /* SUBN Vx, Vy */
                    chip8->regs.V[0xF] = (chip8->regs.V[y] > chip8->regs.V[x]);
                    chip8->regs.V[x] = chip8->regs.V[y] - chip8->regs.V[x];
                    break;
                case 0xE: /* SHL Vx {, Vy} */
                    chip8->regs.V[0xF] = chip8->regs.V[x] >> 7;
                    chip8->regs.V[x] <<= 1;
                    break;
                default:
                    break; /* Unknown instruction, nop */
            } /* switch (n) */
            break;
        }         /* case: 0x8 */
        case 0x9: /* SNE Vx, Vy */
            if (chip8->regs.V[x] != chip8->regs.V[y]) {
                chip8->regs.PC += 2;
            }
            break;
        case 0xA: /* LD I, addr */
            chip8->regs.I = nnn;
            break;
        case 0xB: /* JP V0, addr */
            chip8->regs.PC = chip8->regs.V[0] + nnn;
            break;
        case 0xC: /* RND Vx, byte */
            chip8->regs.V[x] = (unsigned char) (rand() % 0xFF) & nn;
            break;
        case 0xD: /* DRW Vx, Vy, n */
            chip8->regs.V[0xF] = 0;
            for (int yline = 0; yline < n; yline++) {
                uint8_t pixel = chip8->memory[chip8->regs.I + yline];
                for (int xline = 0; xline < 8; xline++) {
                    if ((pixel & (0x80 >> xline)) != 0) {
                        int index =
                                (chip8->regs.V[x] + xline + ((chip8->regs.V[y] + yline) * 64));
                        if (chip8->display[index] == 1) {
                            chip8->regs.V[0xF] = 1;
                        }
                        chip8->display[index] ^= 1;
                    }
                }
            }
            chip8->draw_flag = true;
            break;
        case 0xE: {
            switch (nn) {
                case 0x9E: /* SKP Vx */
                    if (chip8->keypad[chip8->regs.V[x]]) {
                        chip8->regs.PC += 2;
                    }
                    break;
                case 0xA1: /* SKNP Vx */
                    if (!chip8->keypad[chip8->regs.V[x]]) {
                        chip8->regs.PC += 2;
                    }
                    break;
                default:
                    break; /* Unknown instruction, nop */
            } /* switch (nn) */
            break;
        } /* case: 0xE */
        case 0xF: {
            switch (nn) {
                case 0x07: /* LD Vx, DT */
                    chip8->regs.V[x] = chip8->regs.DT;
                    break;
                case 0x0A: /* LD Vx, K */
                    chip8->regs.V[x] = chip8->keypad[chip8->regs.V[x]];
                    break;
                case 0x15: /* LD DT, Vx */
                    chip8->regs.DT = chip8->regs.V[x];
                    break;
                case 0x18: /* LD ST, Vx */
                    chip8->regs.ST = chip8->regs.V[x];
                    break;
                case 0x1E: /* ADD I, Vx */
                    chip8->regs.I += chip8->regs.V[x];
                    break;
                case 0x29: /* LD F, Vx */
                    chip8->regs.I = chip8->regs.V[x] * 5;
                    break;
                case 0x33: /* LD B, Vx */
                    chip8->memory[chip8->regs.I] = chip8->regs.V[x] / 100;
                    chip8->memory[chip8->regs.I + 1] = (chip8->regs.V[x] / 10) % 10;
                    chip8->memory[chip8->regs.I + 2] = (chip8->regs.V[x] % 100) % 10;
                    break;
                case 0x55: /* LD [I], Vx */
                    for (int i = 0; i <= x; i++) {
                        chip8->memory[chip8->regs.I + i] = chip8->regs.V[i];
                    }
                    break;
                case 0x65: /* LD Vx, [I] */
                    for (int i = 0; i <= x; i++) {
                        chip8->regs.V[i] = chip8->memory[chip8->regs.I + i];
                    }
                    break;
                default:
                    break; /* Unknown instruction, nop */
            } /* switch (nn) */
            break;
        } /* case: 0xF */
        default:
            break; /* Unknown instruction, nop */
    } /* end of opcode switch */

    /* timers */

    if (chip8->regs.DT > 0) {
        chip8->regs.DT--;
    }

    /* I think SDL has sound maybe ill learn */
    if (chip8->regs.ST > 0) {
        if (chip8->regs.ST == 1) {
            printf("^_^ :3\n");
        }
        chip8->regs.ST--;
    }
}

void chip8_draw(struct Chip8 *chip8, SDL_Renderer *renderer,
                SDL_Texture *texture, uint8_t window_scale) {
    if (!chip8->draw_flag) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (chip8->display[(y * 64) + x] == 1) {
                SDL_Rect rect = {x * window_scale, y * window_scale, window_scale,
                                 window_scale};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
    chip8->draw_flag = false;
}

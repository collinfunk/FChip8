
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Macros for getting register index, immediate values etc. */
#define NNN(opcode) (opcode & 0x0FFF)
#define NN(opcode) (opcode & 0x00FF)
#define N(opcode) (opcode & 0x000F)
#define X(opcode) ((opcode & 0x0F00) >> 8)
#define Y(opcode) ((opcode & 0x00F0) >> 4)

/* 0x0??? sub-opcodes */
static void chip8_disassemble_0xxx(size_t address, uint16_t opcode, FILE *output) {
    switch (opcode & 0x000F) {
        case 0x0000:
            fprintf(output, "ADDR %#04zx: CLS\n", address);
            break;
        case 0x000E:
            fprintf(output, "ADDR %#04zx: RET\n", address);
            break;
        default:
            fprintf(output, "ADDR %#04zx: SYS 0x%03X\n", address, NNN(opcode));
            break;
    }
}

/* 0x8??? sub-opcodes */
static void chip8_disassemble_8xxx(size_t address, uint16_t opcode, FILE *output) {
    switch (opcode & 0x000F) {
        case 0x0000:
            fprintf(output, "ADDR %#04zx: LD V[%d], V[%d]\n", address, X(opcode), Y(opcode));
            break;
        case 0x0001:
            fprintf(output, "ADDR %#04zx: OR V[%d], V[%d]\n", address, X(opcode), Y(opcode));
            break;
        case 0x0002:
            fprintf(output, "ADDR %#04zx: AND V[%d], V[%d]\n", address, X(opcode), Y(opcode));
            break;
        case 0x0003:
            fprintf(output, "ADDR %#04zx: XOR V[%d], V[%d]\n", address, X(opcode), Y(opcode));
            break;
        case 0x0004:
            fprintf(output, "ADDR %#04zx: ADD V[%d], V[%d]\n", address, X(opcode), Y(opcode));
            break;
        case 0x0005:
            fprintf(output, "ADDR %#04zx: SUB V[%d], V[%d]\n", address, X(opcode), Y(opcode));
            break;
        case 0x0006:
            fprintf(output, "ADDR %#04zx: SHR V[%d], V[%d]\n", address, X(opcode), Y(opcode));
            break;
        case 0x0007:
            fprintf(output, "ADDR %#04zx: SUBN V[%d], V[%d]\n", address, X(opcode), Y(opcode));
            break;
        case 0x000E:
            fprintf(output, "ADDR %#04zx: SHL V[%d], V[%d]\n", address, X(opcode), Y(opcode));
            break;
        default:
            fprintf(output, "ADDR %#04zx: UNKNOWN 0x%04X\n", address, opcode);
            break;
    }
}

/* 0xe??? sub-opcodes */
static void chip8_disassemble_exxx(size_t address, uint16_t opcode, FILE *output) {
    switch (opcode & 0x00FF) {
        case 0x009E:
            fprintf(output, "ADDR %#04zx: SKP V[%d]\n", address, X(opcode));
            break;
        case 0x00A1:
            fprintf(output, "ADDR %#04zx: SKNP V[%d]\n", address, X(opcode));
            break;
        default:
            fprintf(output, "ADDR %#04zx: Unknown opcode 0x%04X\n", address, opcode);
            break;
    }
}

/* 0xf??? sub-opcodes */
static void chip8_disassemble_fxxx(size_t address, uint16_t opcode, FILE *output) {
    switch (opcode & 0x00FF) {
        case 0x0007:
            fprintf(output, "ADDR %#04zx: LD V[%d], DT\n", address, X(opcode));
            break;
        case 0x000A:
            fprintf(output, "ADDR %#04zx: LD V[%d], K\n", address, X(opcode));
            break;
        case 0x0015:
            fprintf(output, "ADDR %#04zx: LD DT, V[%d]\n", address, X(opcode));
            break;
        case 0x0018:
            fprintf(output, "ADDR %#04zx: LD ST, V[%d]\n", address, X(opcode));
            break;
        case 0x001E:
            fprintf(output, "ADDR %#04zx: ADD I, V[%d]\n", address, X(opcode));
            break;
        case 0x0029:
            fprintf(output, "ADDR %#04zx: LD F, V[%d]\n", address, X(opcode));
            break;
        case 0x0033:
            fprintf(output, "ADDR %#04zx: LD B, V[%d]\n", address, X(opcode));
            break;
        case 0x0055:
            fprintf(output, "ADDR %#04zx: LD [I], V[%d]\n", address, X(opcode));
            break;
        case 0x0065:
            fprintf(output, "ADDR %#04zx: LD V[%X], [I]\n", address, X(opcode));
            break;
        default:
            fprintf(output, "ADDR %#04zx: Unknown opcode 0x%04X\n", address, opcode);
            break;
    }
}

void chip8_disassemble(const char* filename, FILE* output_stream) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Get file size */
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    /* Check if file is too large */
    if (file_size > 0x1000 - 0x200) {
        fprintf(stderr, "Error: File %s is too large\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Allocate memory and load the rom into memory */
    uint8_t* memory = malloc(sizeof(uint8_t) * file_size);

    if (memory == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fread(memory, sizeof(uint8_t), file_size, file);

    /* Close file */
    fclose(file);

    uint16_t opcode;

    for (size_t i = 0; i < file_size; i += 2) {
        opcode = memory[i] << 8 | memory[i + 1];

        switch ((opcode & 0xF000) >> 12) {  /* 0x?000 */
            case 0x0:  /* 0x0??? CLS, RET */
                chip8_disassemble_0xxx(i, opcode, output_stream);
                break;
            case 0x1: /* 0x1NNN, jump to address NNN */
                fprintf(output_stream, "ADDR %#04zx: JP %#03x\n", i, NNN(opcode));
                break;
            case 0x2: /* 0x2NNN, call subroutine at NNN */
                fprintf(output_stream, "ADDR %#04zx: CALL %#03x\n", i, NNN(opcode));
                break;
            case 0x03:  /* 0x3XNN, skip next instruction if VX == NN */
                fprintf(output_stream, "ADDR %#04zx: SE V[%X], %#02x\n", i, X(opcode), NN(opcode));
                break;
            case 0x4: /* 0x4XNN, skip next instruction if VX != NN */
                fprintf(output_stream, "ADDR %#04zx: SNE V[%X], %#02x\n", i, X(opcode), NN(opcode));
                break;
            case 0x5: /* 0x5XY0, skip next instruction if VX == VY */
                fprintf(output_stream, "ADDR %#04zx: SE V[%X], V[%X]\n", i, X(opcode), Y(opcode));
                break;
            case 0x6: /* 0x6XNN, set VX to NN */
                fprintf(output_stream, "ADDR %#04zx: LD V[%X], %#02x\n", i, X(opcode), NN(opcode));
                break;
            case 0x7: /* 0x7XNN, add NN to VX */
                fprintf(output_stream, "ADDR %#04zx: ADD V[%X], %#02x\n", i, X(opcode), NN(opcode));
                break;
            case 0x8: /* 0x8XY?, arithmetic operations */
                chip8_disassemble_8xxx(i, opcode, output_stream);
                break;
            case 0x9: /* 0x9XY0, skip next instruction if VX != VY */
                fprintf(output_stream, "ADDR %#04zx: SNE V[%X], V[%X]\n", i, X(opcode), Y(opcode));
                break;
            case 0xA: /* 0xANNN, set I to NNN */
                fprintf(output_stream, "ADDR %#04zx: LD I, %#03x\n", i, NNN(opcode));
                break;
            case 0xB: /* 0xBNNN, jump to address NNN + V0 */
                fprintf(output_stream, "ADDR %#04zx: JP V[0], %#03x\n", i, NNN(opcode));
                break;
            case 0xC: /* 0xCXNN, set VX to random number & NN */
                fprintf(output_stream, "ADDR %#04zx: RND V[%X], %#02x\n", i, X(opcode), NN(opcode));
                break;
            case 0xD: /* 0xDXYN, draw sprite at (VX, VY) with N bytes of sprite data starting at I */
                fprintf(output_stream, "ADDR %#04zx: DRW V[%X], V[%X], %#01x\n", i, X(opcode), Y(opcode), N(opcode));
                break;
            case 0xE: /* 0xEX??, Key ops */
                chip8_disassemble_exxx(i, opcode, output_stream);
                break;
            case 0xF: /* 0xFX??, other misc ops */
                chip8_disassemble_fxxx(i, opcode, output_stream);
                break;
            default: /* Unknown opcodes (note only original chip8 opcodes are supported, some roms use more */
                fprintf(output_stream, "ADDR %#04zx: UNKNOWN OPCODE\n", i);
                break;
        }
    }
    free(memory);
}

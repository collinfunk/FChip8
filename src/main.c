
#include <stdio.h>
#include <chip8.h>
#include <string.h>
#include <stdlib.h>

void print_help(char* filename);
void cmdline_call_disassemble(int argc, char** argv);
int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    /* Disassembler options */
    if ((strcmp(argv[1], "-d") == 0) || (strcmp(argv[1], "--disassemble") == 0)) {
        cmdline_call_disassemble(argc, argv);
    }
    return 0;
}

void print_help(char* filename) {
    printf("Usage: %s [options] [rom]\n", filename);
    printf("Options:\n");
    printf("  -h, --help\t\tPrint this help message\n");
    printf("  -d, --disassemble\tDisassemble the rom\n");
}

void cmdline_call_disassemble(int argc, char** argv) {
    /* Check if a rom was specified */
    if (argc < 3) {
        fprintf(stderr, "Error: No rom specified\n");
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* If no output file, stdout */
    if (argc == 3) {
        chip8_disassemble(argv[2], stdout);
    }

    /* If file, output to file */
    else if (argc == 4) {
        FILE *file_descriptor = fopen(argv[3], "w");
        if (file_descriptor == NULL) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[3]);
            exit(EXIT_FAILURE);
        }
        chip8_disassemble(argv[2], file_descriptor);
        fclose(file_descriptor);
    }

    /* Too many arguments */
    else {
        fprintf(stderr, "Error: Too many arguments\n");
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }
}

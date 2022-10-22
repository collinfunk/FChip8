
#include <stdio.h>
#include <chip8.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    const char* filename = argv[1];

    chip8_disassemble(filename, stdout);

    return 0;
}

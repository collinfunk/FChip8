
#ifndef CHIP8SDL_H_
#define CHIP8SDL_H_

#include <SDL2/SDL.h>
#include <chip8.h>
#include <stdbool.h>

struct SDLChip8 {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    int window_scale;
};

int sdl_chip8_init(struct SDLChip8 *sdl_chip8, int window_scale);
void sdl_chip8_destroy(struct SDLChip8 *sdl_chip8);
bool sdl_chip8_events(struct SDLChip8 *sdl_chip8, struct Chip8 *chip8);

#endif /* CHIP8SDL_H_ */

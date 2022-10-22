
#include <chip8.h>
#include <chip8sdl.h>

int sdl_chip8_init(struct SDLChip8 *sdl_chip8, int window_scale) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    sdl_chip8->window = SDL_CreateWindow(
            "FUNKEMU CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            CHIP8_DISPLAY_WIDTH * window_scale, CHIP8_DISPLAY_HEIGHT * window_scale,
            SDL_WINDOW_SHOWN);

    if (sdl_chip8->window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    sdl_chip8->renderer =
            SDL_CreateRenderer(sdl_chip8->window, -1, SDL_RENDERER_ACCELERATED);

    if (sdl_chip8->renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    sdl_chip8->texture = SDL_CreateTexture(
            sdl_chip8->renderer, SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING, CHIP8_DISPLAY_WIDTH, CHIP8_DISPLAY_HEIGHT);

    if (sdl_chip8->texture == NULL) {
        fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
        return 1;
    }

    sdl_chip8->window_scale = window_scale;

    return 0;
}

void sdl_chip8_destroy(struct SDLChip8 *sdl_chip8) {
    SDL_DestroyTexture(sdl_chip8->texture);
    SDL_DestroyRenderer(sdl_chip8->renderer);
    SDL_DestroyWindow(sdl_chip8->window);
    SDL_Quit();
}

bool sdl_chip8_events(struct SDLChip8 *sdl_chip8, struct Chip8 *chip8) {
    bool quit = false;

    while (SDL_PollEvent(&sdl_chip8->event)) {
        switch (sdl_chip8->event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (sdl_chip8->event.key.keysym.sym) {
                    case SDLK_1:
                        chip8->keypad[0x1] = 1;
                        break;
                    case SDLK_2:
                        chip8->keypad[0x2] = 1;
                        break;
                    case SDLK_3:
                        chip8->keypad[0x3] = 1;
                        break;
                    case SDLK_4:
                        chip8->keypad[0xC] = 1;
                        break;
                    case SDLK_q:
                        chip8->keypad[0x4] = 1;
                        break;
                    case SDLK_w:
                        chip8->keypad[0x5] = 1;
                        break;
                    case SDLK_e:
                        chip8->keypad[0x6] = 1;
                        break;
                    case SDLK_r:
                        chip8->keypad[0xD] = 1;
                        break;
                    case SDLK_a:
                        chip8->keypad[0x7] = 1;
                        break;
                    case SDLK_s:
                        chip8->keypad[0x8] = 1;
                        break;
                    case SDLK_d:
                        chip8->keypad[0x9] = 1;
                        break;
                    case SDLK_f:
                        chip8->keypad[0xE] = 1;
                        break;
                    case SDLK_z:
                        chip8->keypad[0xA] = 1;
                        break;
                    case SDLK_x:
                        chip8->keypad[0x0] = 1;
                        break;
                    case SDLK_c:
                        chip8->keypad[0xB] = 1;
                        break;
                    case SDLK_v:
                        chip8->keypad[0xF] = 1;
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (sdl_chip8->event.key.keysym.sym) {
                    case SDLK_1:
                        chip8->keypad[0x1] = 0;
                        break;
                    case SDLK_2:
                        chip8->keypad[0x2] = 0;
                        break;
                    case SDLK_3:
                        chip8->keypad[0x3] = 0;
                        break;
                    case SDLK_4:
                        chip8->keypad[0xC] = 0;
                        break;
                    case SDLK_q:
                        chip8->keypad[0x4] = 0;
                        break;
                    case SDLK_w:
                        chip8->keypad[0x5] = 0;
                        break;
                    case SDLK_e:
                        chip8->keypad[0x6] = 0;
                        break;
                    case SDLK_r:
                        chip8->keypad[0xD] = 0;
                        break;
                    case SDLK_a:
                        chip8->keypad[0x7] = 0;
                        break;
                    case SDLK_s:
                        chip8->keypad[0x8] = 0;
                        break;
                    case SDLK_d:
                        chip8->keypad[0x9] = 0;
                        break;
                    case SDLK_f:
                        chip8->keypad[0xE] = 0;
                        break;
                    case SDLK_z:
                        chip8->keypad[0xA] = 0;
                        break;
                    case SDLK_x:
                        chip8->keypad[0x0] = 0;
                        break;
                    case SDLK_c:
                        chip8->keypad[0xB] = 0;
                        break;
                    case SDLK_v:
                        chip8->keypad[0xF] = 0;
                        break;
                }
                break;
        }
    }
    return quit;
}

#include "vm/gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
SDL_Color fg = FG_COLOR, bg = BG_COLOR;

static void clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	SDL_RenderClear(renderer);
}

static int sdl_key_to_io_keycode(SDL_Keycode key) {
	if (key >= SDLK_a && key <= SDLK_z)
		return IO_KEY_A + key - SDLK_a; 

	switch (key) {
		case SDLK_ESCAPE: return IO_KEY_ESCAPE;
		case SDLK_RETURN: return IO_KEY_RETURN;
		case SDLK_DELETE: return IO_KEY_DELETE;
		case SDLK_LCTRL:
		case SDLK_RCTRL:  return IO_KEY_CONTROL;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT: return IO_KEY_SHIFT;
		case SDLK_LALT:
		case SDLK_RALT:   return IO_KEY_ALT;

		case SDLK_LEFT:   return IO_KEY_LEFT;
		case SDLK_RIGHT:  return IO_KEY_RIGHT;
		case SDLK_UP:     return IO_KEY_UP;
		case SDLK_DOWN:   return IO_KEY_DOWN;

		default: return IO_KEY_ANY;
	}
}

static void handle_events(cpu* cpu) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				if (cli_args.verbose) Log("quit!");
				cpu->flags.h = true;
				break;
			
			case SDL_KEYDOWN: {
				int io_keycode = sdl_key_to_io_keycode(event.key.keysym.sym);
				run_input_interrupt(cpu, io_keycode, true);
				break;
			}
			case SDL_KEYUP: {
				int io_keycode = sdl_key_to_io_keycode(event.key.keysym.sym);
				run_input_interrupt(cpu, io_keycode, false);
				break;
			}
		}
	}
}

void init_gui() {
	// init SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(
		WINDOW_WIDTH + 2 * WINDOW_PADDING,
		WINDOW_HEIGHT + 2 * WINDOW_PADDING,
		SDL_WINDOW_SHOWN,
		&window, &renderer
	);
	Assert(window && renderer,
		"failed to initialize SDL: %s", SDL_GetError());
	
	// init TTF
	Assert(!TTF_Init(),
		"failed to initialize TTF: %s", TTF_GetError());
	Assert(font = TTF_OpenFont(FONT_PATH, TEXT_SCALE),
		"failed to initialize font: %s", TTF_GetError());
	TTF_SetFontHinting(font, TTF_HINTING_MONO);

	// setup window
	SDL_SetWindowTitle(window, "S8BC");
	clear();
	SDL_RenderPresent(renderer);

	if (cli_args.verbose) Log("initialized SDL");
}

void draw_gui(cpu* cpu) {
	clear();

	// draw the text
	for (int y = 0; y < TEXT_HEIGHT; y++) {
		printf("Y = %d\n", y);
		for (int x = 0; x < TEXT_WIDTH; x++) {
			printf("X = %d\n", x);
			int offset = TEXT_START + y * TEXT_HEIGHT + x * TEXT_WIDTH;
			if (!cpu->memory[offset]) continue;
			char str[2] = {cpu->memory[offset], 0};

			SDL_Surface* surf = TTF_RenderText_Shaded(font, str, fg, bg);
			Assert(surf, "text rendering failed: %s", TTF_GetError());
			SDL_SetSurfaceAlphaMod(surf, 255);

			SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surf);
			Assert(text, "texture creation failed: %s", SDL_GetError());

			SDL_Rect rect = {0};
			SDL_GetClipRect(surf, &rect);
			rect.x = WINDOW_PADDING + x * TEXT_SCALE;
			rect.y = WINDOW_PADDING + y * TEXT_SCALE;
			printf("%dx%d -> '%s'\n", rect.x, rect.y, str);
			SDL_RenderCopy(renderer, text, NULL, &rect);

			SDL_DestroyTexture(text);
			SDL_FreeSurface(surf);
		}
	}

	SDL_RenderPresent(renderer);

	handle_events(cpu);
}

void quit_gui() {
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
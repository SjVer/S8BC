#include "vm/gui.h"

static void clear() {
}

static int sdl_key_to_io_keycode(int key) {
	// if (key >= SDLK_a && key <= SDLK_z)
	// 	return IO_KEY_A + key - SDLK_a; 

	// switch (key) {
	// 	case SDLK_ESCAPE: return IO_KEY_ESCAPE;
	// 	case SDLK_RETURN: return IO_KEY_RETURN;
	// 	case SDLK_DELETE: return IO_KEY_DELETE;
	// 	case SDLK_LCTRL:
	// 	case SDLK_RCTRL:  return IO_KEY_CONTROL;
	// 	case SDLK_LSHIFT:
	// 	case SDLK_RSHIFT: return IO_KEY_SHIFT;
	// 	case SDLK_LALT:
	// 	case SDLK_RALT:   return IO_KEY_ALT;

	// 	case SDLK_LEFT:   return IO_KEY_LEFT;
	// 	case SDLK_RIGHT:  return IO_KEY_RIGHT;
	// 	case SDLK_UP:     return IO_KEY_UP;
	// 	case SDLK_DOWN:   return IO_KEY_DOWN;

	// 	default: return IO_KEY_ANY;
	// }
}

static void handle_events(cpu* cpu) {
}

void init_gui() {
	clear();

	if (cli_args.verbose) Log("initialized GUI");
}

void draw_gui(cpu* cpu) {
	clear();

	handle_events(cpu);
}

void quit_gui() {
}
#define _GNU_SOURCE
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#include "vm/tty.h"

int pty_master_fd;
FILE* pty_slave_f;
volatile int screen_pid;

// static int sdl_key_to_io_keycode(int key) {
// 	if (key >= SDLK_a && key <= SDLK_z)
// 		return IO_KEY_A + key - SDLK_a; 
//
// 	switch (key) {
// 		case SDLK_ESCAPE: return IO_KEY_ESCAPE;
// 		case SDLK_RETURN: return IO_KEY_RETURN;
// 		case SDLK_DELETE: return IO_KEY_DELETE;
// 		case SDLK_LCTRL:
// 		case SDLK_RCTRL:  return IO_KEY_CONTROL;
// 		case SDLK_LSHIFT:
// 		case SDLK_RSHIFT: return IO_KEY_SHIFT;
// 		case SDLK_LALT:
// 		case SDLK_RALT:   return IO_KEY_ALT;
//
// 		case SDLK_LEFT:   return IO_KEY_LEFT;
// 		case SDLK_RIGHT:  return IO_KEY_RIGHT;
// 		case SDLK_UP:     return IO_KEY_UP;
// 		case SDLK_DOWN:   return IO_KEY_DOWN;
//
// 		default: return IO_KEY_ANY;
// 	}
// }

// static void handle_events(cpu* cpu) {
// }

void init_tty() {
	// initialize PTY
	pty_master_fd = posix_openpt(O_RDWR | O_NOCTTY);
	Assert(pty_master_fd,
		"failed to open PTY: %s", strerror(errno));
	Assert(!grantpt(pty_master_fd),
		"failed to grant PTY: %s", strerror(errno));
	Assert(!unlockpt(pty_master_fd),
		"failed to unlock PTY: %s", strerror(errno));

	char* pt_name = ptsname(pty_master_fd);
	pty_slave_f = fopen(pt_name, "rw");

	if (cli_args.verbose) Log("initialized PTY: %s", pt_name);

	if (cli_args.pty) {
		if (cli_args.verbose) Log("starting screen: " SCREEN_PATH);

		int child_pid = fork();
		if (child_pid == 0) {
			screen_pid = child_pid;

			// not working?
			const char* msg = "[" APP_NAME "] type [Ctrl a + k] to exit\n\r";			
			write(pty_master_fd, msg, strlen(msg));
			
			execl(SCREEN_PATH, SCREEN_PATH, SCREEN_ARGS, pt_name, 0);

			Log_err("screen failed: %s", strerror(errno));
			Abort(STATUS_INTERNAL_ERROR);
		}
	}
}

void update_tty(cpu* cpu) {
	// write(pty_master_fd, "update pty\n\r", 12);
	// THIS KEEPS THE PROGRAM AWAKE
	// char c = fgetc(pty_slave_f);
	// if (c) Log("  char: %c", c);
}

void quit_tty() {
	if (cli_args.pty && screen_pid) {
		Log("killing %d", screen_pid);
		kill(screen_pid, SIGKILL);
	}
}
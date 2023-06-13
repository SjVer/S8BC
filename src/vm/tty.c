#define _GNU_SOURCE
#include <sys/select.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#include "specs.h"
#include "vm/cpu.h"
#include "vm/tty.h"

int pty_master_fd;
int pty_slave_fd;
volatile int screen_pid;

void write_to_pty(char* str) {
    write(pty_master_fd, str, strlen(str));
}

void init_tty() {
	// initialize PTY
	pty_master_fd = posix_openpt(O_RDWR | O_NOCTTY);
	Assert(pty_master_fd,
		"failed to open PTY: %s", strerror(errno));
	Assert(!grantpt(pty_master_fd),
		"failed to grant PTM: %s", strerror(errno));
	Assert(!unlockpt(pty_master_fd),
		"failed to unlock PTM: %s", strerror(errno));
    Assert(!fcntl(pty_master_fd, F_SETFL, O_NONBLOCK),
        "failed to set PTM: %s", strerror(errno));

    // setup PTY slave
	char* pt_name = ptsname(pty_master_fd);
    FILE* pty_slave_f = fopen(pt_name, "rw");
    Assert(pty_slave_f, "failed to open PTS: %s", strerror(errno));
    pty_slave_fd = fileno(pty_slave_f);
    fcntl(pty_slave_fd, F_SETFL, O_NONBLOCK);

    struct termios ttmode = {0};
    // Assert(!tcgetattr(pty_slave_fd, &ttmode),
    //     "failed to get PTS attributes: %s", strerror(errno));
 
    // see `man termios`
    ttmode.c_iflag = ICRNL | IXON;
    ttmode.c_oflag = ONLCR | OPOST;
    ttmode.c_cflag = CREAD | CSIZE | CS8;
    ttmode.c_lflag = ISIG | IEXTEN;
    ttmode.c_cc[VMIN] = 0;
    ttmode.c_cc[VTIME] = 0;

    Assert(!tcsetattr(pty_slave_fd, TCSANOW, &ttmode),
        "failed to set PTS attributes: %s", strerror(errno));

	if (cli_args.verbose) {
        Log("initialized PTY: %s", pt_name);
        write_to_pty("[" APP_NAME "] type [Ctrl a + k] to exit\n\r");
    }

	if (cli_args.pty) {
        Log_err("TTY in console not yet supported");
		Abort(STATUS_INTERNAL_ERROR);

		// if (cli_args.verbose) Log("starting screen: " SCREEN_PATH);
		//
		// int child_pid = fork();
		// if (child_pid == 0) {
		// 	screen_pid = child_pid;
		//
		// 	// not working?
		// 	const char* msg = "[" APP_NAME "] type [Ctrl a + k] to exit\n\r";			
		// 	write(pty_master_fd, msg, strlen(msg));
		// 	
		// 	execl(SCREEN_PATH, SCREEN_PATH, SCREEN_ARGS, pt_name, 0);
		//
		// 	Log_err("screen failed: %s", strerror(errno));
		// 	Abort(STATUS_INTERNAL_ERROR);
		// }
	}
}

void update_tty(cpu* cpu) {
    // check for- and get input (non blocking)
    // NOTE: errno is always EAGAIN (due to blocking issues?)
    int len = read(pty_master_fd, &cpu->memory[IO_TTY_INPUT], 1);
    if (len == 1) start_input_interrupt(cpu);

    // write output
    if (cpu->memory[IO_TTY_OUTPUT]) {
        write(pty_master_fd, &cpu->memory[IO_TTY_OUTPUT], 1);
        cpu->memory[IO_TTY_OUTPUT] = 0;
    }
}

void quit_tty() {
	// if (cli_args.pty && screen_pid) {
	// 	Log("killing %d", screen_pid);
	// 	kill(screen_pid, SIGKILL);
	// }
}

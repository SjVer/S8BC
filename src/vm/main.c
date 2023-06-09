#include <argp.h>
#include <stdlib.h>

#include "instructions.h"
#include "vm/common.h"
#include "vm/tty.h"
#include "vm/cpu.h"

struct cli_args cli_args = {
    .verbose = false,
    .rom_file = NULL,
    .pty = false,
    .debug = false,
    .slow = false,
};

// CLI argument stuff
#pragma region

const char* argp_program_version = APP_NAME " " APP_VERSION;
const char* argp_program_bug_address = "\b\b\bat " PROJECT_LINK;
static char args_doc[] = "[ROM file]";

static struct argp_option options[] = {
    {"help", 	 'h', 0, 0, "Display a help message."},
    {"version",  'V', 0, 0, "Display version information."},
    {"usage", 	 'u', 0, 0, "Display a usage information message."},
    {"verbose",  'v', 0, 0, "Produce verbose output."},
    {"pty",      'p', 0, 0, "Have a pseudo-TTY in the console."},
    {"debug",    'd', 0, 0, "Produce debug output."},
    {"slow",     's', 0, 0, "Slow down execution."},
    {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = {options, parse_opt, args_doc, APP_DOC};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct cli_args* args = state->input;
    switch (key) {
        case 'h': {
            int flags = ARGP_HELP_LONG | ARGP_HELP_EXIT_OK
                      | ARGP_HELP_DOC | ARGP_HELP_BUG_ADDR;
            argp_help(&argp, stdout, flags, APP_NAME);
            exit(0);
        }
        case 'V':
            printf("%s\n", argp_program_version);
            exit(0);
        case 'u':
            argp_usage(state);
            exit(0);
        case 'v':
            args->verbose = true;
            break;

        case 'p':
            args->pty = true;
            break;
        case 'd':
            args->debug = true;
            break;
        case 's':
            args->slow = true;
            break;

        case ARGP_KEY_ARG:
            if (args->rom_file) argp_usage(state);
            args->rom_file = arg;
            break;
    
        case ARGP_KEY_END:
            if (!args->rom_file) argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

#pragma endregion

byte* read_rom_file() {
    FILE* fp = fopen(cli_args.rom_file, "rb");
    Assert(fp, "could not open ROM file: %s", cli_args.rom_file);
    
    fseek(fp, 0, SEEK_END);
    word size = ftell(fp);
    Assert(size <= ROM_SIZE, "loaded ROM too large");
    rewind(fp);

    byte* data = malloc(size);
    fread(data, size, 1, fp);
    return data;
}

volatile cpu* main_cpu;
volatile bool is_stubborn;

void halt_cpu() {
    if (is_stubborn) {
        if (cli_args.verbose) Log("forcefully interrupted!");
        raise(SIGKILL);
    } else {
        if (cli_args.verbose) Log("interrupted!");
        main_cpu->flags.h = true;
        is_stubborn = true;
    }
}

int main(int argc, char** argv) {
    if (argp_parse(&argp, argc, argv, 0, 0, &cli_args))
        Abort(STATUS_CLI_ERROR);
    
    // initialize the CPU
    struct cpu cpu;
    reset_cpu(&cpu);

    // register the interrupt handler
    main_cpu = &cpu;
    is_stubborn = false;
    signal(SIGINT, halt_cpu);

    // initalize the PTY
    init_tty();

    // read and load ROM
    byte* rom = read_rom_file();
    load_rom(&cpu, rom);
    free(rom);

    // run the CPU
    load_reset_vector(&cpu);
    execute(&cpu);

    // stop the GUI
    quit_tty();

    return STATUS_SUCCESS;
}
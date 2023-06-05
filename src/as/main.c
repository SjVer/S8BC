#include <argp.h>
#include <stdlib.h>

#include "as/common.h"
#include "as/lexer.h"
#include "as/parser.h"

struct cli_args cli_args = {
    .verbose = false,
    .asm_file = NULL,
    .out_file = NULL,
};

// CLI argument stuff
#pragma region

const char* argp_program_version = APP_NAME " " APP_VERSION;
const char* argp_program_bug_address = "\b\b\bat " PROJECT_LINK;
static char args_doc[] = "[assembly file]";

static struct argp_option options[] = {
    {"help", 	 'h', 0,      0, "Display a help message."},
    {"version",  'V', 0,      0, "Display version information."},
    {"usage", 	 'u', 0,      0, "Display a usage information message."},
    {"verbose",  'v', 0,      0, "Produce verbose output."},
    {"output",   'o', "FILE", 0, "Redirect output to FILE."},
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

        case 'o':
            args->out_file = arg;
            break;

        case ARGP_KEY_ARG:
            if (args->asm_file) argp_usage(state);
            args->asm_file = arg;
            break;
    
        case ARGP_KEY_END:
            if (!args->asm_file) argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

#pragma endregion

int main(int argc, char** argv) {
    if (argp_parse(&argp, argc, argv, 0, 0, &cli_args))
        Abort(STATUS_CLI_ERROR);

    // read input file
    FILE* fp = fopen(cli_args.asm_file, "r");
    Assert(fp, "could not open file: %s", cli_args.asm_file);
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    char* source = malloc(size);
    fread(source, size, 1, fp);

    // init lexer and parser
    init_lexer(source);
    init_parser();
    

    return STATUS_SUCCESS;
}
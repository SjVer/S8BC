#pragma once
#define VM_COMMON_H

#include "../common.h"

// constant macros
#pragma region

// app info
#pragma region
#define APP_NAME PROJECT_NAME_INTERNAL "-vm"
#define APP_DOC \
    APP_NAME " -- The " PROJECT_NAME " virtual machine.\n" \
    "\v" \
    "More information at " PROJECT_LINK ".\n" \
    "Build: " __DATE__ " " __TIME__ " on " OS_NAME " (" COMPILER ")"

#define APP_VERSION_MAJOR 0
#define APP_VERSION_MINOR 1
#ifndef APP_VERSION_PATCH
#error "APP_VERSION_PATCH not defined!"
#endif
#define APP_VERSION \
    STRINGIFY(APP_VERSION_MAJOR) "." \
    STRINGIFY(APP_VERSION_MINOR) "." \
    APP_VERSION_PATCH

#pragma endregion

extern struct cli_args {
    bool verbose;
    char* rom_file;
    bool pty;
    bool slow;
    bool debug;
} cli_args;

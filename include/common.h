#pragma once
#define COMMON_H

// includes
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

// macro constants
#pragma region

// compiler specifics
#ifndef COMPILER
#pragma warning "COMPILER not set! Defaulting to \"unkown\"."
#define COMPILER "unknown"
#endif

// os specifics
#ifdef _WIN32
    #define OS_NAME "Windows 32-bit"
    #define PATH_SEPARATOR '\\'
#elif _WIN64
    #define OS_NAME "Windows 64-bit"
    #define PATH_SEPARATOR '\\'
#elif __APPLE__ || __MACH__
    #define OS_NAME "Mac OS_NAMEX"
    #define PATH_SEPARATOR '/'
#elif __linux__
    #define OS_NAME "Linux"
    #define PATH_SEPARATOR '/'
#elif __unix || __unix__
    #define OS_NAME "Unix"
    #define PATH_SEPARATOR '/'
#else
    #define OS_NAME "Unknown OS"
    #define PATH_SEPARATOR '/'
#endif

// project info
#define PROJECT_NAME "S8BC"
#define PROJECT_NAME_INTERNAL "s8bc"
#define PROJECT_LINK "https://github.com/SjVer/S8BC"

#pragma endregion

// useful macros
#pragma region

#define __STRINGIFY(value) #value
#define STRINGIFY(value) __STRINGIFY(value)

#define Log(...) \
    (printf("[" APP_NAME "] " __VA_ARGS__), putchar('\n'))
#define Log_err(...) \
    fprintf(stderr, "[" APP_NAME "] ERROR: " __VA_ARGS__)
#define Abort(status) \
    (Log_err("aborted with status %d", status), exit(status))
#define Internal_error(...) \
    (Log_err(__VA_ARGS__), raise(SIGINT))
#define Assert(condition, ...) \
    {if(!(condition)) Internal_error(__VA_ARGS__);}

#pragma endregion

typedef uint8_t byte;

#define STATUS_SUCCESS 0
#define STATUS_CLI_ERROR 64
#define STATUS_PARSE_ERROR 66
#define STATUS_TYPE_ERROR 67
#define STATUS_CODEGEN_ERROR 68
#define STATUS_OUTPUT_ERROR 69
#define STATUS_Internal_error -1
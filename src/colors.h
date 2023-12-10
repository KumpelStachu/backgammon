#pragma once
#include <ncurses.h>
#include <stdint.h>

#define COLORS_LIST           \
    X(0x00, BLACK, BLACK)     \
    X(0x10, RED, BLACK)       \
    X(0x20, GREEN, BLACK)     \
    X(0x30, YELLOW, BLACK)    \
    X(0x40, BLUE, BLACK)      \
    X(0x50, MAGENTA, BLACK)   \
    X(0x60, CYAN, BLACK)      \
    X(0x70, WHITE, BLACK)     \
    X(0x01, BLACK, RED)       \
    X(0x11, RED, RED)         \
    X(0x21, GREEN, RED)       \
    X(0x31, YELLOW, RED)      \
    X(0x41, BLUE, RED)        \
    X(0x51, MAGENTA, RED)     \
    X(0x61, CYAN, RED)        \
    X(0x71, WHITE, RED)       \
    X(0x02, BLACK, GREEN)     \
    X(0x12, RED, GREEN)       \
    X(0x22, GREEN, GREEN)     \
    X(0x32, YELLOW, GREEN)    \
    X(0x42, BLUE, GREEN)      \
    X(0x52, MAGENTA, GREEN)   \
    X(0x62, CYAN, GREEN)      \
    X(0x72, WHITE, GREEN)     \
    X(0x03, BLACK, YELLOW)    \
    X(0x13, RED, YELLOW)      \
    X(0x23, GREEN, YELLOW)    \
    X(0x33, YELLOW, YELLOW)   \
    X(0x43, BLUE, YELLOW)     \
    X(0x53, MAGENTA, YELLOW)  \
    X(0x63, CYAN, YELLOW)     \
    X(0x73, WHITE, YELLOW)    \
    X(0x04, BLACK, BLUE)      \
    X(0x14, RED, BLUE)        \
    X(0x24, GREEN, BLUE)      \
    X(0x34, YELLOW, BLUE)     \
    X(0x44, BLUE, BLUE)       \
    X(0x54, MAGENTA, BLUE)    \
    X(0x64, CYAN, BLUE)       \
    X(0x74, WHITE, BLUE)      \
    X(0x05, BLACK, MAGENTA)   \
    X(0x15, RED, MAGENTA)     \
    X(0x25, GREEN, MAGENTA)   \
    X(0x35, YELLOW, MAGENTA)  \
    X(0x45, BLUE, MAGENTA)    \
    X(0x55, MAGENTA, MAGENTA) \
    X(0x65, CYAN, MAGENTA)    \
    X(0x75, WHITE, MAGENTA)   \
    X(0x06, BLACK, CYAN)      \
    X(0x16, RED, CYAN)        \
    X(0x26, GREEN, CYAN)      \
    X(0x36, YELLOW, CYAN)     \
    X(0x46, BLUE, CYAN)       \
    X(0x56, MAGENTA, CYAN)    \
    X(0x66, CYAN, CYAN)       \
    X(0x76, WHITE, CYAN)      \
    X(0x07, BLACK, WHITE)     \
    X(0x17, RED, WHITE)       \
    X(0x27, GREEN, WHITE)     \
    X(0x37, YELLOW, WHITE)    \
    X(0x47, BLUE, WHITE)      \
    X(0x57, MAGENTA, WHITE)   \
    X(0x67, CYAN, WHITE)      \
    X(0x77, WHITE, WHITE)

#define X(id, fg, bg)          \
    void attron_##fg##_##bg(); \
    void attroff_##fg##_##bg();
COLORS_LIST
#undef X

void attron_BOLD();
void attroff_BOLD();

void init_colors();

#define PAIR_BLACK_WHITE 0
#define PAIR_RED_BLACK 1

#define ATTR_BLACK_WHITE COLOR_PAIR(PAIR_BLACK_WHITE)
#define ATTR_RED_BLACK COLOR_PAIR(PAIR_RED_BLACK)